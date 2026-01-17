#include <system.h>
#include <string.h>
#include <mem.h>
#include <io.h>
#include "fb.h"

//mouse button flags
#define MOUSE_BTN_LEFT      0x01
#define MOUSE_BTN_RIGHT     0x02
#define MOUSE_BTN_MIDDLE    0x04

//mouse event structure
typedef struct {
    int16 dx; //x movement delta
    int16 dy; //y movement delta
    uint8 buttons; //button state (MOUSE_BTN_*)
    uint8 _pad[3];
} mouse_event_t;

//cursor API
int cursor_get_width(void);
int cursor_get_height(void);
uint32 cursor_get_pixel(int x, int y);

uint32 *fb = NULL;
handle_t fb_handle = INVALID_HANDLE;

int32 cursor_x = FB_W / 2;
int32 cursor_y = FB_H / 2;

static void draw_cursor(uint32 *framebuffer, int x, int y) {
    int cw = cursor_get_width();
    int ch = cursor_get_height();
    for (int cy = 0; cy < ch; cy++) {
        for (int cx = 0; cx < cw; cx++) {
            uint32 pixel = cursor_get_pixel(cx, cy);
            if (pixel & 0xFF000000) {  // has alpha
                int fx = x + cx;
                int fy = y + cy;
                if (fx >= 0 && fx < FB_W && fy >= 0 && fy < FB_H) {
                    framebuffer[fy * FB_W + fx] = pixel & 0x00FFFFFF;
                }
            }
        }
    }
}

static void clear_cursor(uint32 *framebuffer, int x, int y) {
    int cw = cursor_get_width();
    int ch = cursor_get_height();
    fb_fillrect(framebuffer, x, y, cw, ch, 0);
}

typedef struct {
    uint16 width, height;
    char *title;
} window_req_t;

void mouse_tick(handle_t mouse_channel) {
    mouse_event_t event;
    int len = channel_recv(mouse_channel, &event, sizeof(event));
    
    if (len <= 0) {
        yield();
        return;
    }
    
    if (len == sizeof(mouse_event_t)) {
        clear_cursor(fb, cursor_x, cursor_y);
        
        cursor_x += event.dx;
        cursor_y += event.dy;
        
        if (cursor_x >= FB_W) cursor_x = FB_W - 1;
        if (cursor_x < 0) cursor_x = 0;
        if (cursor_y >= FB_H) cursor_y = FB_H - 1;
        if (cursor_y < 0) cursor_y = 0;


        if (event.buttons & MOUSE_BTN_LEFT) { // test - spawn a window on click
            handle_t create = get_obj(INVALID_HANDLE, "$gui/window/create", RIGHT_WRITE | RIGHT_READ);
            window_req_t req = { .width = 800, .height = 600, .title = "Test App" };
            channel_send(create, &req, sizeof(req));

            handle_t window;
            channel_recv(create, &window, sizeof(window));
        }

        draw_cursor(fb, cursor_x, cursor_y);
        handle_write(fb_handle, fb, 4096000);
        handle_seek(fb_handle, 0, HANDLE_SEEK_SET);
    } else {
        printf("Received unexpected message size: %d\n", len);
    }
}

void window_create(handle_t window_channel) {
    channel_recv_result_t res;
    int len = channel_try_recv_msg(window_channel, NULL, 0, NULL, 0, &res);
    if (len <= 0) {
        yield();
        return;
    }

    handle_t ep0, ep1;
    channel_create(&ep0, &ep1);

    char path[64];
    snprintf(path, sizeof(path), "$gui/window/%d", res.sender_pid);
    ns_register(path, ep0);

    channel_send(window_channel, &ep1, sizeof(ep1));
}

int main(int argc, char *argv[]) {
    fb = malloc(FB_W * FB_H * sizeof(uint32));
    fb_handle = get_obj(INVALID_HANDLE, "$devices/fb0", RIGHT_WRITE);
    
    handle_t mouse_channel = get_obj(INVALID_HANDLE, "$devices/mouse/channel", RIGHT_READ);
    if (mouse_channel == INVALID_HANDLE) {
        puts("Failed to open $devices/mouse/channel\n");
        return 1;
    }

    //create an endpoint for window creation
    handle_t window_channel, empty;
    channel_create(&window_channel, &empty);
    ns_register("$gui/window/create", window_channel);

    while (1) {
        mouse_tick(mouse_channel);
        window_create(window_channel);
    }
    
    return 0;
}
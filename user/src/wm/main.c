#include <system.h>
#include <string.h>
#include <mem.h>
#include <io.h>
#include "fb.h"

#define FB_BACKBUFFER_SIZE  (1280 * 800 * sizeof(uint32))

static inline int max(int a, int b) {
    return (a > b) ? a : b;
}

static inline int min(int64 a, int64 b) {
    return (a < b) ? a : b;
}

void fb_setup(handle_t *h, uint32 **backbuffer) {
    *h = get_obj(INVALID_HANDLE, "$devices/fb0", RIGHT_READ | RIGHT_WRITE);
    *backbuffer = malloc(FB_BACKBUFFER_SIZE);
}

void kbd_setup(handle_t *h) {
    *h = get_obj(INVALID_HANDLE, "$devices/keyboard", RIGHT_READ);
}

void server_setup(handle_t *server, handle_t *client) {
    channel_create(server, client);
    ns_register("$gui/wm", *client);
}

typedef struct {
    enum {
        CREATE, COMMIT, DESTROY,
    } type;
    union {
        struct {
            uint16 width, height;
        } create;
        struct {

        } commit;
        struct {

        } destroy;
    } u;
} wm_req_t;

typedef struct {
    bool ack;
    union {
        struct {

        } create;
        struct {

        } commit;
        struct {

        } destroy;
    } u;
} wm_res_t;

typedef struct {
    uint32 pid;
    uint32 *surface;
    handle_t vmo_handle;
    uint16 surface_w, surface_h;
    uint16 win_w, win_h;
    uint16 x, y;
    bool dirty;
} wm_client_t;

wm_client_t clients[16];
uint8 num_clients = 0;

#define WM_ACK  0xFF

void recompute_layout(uint16 screen_w, uint16 screen_h) {
    uint16 tile_h = screen_h / num_clients;
    for (int i = 0; i < num_clients; i++) {
        clients[i].x = 0;
        clients[i].y = i * tile_h;
        clients[i].win_w = screen_w;
        clients[i].win_h = (i == num_clients - 1) ? (screen_h - tile_h * (num_clients - 1)) : tile_h;
        clients[i].dirty = true;
    }
}

void window_create(handle_t *server, channel_recv_result_t res, wm_req_t req) {
    if (num_clients == 16) return; //ignore for now

    //create surface
    char path[64];
    handle_t client_vmo = vmo_create(req.u.create.width * req.u.create.height * sizeof(uint32), VMO_FLAG_NONE, RIGHT_MAP);
    snprintf(path, sizeof(path), "$gui/%d/surface", res.sender_pid);
    ns_register(path, client_vmo);
    uint32 *surface = vmo_map(client_vmo, NULL, 0, req.u.create.width * req.u.create.height * sizeof(uint32), RIGHT_MAP);
    if (!surface) debug_puts("wm: failed to map client's surface\n");

    clients[num_clients++] = (wm_client_t){
        .pid = res.sender_pid,
        .surface = surface,
        .vmo_handle = client_vmo,
        .surface_w = req.u.create.width,
        .surface_h = req.u.create.height,
        .win_w = req.u.create.width,
        .win_h = req.u.create.height,
        .x = 0,
        .y = 0,
        .dirty = false,
    };

    wm_res_t resp = (wm_res_t){ .ack = true };
    channel_send(*server, &resp, sizeof(resp));

    recompute_layout(1280, 800);
}

void window_commit(handle_t *server, channel_recv_result_t res, wm_req_t req) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].pid == res.sender_pid) {
            clients[i].dirty = true;
            wm_res_t resp = (wm_res_t){ .ack = true };
            channel_send(*server, &resp, sizeof(resp));
            return;
        }
    }
}

void server_listen(handle_t *server) {
    wm_req_t msg;
    channel_recv_result_t res;
    if (channel_recv_msg(*server, &msg, sizeof(wm_req_t), NULL, 0, &res) != 0) {
        yield(); return;
    }

    switch (msg.type) {
        case CREATE: window_create(server, res, msg); break;
        case COMMIT: window_commit(server, res, msg); break;
    }
}

void render_surfaces(handle_t fb_handle, uint32 *fb_backbuffer) {
    if (num_clients < 1) return;
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].dirty == false) continue;
        wm_client_t c = clients[i];

        //visible copy rectangle in fb space
        int dst_x0 = max(c.x, 0);
        int dst_y0 = max(c.y, 0);
        int dst_x1 = min(c.x + c.win_w, FB_W);
        int dst_y1 = min(c.y + c.win_h, FB_H);
        if (dst_x0 >= dst_x1 || dst_y0 >= dst_y1) continue; //nothing visible, skip

        //how many pixels to copy
        int copy_w = dst_x1 - dst_x0;
        int copy_h = dst_y1 - dst_y0;

        //where in the surface to start
        int src_x0 = dst_x0 - c.x;
        int src_y0 = dst_y0 - c.y;

        //clamp
        src_x0 = max(src_x0, 0);
        src_y0 = max(src_y0, 0);
        copy_w = min(copy_w, c.surface_w - src_x0);
        copy_h = min(copy_h, c.surface_h - src_y0);

        if (copy_w <= 0 || copy_h <= 0) continue; //nothing to copy, skip

        for (int row = 0; row < copy_h - 1; row++) {
            uint32 *src_row = c.surface + (src_y0 + row) * c.surface_w + src_x0;
            uint32 *dst_row = fb_backbuffer + (dst_y0 + row) * FB_W + dst_x0;
            memcpy(dst_row, src_row, copy_w * sizeof(uint32));
        }
        
        clients[i].dirty = false;
    }
    handle_seek(fb_handle, 0, HANDLE_SEEK_SET);
    handle_write(fb_handle, fb_backbuffer, FB_BACKBUFFER_SIZE);
}

int main(void) {
    handle_t fb_handle = INVALID_HANDLE;
    uint32 *fb_backbuffer = NULL;
    handle_t kbd_handle = INVALID_HANDLE;
    handle_t server_handle = INVALID_HANDLE;
    handle_t client_handle = INVALID_HANDLE;

    fb_setup(&fb_handle, &fb_backbuffer);
    kbd_setup(&kbd_handle);
    server_setup(&server_handle, &client_handle);

    spawn("$files/system/binaries/app", 0, NULL);
    spawn("$files/system/binaries/app", 0, NULL);
    spawn("$files/system/binaries/app", 0, NULL);
    spawn("$files/system/binaries/app", 0, NULL);
    while (1) {
        server_listen(&server_handle);
        // handle_input();
        render_surfaces(fb_handle, fb_backbuffer);
        
        yield();
    }
    __builtin_unreachable();
    return 0;
}

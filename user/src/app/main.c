#include <system.h>

typedef struct {
    uint16 width, height;
    char *title;
} window_req_t;

int main(void) {
    handle_t create = get_obj(INVALID_HANDLE, "$gui/window/create", RIGHT_READ | RIGHT_WRITE);
    window_req_t req = { .width = 800, .height = 600, .title = "Test App"};
    channel_send(create, &req, sizeof(req));

    handle_t window;
    channel_recv(create, &window, sizeof(window));
    return 0;
}
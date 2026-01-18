#ifndef WM_PROTOCOL_H
#define WM_PROTOCOL_H

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
    enum {
        ACK,
        CONFIGURE,
    } type;
    union {
        bool ack;
        struct {
            uint16 x, y, w, h;
        } configure;
    } u;
} wm_res_t;

#endif
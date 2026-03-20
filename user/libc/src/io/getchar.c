#include <stdio.h>

char getchar(FILE* f) {
    if (f->handle == INVALID_HANDLE) return -1;
    char c;
    int code;
    if ((code = handle_read(f->handle, &c, 1)) < 0) {
        return code;
    }
    return c;
}

#include <string.h>
#include <system.h>
#include <io.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: read <file> [-n | --new-line]\n");
        return 1;
    }
    
    handle_t file = get_obj(INVALID_HANDLE, argv[1], RIGHT_READ);
    if (file == INVALID_HANDLE) {
        printf("read: cannot open '%s'\n", argv[1]);
        return 1;
    }
    bool add_new_line = false;

    if (argc >= 3 && (streq(argv[2], "-n") || streq(argv[2], "--new-line"))) {
	add_new_line = true;
    }
    
    char buf[512];
    int len;
    while ((len = handle_read(file, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < len; i++) {
            putc(buf[i]);
        }
    }

    if (add_new_line) printf("\n");
    handle_close(file);
    return 0;
}

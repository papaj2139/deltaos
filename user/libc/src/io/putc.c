#include <sys/syscall.h>

void putc(const char c) {
    __syscall2(SYS_WRITE, (long)&c, 1);
}
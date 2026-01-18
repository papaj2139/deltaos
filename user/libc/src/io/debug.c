#include <sys/syscall.h>
#include <string.h>

void debug_puts(const char *str) {
    __syscall2(SYS_DEBUG_WRITE, (long)str, (long)strlen(str));
}
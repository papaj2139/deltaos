#include <sys/syscall.h>
#include <string.h>

void puts(const char *str) {
    size len = strlen(str);
    __syscall2(SYS_WRITE, (long)str, (long)len);
}

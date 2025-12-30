#include <types.h>
#include <sys/syscall.h>

int64 getpid(void) {
    return __syscall0(SYS_GETPID);
}
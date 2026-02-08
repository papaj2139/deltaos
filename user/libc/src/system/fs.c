#include <system.h>
#include <sys/syscall.h>

int mkdir(const char *path) {
    return __syscall2(SYS_MKDIR, (long)path, (long)FS_TYPE_DIR);
}

int mkfile(const char *path) {
    return __syscall2(SYS_MKDIR, (long)path, (long)FS_TYPE_FILE);
}

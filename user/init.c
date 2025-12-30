#include <system.h>
#include <io.h>

int main(int argc, char *argv[]) {
    puts("[user] hello from userspace!\n");

    printf("[user] argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("[user] argv[%d] = %s\n", i, argv[i]);
    }

    printf("[user] getpid() = %d\n", (int)getpid());
    
    puts("[user] syscall test complete, exiting\n");
    
    return 0;
}

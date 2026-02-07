#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char**argv) {
    if (argc < 2) {
        puts("Usage: mkdir <path>\n");
        return 0;
    }
    if (mkdir(argv[1], 0755) < 0) {
        printf("Failed to create path: '%s'\n", argv[1]);
    }
}

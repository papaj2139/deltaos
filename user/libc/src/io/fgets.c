#include <types.h>
#include <stdio.h>

char* fgets(char* buf, int bufsz, FILE* f) {
    char c;
    size_t pos = 0;
    
    while ((c = getchar(f)) == 0) {
        if (pos >= (bufsz - 1)) {
            return buf;
        }
        buf[pos++] = c;
        
        if (c == '\n') {
            return buf;
        }
    }
    
    // we should only reach this if an error occurs
    return NULL;
}

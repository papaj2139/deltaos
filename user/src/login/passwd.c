#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "passwd.h"

// this returns a malloc'd string, make sure to free it!!!
char* serialize_passwd(struct passwd* pwd) {
    size_t sz = strlen(pwd->username) + strlen(pwd->pwd_hash) + 1; // username + delim (,) + hex hash
    char* str = malloc(sz);
    if (str == NULL) {
        return NULL;
    }
    
    if (snprintf(str, sz, "%s,%s", pwd->username, pwd->pwd_hash) != sz) {
        return NULL;
    }
    return str;
}

// returns a malloc'd struct, free it!
struct passwd* deserialize_passwd(char* str) {
    size_t sz = 256 + 1 + 64;
    
    if (strlen(str) != sz) {
        errno = EINVAL;
        return NULL;
    }
    
    char* delim = strchr(str, ',');
    if (delim == NULL) {
        errno = EINVAL;
        return NULL;
    }
    
    *delim = '\0';
    char* pwd_hash = delim + 1;
    
    struct passwd* pwd = malloc(sz);
    memset(pwd, 0x00, sz);
    
    memcpy(pwd->username, str, strlen(str));
    memcpy(pwd->pwd_hash, pwd_hash, strlen(pwd_hash));
    
    return pwd;
}

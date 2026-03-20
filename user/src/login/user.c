#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "user.h"
#include "passwd.h"
#include "sha256.h"

int create_user(char* username, char* pt_pwd) {
    struct passwd* pwd;

    size_t usrname_len = strlen(username);
    
    if (usrname_len > 256) {
        errno = EINVAL;
        return -1;
    }
    memcpy(pwd->username, username, strlen(username));
    
    uint8_t shabuf[32];
    sha256(pt_pwd, strlen(pt_pwd), shabuf);
    sha256_to_hex(shabuf, pwd->pwd_hash);
    
    FILE* passwd = fopen("/etc/passwd", "aw");
    if (passwd == NULL) {
        return -1;
    }
    
    char* pwd_str = serialize_passwd(pwd);
    if (pwd_str == NULL) {
        return -1;
    }
    
    if (fprintf(passwd, "%s\n", pwd_str) != (strlen(pwd_str) + 1)) {
        return -1;
    }
    
    free(pwd_str);
    
    return 0;
}

// the struct returnd will be malloc'd, free it when your done!
struct passwd* get_user(const char* username) {
    FILE* passwd = fopen("/etc/passwd", "r");
    if (passwd == NULL) {
        return NULL;
    }
    
    // TODO: loop through lines, deserializing each
    // if we hit a matching username, return that one
    
    const size_t line_sz = 256 + 1 + 64 + 1; // usrname + delim + pwdhash + null ending / newline
    
    char line[line_sz];
    while (fgets(line, line_sz, passwd) != NULL) {
        struct passwd* pwd = deserialize_passwd(line);
        if (strcmp(pwd->username, username) == 0) {
            fclose(passwd);
            return pwd;
        } else {
            free(pwd);
        }
    }
    
    fclose(passwd);
    return NULL;
}

enum verif_stat verify_user(char* username, char* pt_pwd) {
    enum verif_stat code;
    
    struct passwd* passwd = get_user(username);
    if (passwd == NULL) {
        return ENUSR; // its possible this was also an internal error, such as failing to open /etc/passwd
            // we should later make it so in that case, EINTR is actually returned, not ENUSR
    }
    
    uint8_t shabuf[32];
    char shahex[65];
    sha256(pt_pwd, strlen(pt_pwd), shabuf);
    sha256_to_hex(shabuf, shahex);
    
    if (strcmp(passwd->pwd_hash, shahex) == 0) {
        code = VALID;
    } else {
        code = EWPWD;
    }
    
    free(passwd);
    return code;
}

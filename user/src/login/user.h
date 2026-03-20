#ifndef __USER_H
#define __USER_H

#include "passwd.h"

// -1 = error
// 0 = success
int create_user(char* usrname, char* pwd);

enum verif_stat {
    EWPWD, // wrong password
    ENUSR, // user not exist
    EINTR, // internal error
    VALID // valid login
};

enum verif_stat verify_user(char* usr, char* pwd);

#endif

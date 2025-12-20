#ifndef LIB_STRING_H
#define LIB_STRING_H

#include <types.h>

uint8 atoi(char *p);
size strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size n);
char *strchr(const char *s, int c);
char *strtok(char *str, const char *delim);

#endif
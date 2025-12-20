#ifndef _TYPES_H
#define _TYPES_H

//exact width integer types
typedef signed char        int8;
typedef unsigned char      uint8;
typedef signed short       int16;
typedef unsigned short     uint16;
typedef signed int         int32;
typedef unsigned int       uint32;
typedef signed long long   int64;
typedef unsigned long long uint64;

//mnimum-width integer types
typedef int8   least8;
typedef uint8  uleast8;
typedef int16  least16;
typedef uint16 uleast16;
typedef int32  least32;
typedef uint32 uleast32;
typedef int64  least64;
typedef uint64 uleast64;

//fastest minimum-width integer types
typedef int8   fast8;
typedef uint8  ufast8;
typedef int64  fast16;
typedef uint64 ufast16;
typedef int64  fast32;
typedef uint64 ufast32;
typedef int64  fast64;
typedef uint64 ufast64;

//pointer-sized integers (amd64)
typedef int64  intptr;
typedef uint64 uintptr;

//maximum-width integer types
typedef int64  intmax;
typedef uint64 uintmax;

//limits
#define INT8_MIN   (-128)
#define INT8_MAX   127
#define UINT8_MAX  255

#define INT16_MIN  (-32768)
#define INT16_MAX  32767
#define UINT16_MAX 65535

#define INT32_MIN  (-2147483647 - 1)
#define INT32_MAX  2147483647
#define UINT32_MAX 4294967295U

#define INT64_MIN  (-9223372036854775807LL - 1)
#define INT64_MAX  9223372036854775807LL
#define UINT64_MAX 18446744073709551615ULL

#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX

#define SIZE_MAX    UINT64_MAX

#define NULL ((void *)0)

//size type (for amd64 alteast)
typedef unsigned long long size;

//signed size type
typedef long long ssize;

typedef char bool;
#define true 1
#define false 0

#endif

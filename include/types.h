#ifndef TYPES_H
#define TYPES_H

#define true  1
#define false 0
#define NULL ((void *)0)

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;

typedef u32 size_t;
typedef u32 uintptr_t;
//typedef u8  bool;

_Static_assert(sizeof(u8)  == 1, "u8 must be 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
_Static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
_Static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");
_Static_assert(sizeof(void *) == 4, "pointers must be 32-bit");

#endif

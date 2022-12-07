#ifndef TYPES_H
#define TYPES_H

#define true (1)
#define false (0)
#define NULL (0)
#define nullptr ((void*)0)
#define EOS ('\0')
#define _packed __attributes__((packed))

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef int size_t;
#endif
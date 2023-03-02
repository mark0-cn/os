#ifndef STDIO_H
#define STDIO_H

#include <os/stdargs.h>

int vsprintf(char* buf,const char* fmt,va_list args);
int sprintf(char* buf,const char* fmt);

#endif //STDIO_H
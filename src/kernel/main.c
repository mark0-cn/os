#include "os/os.h"

char massage[]="hello this is os !!!";

void kernel_init()
{
    char* screen=(char*) 0xb8000;
    for(int i=0;i<sizeof(massage);i++)
    {
        screen[2*i]=massage[i];
    }
}
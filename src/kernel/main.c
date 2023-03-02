#include "os/os.h"
#include "os/console.h"

char massage[]="hello this is os !!!";

void kernel_init()
{
    console_init();
}
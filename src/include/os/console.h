#ifndef CONSOLE_H
#define CONSOLE_H

#include <os/types.h>

void console_init();
void console_clear();
void console_write(char* buf,u32 count);

void get_cursor();
void set_cursor();

void get_screen();
void set_screen();

#endif
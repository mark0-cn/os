#include <os/string.h>

char* strcpy(char* dest,const char* src);
char* strcat(char* dest,const char* src)
{
    char* ptr=dest;
    while(*ptr!=EOS){
        ptr++;
    }

    while(true)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
        {
            return dest;
        }
    }
}

size_t strlen(const char* str)
{
    char* ptr=(char*)str;
    size_t count=0;
    while(*ptr++!=EOS)
    {
        count++;
    }
    return count;
}

int strcmp(const char* lhs,const char* rhs);
char* strchr(const char* str,int ch);
char* strrchr(const char* str,int ch);

int memcmp(const void* lhs,const void* rhs,size_t count);

void* memset(void* dest,int ch,size_t count)
{
    char* ptr=dest;
    while(count--)
    {
        *ptr++=ch;
    }
}

void* memcpy(void* dest,const void* src,size_t count)
{
    char* ptr=dest;
    while (count--)
    {
        *ptr++=*(char*)src++;
    }
    
}
void* memchr(const void* ptr,int ch,size_t count);
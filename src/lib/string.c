#include <os/string.h>

char* strcpy(char* dest,const char* src)
{
    char* ptr=dest;
    while(*src!=EOS)
    {
        *ptr++=*src++;
    }
    *ptr=EOS;
    return dest;
}

char* strcat(char* dest,const char* src)
{
    char* ptr=dest;
    while(*ptr!=EOS){
        ptr++;
    }

    while(*src!=EOS)
    {
        *ptr++=*src++;
    }
    *ptr=EOS;
    return dest;
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

int strcmp(const char* lhs,const char* rhs)
{
    while(*lhs!=EOS && *rhs!=EOS && *lhs==*rhs)
    {
        lhs++;
        rhs++;
    }
    return *lhs<*rhs?-1:*lhs>*rhs;
}

char* strchr(const char* str,int ch)
{
    char* ptr=str;
    while(*ptr!=EOS)
    {
        ptr++;
        if(*ptr==ch)
        {
            return *ptr;
        }
    }
    return NULL;
}

char* strrchr(const char* str,int ch)
{
    char* ptr=str;
    long count=0;

    while(*ptr!=EOS)
    {
        count++;
        ptr++;
    }

    while(count--)
    {
        if(*ptr==ch)
        {
            return ptr;
        }
        ptr--;
    }
    return NULL;
}

int memcmp(const void* lhs,const void* rhs,size_t count)
{
    char *lptr = (char *)lhs;
    char *rptr = (char *)rhs;
    while ((count > 0) && *lptr == *rptr)
    {
        lptr++;
        rptr++;
        count--;
    }
    if (count == 0)
        return 0;
    return *lptr < *rptr ? -1 : *lptr > *rptr;
}

void* memset(void* dest,int ch,size_t count)
{
    char* ptr=dest;
    while(count--)
    {
        *ptr++=ch;
    }
    return dest;
}

void* memcpy(void* dest,const void* src,size_t count)
{
    char* ptr=dest;
    while (count--)
    {
        *ptr++=*(char*)src++;
    }
    return dest;
    
}

void* memchr(const void* mem,int ch,size_t count)
{
    char* ptr=(char*)mem;

    while(count--)
    {
        if(*ptr==ch)
        {
            return ptr;
        }
        ptr++;
    }
    return NULL;
}
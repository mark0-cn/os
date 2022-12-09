#include <os/io.h>
#include <os/string.h>
#include <os/console.h>

#define CRT_ADDR_REG 0x3D4 // CRT(6845)索引寄存器
#define CRT_DATA_REG 0x3D5 // CRT(6845)数据寄存器

#define CRT_START_ADDR_H 0xC // 显示内存起始位置 - 高位
#define CRT_START_ADDR_L 0xD // 显示内存起始位置 - 低位
#define CRT_CURSOR_H 0xE     // 光标位置 - 高位
#define CRT_CURSOR_L 0xF     // 光标位置 - 低位

#define MEM_BASE 0xB8000              // 显卡内存起始位置
#define MEM_SIZE 0x4000               // 显卡内存大小
#define MEM_END (MEM_BASE + MEM_SIZE) // 显卡内存结束位置
#define WIDTH 80                      // 屏幕文本列数
#define HEIGHT 25                     // 屏幕文本行数
#define ROW_SIZE (WIDTH * 2)          // 每行字节数
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // 屏幕字节数

#define ASCII_NUL 0x00
#define ASCII_ENQ 0x05
#define ASCII_BEL 0x07 // \a
#define ASCII_BS 0x08  // \b
#define ASCII_HT 0x09  // \t
#define ASCII_LF 0x0A  // \n
#define ASCII_VT 0x0B  // \v
#define ASCII_FF 0x0C  // \f
#define ASCII_CR 0x0D  // \r
#define ASCII_DEL 0x7F

static u32 screen; // 当前显示器开始的内存位置

static u32 pos; // 记录当前光标的内存位置

static u32 x, y; // 当前光标的坐标

static u8 attr = 7;        // 字符样式
static u16 erase = 0x0720; // 空格

void get_screen()
{
    outb(CRT_ADDR_REG,CRT_START_ADDR_H);
    screen=inb(CRT_DATA_REG)<<8;
    outb(CRT_ADDR_REG,CRT_START_ADDR_L);
    screen|=inb(CRT_DATA_REG);

    screen<<=1;
    screen+=MEM_BASE;
}

void get_cursor()
{
    outb(CRT_ADDR_REG,CRT_CURSOR_H);
    pos=inb(CRT_DATA_REG)<<8;
    outb(CRT_ADDR_REG,CRT_CURSOR_L);
    pos|=inb(CRT_DATA_REG);

    get_screen();

    pos<<=1;
    pos+=MEM_BASE;

    u32 data=(pos-screen)>>1;
    x=data/WIDTH;
    y=data%WIDTH;
    return;
}

void set_cursor()
{
    outb(CRT_ADDR_REG,CRT_CURSOR_H);
    outb(CRT_DATA_REG,(pos-MEM_BASE)>>9&0xff);
    outb(CRT_ADDR_REG,CRT_CURSOR_L);
    outb(CRT_DATA_REG,(pos-MEM_BASE)>>1&0xff);
}

void set_screen()
{
    outb(CRT_ADDR_REG,CRT_START_ADDR_H);
    outb(CRT_DATA_REG,(screen-MEM_BASE)>>9&0xff);
    outb(CRT_ADDR_REG,CRT_START_ADDR_L);
    outb(CRT_DATA_REG,(screen-MEM_BASE)>>1&0xff);
}

// 向上滚屏
static void scroll_up()
{
    if (screen + SCR_SIZE + ROW_SIZE >= MEM_END)
    {
        memcpy((void *)MEM_BASE, (void *)screen, SCR_SIZE);
        pos -= (screen - MEM_BASE);
        screen = MEM_BASE;
    }

    u32 *ptr = (u32 *)(screen + SCR_SIZE);
    for (size_t i = 0; i < WIDTH; i++)
    {
        *ptr++ = erase;
    }
    screen += ROW_SIZE;
    pos += ROW_SIZE;
    set_screen();
}

//光标向下移动一个单位
static void commond_lf()
{
    if(y+1<HEIGHT)
    {
        y++;
        pos+=ROW_SIZE;
        return;
    }
    scroll_up();
}

//光标移动到最左边
static void commond_cr()
{
    pos-=(x<<1);
    x=0;
    return;
}

//光标回退并删除当前一个字符
static void commond_bs()
{
    if(x)
    {
        x--;
        pos-=2;
        *(u32*)pos=erase;
    }
}

//删除当前字符
static void commond_del()
{
    *(u32*)pos=erase;
}

void console_init()
{
    console_clear();
}

void console_clear()
{
    u16* ptr;

    x=0;
    y=0;
    pos=MEM_BASE;
    screen=MEM_BASE;

    set_cursor(); //设置光标
    set_screen(); //设置显示器开始位置

    ptr=(u16*)MEM_BASE;
    while(ptr<(u16*)MEM_END)
    {
        *ptr++=erase;
    }
}

void console_write(char* buf,u32 count)
{
    char ch=*buf;
    char* ptr=buf;
    while (count--)
    {
        ch=*ptr++;
        switch (ch)
        {
        case ASCII_NUL:
            break;
        case ASCII_ENQ:
            break;
        case ASCII_BEL:     // \a
            break;
        case ASCII_BS:      // \b
            commond_bs();
            break;
        case ASCII_HT:      // \t
            break;
        case ASCII_LF:      // \n
            commond_lf();
            commond_cr();
            break;
        case ASCII_VT:      // \v
            break;
        case ASCII_FF:      // \f
            break;
        case ASCII_CR:      // \r
            commond_cr();
            break;
        case ASCII_DEL:
            commond_del();
            break;
        default:
            *(char*)pos=ch;
            pos++;
            *(char*)pos=attr;
            pos++;

            x++;
            break;
        }
    }
    set_cursor();
}
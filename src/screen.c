#include <stddef.h>
#include <stdint.h>
#include "mailbox.h"
#include "util.h"
#include "screen.h"
#include "font.h"

volatile unsigned int __attribute__((aligned(16))) mbox[10];

//Initialize a framebuffer and return its adress
void* framebuffer_init(uint32_t width,uint32_t height,uint32_t depth)
{
    mbox[0]=width;
    mbox[1]=height;
    mbox[2]=width;
    mbox[3]=height;
    mbox[4]=0;
    mbox[5]=depth;
    mbox[6]=0;
    mbox[7]=0;
    mbox[8]=0;
    mbox[9]=0;
    uint32_t r=(uint32_t)&mbox;
    uint32_t rep;
    mailbox_write(1,r);
    rep=mailbox_read(1);
    if(rep!=0)
    {
        uart_puts("Failed to initialize framebuffer.\r\n");
    }
    return (uint32_t*)mbox[8];
}

//Display a character in a 16-bit depth framebuffer
void disp_char(int x,int y,int width,int height,char c,uint16_t* fb)
{
    (void)height;
    for(int iy=0;iy<16;iy++)
    {
        for(int ix=0;ix<8;ix++)
        {
            switch((font[16*c+iy]>>(7-ix))%2)
            {
                case 0:
                    fb[(x+ix)+(y+iy)*width]=0x0000;
                    break;
                case 1:
                    fb[(x+ix)+(y+iy)*width]=0xFFFF;
                    break;
            }
        }
    }
}

//Display a character of the given size in a 16-bit depth framebuffer
void disp_large_char(int x,int y,int size,int width,int height,char c,uint16_t* fb)
{
    (void)height;
    for(int iy=0;iy<16;iy++)
    {
        for(int ix=0;ix<8;ix++)
        {
            uint16_t color;
            switch((font[16*c+iy]>>(7-ix))%2)
            {
                case 0:
                    color=0x0000;
                    break;
                case 1:
                    color=0xFC00;
                    break;
            }
            for(int sy=0;sy<size;sy++)
            {
                for(int sx=0;sx<size;sx++)
                {
                    fb[(x+sx+size*ix)+width*(y+sy+size*iy)]=color;
                }
            }
        }
    }
}

//Display a string in a 16-bit depth framebuffer
void disp_string(int x,int y,int width,int height,const char* str,uint16_t* fb)
{
    for(int i=0;str[i]!='\0';i++)
    {
        disp_char(x,y,width,height,str[i],fb);
        x+=8;
        if(x>=width-8)
        {
            x=0;
            y+=16;
        }
    }
}

//Display a string of the given size in a 16-bit depth framebuffer
void disp_large_string(int x,int y,int size,int width,int height,const char* str,uint16_t* fb)
{
    for(int i=0;str[i]!='\0';i++)
    {
        disp_large_char(x,y,size,width,height,str[i],fb);
        x+=8*size;
        if(x>=width-8*size)
        {
            x=0;
            y+=16*size;
        }
    }
}


void clear_screen(int width,int height,uint16_t* fb)
{
    for(int iy=0;iy<height;iy++)
    {
        for(int ix=0;ix<width;ix++)
        {
            fb[ix+width*iy]=0x0000;
        }
    }
}

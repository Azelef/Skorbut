#include <stddef.h>
#include <stdint.h>
#include "mailbox.h"
#include "util.h"
#include "screen.h"
#include "shell.h"
#include "vitamin.h"
#include "mem.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    //mem_init((atag_t*)atags);
    uart_init();
    int width=1024,height=600,depth=16;
    uint16_t* fb=framebuffer_init(width,height,depth);
    disp_large_string(350,250,5,width,height,"SKORBUT",fb);
    disp_logo(650,258,width,height,fb);
    shell(width,height,fb);
}

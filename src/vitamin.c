#include <stddef.h>
#include <stdint.h>
#include "vitamin.h"
#include "orange.h"

//Display the Skorbut logo in a 16-bit depth framebuffer
void disp_logo(int x,int y,int width,int height,uint16_t* fb)
{
    (void)height;
    for(int iy=0;iy<64;iy++)
    {
        for(int ix=0;ix<64;ix++)
        {
            fb[(x+ix)+width*(y+iy)]=logo[64*iy+ix];
        }
    }
}

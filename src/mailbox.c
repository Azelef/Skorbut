#include <stddef.h>
#include <stdint.h>
#include "mailbox.h"
#include "util.h"

//Write data to mailbox #box
void mailbox_write(uint32_t box,uint32_t data)
{
    if((data&0b1111)!=0)
    {
        uart_hex(data);
        uart_puts("Data should end with 4 zeroes");
    }
    if(box>15)
    {
        uart_puts("Mailbox ID should be at most 15");
    }
    uint32_t status;
    do
    {
        status=mmio_read(MAILBOX_BASE+0x18);
    }
    while((status&1<<31)!=0);
    mmio_write(MAILBOX_BASE+0x20,data|box);
}

//Returns the content of mailbox #box
uint32_t mailbox_read(uint32_t box)
{
    if(box>15)
    {
        uart_puts("Mailbox ID should be at most 15");
    }
    uint32_t status,mail;
    do
    {
        do
        {
            status=mmio_read(MAILBOX_BASE+0x18);
        }
        while((status&1<<30)!=0);
        mail=mmio_read(MAILBOX_BASE);
    }
    while((mail&0xF)!=box);
    return mail&0xFFFFFFF0;
}

#include <stddef.h>
#include <stdint.h>
#include "util.h"

//Computes the max of two integers
int max(int a,int b)
{
    if(a>b)
        return a;
    else
        return b;
}

//Computes the min of two integers
int min(int a,int b)
{
    if(a<b)
        return a;
    else
        return b;
}

//Start the UART
void uart_init()
{
    mmio_write(UART0_CR, 0x00000000);

    mmio_write(GPPUD, 0x00000000);
    delay(150);

    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    mmio_write(GPPUDCLK0, 0x00000000);

    mmio_write(UART0_ICR, 0x7FF);

    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

//Write a character using the UART
void uart_putc(unsigned char c)
{
    while ( mmio_read(UART0_FR) & (1 << 5) ) { }
    mmio_write(UART0_DR, c);
}

//Read a character using the UART
unsigned char uart_getc()
{
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}

//Write a string to the UART
void uart_puts(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char)str[i]);
}

//Write a 32 bit int to the UART (in hex format)
void uart_hex(uint32_t i)
{
    for(int k=28;k>=0;k-=4)
    {
        if((i>>k)%16<10)
        {
            uart_putc((unsigned char)((i>>k)%16)+'0');
        }
        else
        {
            uart_putc((unsigned char)((i>>k)%16)+'7');
        }
    }
    uart_puts("\r\n");
}

#include <msp430.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/fileio.h>

#define READ_BYTE(b)                        \
    do {                                    \
        /* Always block wait for data */    \
        while(!fileio_available());         \
        b = fileio_getc();                  \
    }                                       \
    while(0)

#define WRITE_BYTE(b)                       \
    fileio_putc(b)

int main()
{
    char c = 0;

    msp430_io_init();

    while (c != '#')
    {
        READ_BYTE(c);
        int trans = toupper(c);
        pr_info3("received %#x '%c'; transformed= %#x\n", c, c, trans);
        WRITE_BYTE(trans);
    }

    EXIT();
}

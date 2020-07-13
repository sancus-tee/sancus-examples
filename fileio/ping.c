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

char *data = "Hello, world! #";

int main()
{
    char c;
    int i;

    msp430_io_init();

    for (i = 0; i < strlen(data); i++)
    {
        WRITE_BYTE(data[i]);
        READ_BYTE(c);
        pr_info3("received %#x '%c'; sent=%#x\n", c, c, data[i]);
        ASSERT( c == toupper(data[i]));
    }

    EXIT();
}

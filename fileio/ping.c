#include <msp430.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/fileio.h>

char *data = "Hello, world! #";

int main()
{
    char c;
    int i;

    msp430_io_init();

    for (i = 0; i < strlen(data); i++)
    {
        fileio_putc(data[i]);
        while ((c = fileio_getc()) == EOF);
        pr_info3("received %#x '%c'; sent=%#x\n", c, c, data[i]);
        ASSERT( c != EOF);
        ASSERT( c == toupper(data[i]));
    }

    EXIT();
}

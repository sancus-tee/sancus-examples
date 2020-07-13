#include <msp430.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/fileio.h>

int main()
{
    char c = 0;

    msp430_io_init();

    while (c != '#')
    {
        c = fileio_getc();
        if (c != EOF)
        {
            int trans = toupper(c);
            pr_info3("received %#x '%c'; transformed= %#x\n", c, c, trans);
            fileio_putc(trans);
        }
    }

    EXIT();
}

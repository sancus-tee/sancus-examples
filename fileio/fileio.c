#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/fileio.h>

int main()
{
    unsigned char c;
    char *s = TEST_DATA;
    int i;

    msp430_io_init();

    for (i = 0; i < strlen(s); i++)
    {
        if (fileio_available())
        {
            fileio_putc((c = fileio_getc()));
            pr_info1("dumped '%#1x' on fileio_out\n", c);
            ASSERT(c == s[i]);
        }
        else
            ASSERT(0);
    }

    EXIT();
}

#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>

#include "hello.h"


int main()
{
    WDTCTL = WDTPW | WDTHOLD;

    puts("main() started");

    sancus_enable(&hello);
    hello_entry_fn();

    puts("\nmain() done");
    return 0;
}

int putchar(int c)
{
    P1OUT = c;
    P1OUT |= 0x80;
    return c;
}


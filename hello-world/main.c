#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>

#ifndef __SANCUS_SIM
    #include <sancus_support/uart.h>
#endif

void printf2_wrapper(char *s, int d1, int d2)
{
    printf(s, d1, d2);
}

DECLARE_SM(hello, 0x1234);

static void SM_FUNC(hello) hello_internal_fn(sm_id id)
{
    printf2_wrapper("Hi from a Sancus module with ID %d, called by %d\n",
        sancus_get_self_id(), id);
}

void SM_ENTRY(hello) hello_entry_fn(void)
{
    sm_id caller_id = sancus_get_caller_id();
    hello_internal_fn(caller_id);
}

int main()
{
    WDTCTL = WDTPW | WDTHOLD;
    puts("\n[main] enabling hello SM..");

    sancus_enable(&hello);
    hello_entry_fn();

    puts("\n[main] all done!");
    return 0;
}

int putchar(int c)
{
    #if __SANCUS_SIM
        P1OUT = c;
        P1OUT |= 0x80;
    #else
        if (c == '\n')
            putchar('\r');

        uart_write_byte(c);
    #endif

    return c;
}

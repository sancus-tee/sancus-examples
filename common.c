#include "common.h"
#include <sancus_support/uart.h>
#include <msp430.h>

void msp430_init(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    uart_init();
    printf("\n------\n\n");
}

int putchar(int c)
{
    if (c == '\n')
        putchar('\r');

    uart_write_byte(c);

    return c;
}

void do_sancus_enable(struct SancusModule *sm)
{
    ASSERT(sancus_enable(sm));
    ASSERT(sm->id == sancus_get_id(sm->public_start));

    printf("SM %s with ID %d enabled\t: 0x%.4x 0x%.4x 0x%.4x 0x%.4x\n",
        sm->name, sm->id,
        (uintptr_t) sm->public_start, (uintptr_t) sm->public_end,
        (uintptr_t) sm->secret_start, (uintptr_t) sm->secret_end);
}

#ifndef QUIET
    void __attribute__((noinline)) printf0(const char* str)
    {
        printf("%s", str);
    }

    void __attribute__((noinline)) printf1(const char* fmt, int arg1)
    {
        printf(fmt, arg1);
    }

    void __attribute__((noinline)) printf2(const char* fmt, int arg1, int arg2)
    {
        printf(fmt, arg1, arg2);
    }

    void __attribute__((noinline)) printf3(const char* fmt, int arg1, int arg2, int arg3)
    {
        printf(fmt, arg1, arg2, arg3);
    }
#endif

void stop_violation(void)
{
    printf("\t--> SM VIOLATION DETECTED; exiting...\n");
    while(1);
}

__attribute__ ((naked)) __attribute__((interrupt(SM_VECTOR))) void violation_isr(void)
{
    asm(    "mov &__unprotected_sp, r1                  \n\t"   \
            "call #stop_violation                       \n\t"   \
       );
}

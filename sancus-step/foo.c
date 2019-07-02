#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

#define MAX_COUNTER (0xf)

#if __clang_major__ >= 5
asm(".section __interrupt_vector_9,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry                         \n\t");
#endif


int counter = 0x0;
int oldCounterValue = 0x0;

/*
 * Foo module
 */

DECLARE_SM(foo, 0x1234);

void SM_ENTRY(foo) foo_enter(void)
{
    __asm__ __volatile__(
        "mov #0x0, &counter\n\t"
        "mov #0x1, &counter\n\t"
        "mov #0x2, &counter\n\t"
        "mov #0x3, &counter\n\t"
        "mov #0x4, &counter\n\t"
        "mov #0x5, &counter\n\t"
        "mov #0x6, &counter\n\t"
        "mov #0x7, &counter\n\t"
        "mov #0x8, &counter\n\t"
        "mov #0x9, &counter\n\t"
        "mov #0xa, &counter\n\t"
        "mov #0xb, &counter\n\t"
        "mov #0xc, &counter\n\t"
        "mov #0xd, &counter\n\t"
        "mov #0xe, &counter\n\t"
        "mov #0xf, &counter\n\t"
        :::);
}


/*
 * Untrusted context
 */

void checkCounter(void)
{
    __ss_print_latency();
    if (counter != 0)
    {
        if (!(counter == oldCounterValue || counter == oldCounterValue + 1))
            pr_info("error: not single stepping\n");
        if (counter == oldCounterValue && counter != 0 && counter != MAX_COUNTER)
            pr_info("warning: zero-stepping\n");
        oldCounterValue = counter;
    }
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    __ss_start();
    foo_enter();
    if (counter != MAX_COUNTER)
        pr_info2("error: invalid counter: %d - expected: %d\n", counter, MAX_COUNTER);
    
    pr_info("exiting...");
    EXIT();
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY2(checkCounter, __ss_end)

#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>
#include <sancus_support/sancus_step.h>


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
    if (counter != 0)
    {
        if (!(counter == oldCounterValue || counter == oldCounterValue + 1))
        {
            printf("ERROR: not single stepping\n");
        }
        oldCounterValue = counter;
    }
    
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    sancus_step_start();
    foo_enter();
    sancus_step_end();
    
    pr_info("exiting...");
    EXIT();
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY(checkCounter)




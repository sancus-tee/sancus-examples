#include <msp430.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

DECLARE_SM(foo, 0x1234);

// The secure module has one function that behaves differently depending on
// whether the secret value 0x42 is guessed or not.

// This can be observed by printing the latency after every interrupt, and
// comparing the traces.

void SM_ENTRY(foo) test(char key) {
    char * p = &key;
    __asm__ __volatile__(
        "mov %0, r6\n\t"
        "mov #0x42, r7\n\t"
        "cmp.b @r6+, r7\n\t"
        "jz 1f\n\t"
        "bis #0x1, r7\n\t"
        "jmp 2f\n\t"
        "1: nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "2: nop\n\t"
        :
        :"m"(p)
        :"r6", "r7"
    );
}

void irqHandler(void)
{
    __ss_print_latency();
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    __ss_start();
    test(0x41);

    __ss_start();
    test(0x42);

    EXIT();
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY2(irqHandler, __ss_end)

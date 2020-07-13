#include <msp430.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

#define MAX_COUNTER (0xf)

#if __clang_major__ >= 5
asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");
#endif


int counter = 0x0;
int oldCounterValue = 0x0;
int instrAfterCtr = 0;

DECLARE_SM(foo, 0x1234);

// The secure module has one function that behaves differently depending on
// whether the secret value 0x42 is guessed or not.

// This can be observed by printing the latency after every interrupt, and
// comparing the traces.

void SM_ENTRY(foo) test(char key) {
    char * p = &key;
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
    int delay = __ss_get_latency();
    pr_info1("Latency: %d\n", delay);
    if (counter != 0) {
        if (!(counter == oldCounterValue || counter == oldCounterValue + 1)) {
            pr_info("error: not single stepping\n");
        }
        if (counter == oldCounterValue && counter != 0 && counter != MAX_COUNTER) {
            pr_info("error: zero-stepping\n");
        }
        oldCounterValue = counter;
        if (counter == MAX_COUNTER) {
            if (instrAfterCtr == 6) {
                if (delay == 1) {
                    pr_info("Key was guessed!");
                } else {
                    pr_info("Key was not guessed!");
                }
            }
            ++instrAfterCtr;
        }
    }
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    __ss_start();
    test(0x41);
    if (counter != MAX_COUNTER) {
        pr_info2("error: invalid counter: %d - expected: %d\n", counter, MAX_COUNTER);
    }

    counter = 0;
    oldCounterValue = 0;
    instrAfterCtr = 0;

    __ss_start();
    test(0x42);
    if (counter != MAX_COUNTER) {
        pr_info2("error: invalid counter: %d - expected: %d\n", counter, MAX_COUNTER);
    }

    EXIT();
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY2(irqHandler, __ss_end)

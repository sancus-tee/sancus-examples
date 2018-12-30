#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>


volatile char c = '0';
volatile int timer_latency;

/*
 * Foo module
 */

DECLARE_SM(foo, 0x1234);

void SM_ENTRY(foo) foo_enter()
{
    while (c == '0');
    pr_info("Hello from Foo");
}


/*
 * Untrusted context
 */


void timerA_isr(void)
{
    timer_disable();
    pr_info1("Hi from Timer_A ISR! (latency=%d cycles IRQ to ISR)\n",
                timer_latency);
    if (c == '0')
        c = '1';
    else
        c = '0';
}

int main()
{
    int tsc1, tsc2;
    msp430_io_init();
    asm("eint\n\t");
    
    sancus_enable(&foo);

    /* First measure TSC function overhead */
    timer_tsc_start();
    tsc1 = timer_tsc_end();

    /* Now measure the operation of interest */
    timer_tsc_start();
    asm("nop\n\t"
        "nop\n\t");
    tsc2 = timer_tsc_end();
    pr_info1("dummy operation took %d cycles\n", tsc2-tsc1);
    
    pr_info("waiting for interrupting Foo...");
    timer_irq(150);
    foo_enter();
    
    pr_info("waiting for normal interrupt...");
    timer_irq(20);
    while (c == '1');

    pr_info("exiting...");
    EXIT();
}

#if __clang_major__ < 5
__attribute__((interrupt(TIMER_IRQ_VECTOR)))
#else
  /* TODO: Modern LLVM/Clang generates an interrupt specification
   *       which is not compatible with modern mspgcc .
   */
asm(".section __interrupt_vector_9,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry                         \n\t");
#endif

/*
 * NOTE: we use a naked asm function here to be able to store IRQ latency.
 * (Timer_A continues counting from zero after IRQ generation)
 */
__attribute__((naked))
void timerA_isr_entry(void)
{
#if __clang_major__ < 5
    asm("mov &%0, &timer_latency        \n\t"
#else
    /* LLVM bug: See https://bugs.llvm.org/show_bug.cgi?id=39993 */
    asm("mov &0x0170, &timer_latency    \n\t"
#endif
        "cmp #0x0, r1                   \n\t"
        "jne no_sm                      \n\t"
        "; isr interrupted an sm        \n\t"
        "mov &__unprotected_sp, r1      \n\t"
        "; push #0x1 here               \n\t"
        "; to remember how to return    \n\t"
        "push #0x1                      \n\t"
        "jmp cont                       \n\t"
        "no_sm:                         \n\t"
        "; same as the other case       \n\t"
        "push #0x0                      \n\t"
        "cont:                          \n\t"
        "push r15                       \n\t"
        "push r14                       \n\t"
        "push r13                       \n\t"
        "push r12                       \n\t"
        "push r11                       \n\t"
        "push r10                       \n\t"
        "push r9                        \n\t"
        "push r8                        \n\t"
        "push r7                        \n\t"
        "push r6                        \n\t"
        "push r5                        \n\t"
        "push r4                        \n\t"
        "call #timerA_isr               \n\t"
        "pop  r4                        \n\t"
        "pop  r5                        \n\t"
        "pop  r6                        \n\t"
        "pop  r7                        \n\t"
        "pop  r8                        \n\t"
        "pop  r9                        \n\t"
        "pop  r10                       \n\t"
        "pop  r11                       \n\t"
        "pop  r12                       \n\t"
        "pop  r13                       \n\t"
        "pop  r14                       \n\t"
        "pop  r15                       \n\t"
        "; we free here so we can do it \n\t"
        "; just once                    \n\t"
        "add #0x2, r1                   \n\t"
        "cmp #0x0, -2(r1)               \n\t"
        "jeq 1f                         \n\t"
        "; resume interrupted sm        \n\t"
        "br r15                         \n\t"
        "1:                             \n\t"
        "; return normally              \n\t"
        "reti                           \n\t"
        ::"m"(TAR):);
}

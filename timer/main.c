#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>

volatile char c = '0';
volatile int timer_latency;

void timerA_isr(void)
{
    timer_disable();
    pr_info1("Hi from Timer_A ISR! (latency=%d cycles IRQ to ISR)\n",
                timer_latency);
    c = '1';
}

int main()
{
    int tsc1, tsc2;
    msp430_io_init();
    asm("eint\n\t");

    /* First measure TSC function overhead */
    timer_tsc_start();
    tsc1 = timer_tsc_end();

    /* Now measure the operation of interest */
    timer_tsc_start();
    asm("nop\n\t"
        "nop\n\t");
    tsc2 = timer_tsc_end();
    pr_info1("dummy operation took %d cycles\n", tsc2-tsc1);
    
    pr_info("waiting for IRQ...");
    timer_irq(10);
    while (c == '0');

    pr_info("exiting...");
    EXIT();
}

/*
 * NOTE: we use a naked asm function here to be able to store IRQ latency.
 * (Timer_A continues counting from zero after IRQ generation)
 */
__attribute__((naked)) __attribute__((interrupt(TIMER_IRQ_VECTOR)))
void timerA_isr_entry(void)
{
    asm("mov &%0, &timer_latency    \n\t"   \
        "push r15                   \n\t"
        "push r14                   \n\t"
        "push r13                   \n\t"
        "push r12                   \n\t"
        "push r11                   \n\t"
        "push r10                   \n\t"
        "push r9                    \n\t"
        "push r8                    \n\t"
        "push r7                    \n\t"
        "push r6                    \n\t"
        "push r5                    \n\t"
        "push r4                    \n\t"
        "call #timerA_isr           \n\t"
        "pop  r4                    \n\t"
        "pop  r5                    \n\t"
        "pop  r6                    \n\t"
        "pop  r7                    \n\t"
        "pop  r8                    \n\t"
        "pop  r9                    \n\t"
        "pop  r10                   \n\t"
        "pop  r11                   \n\t"
        "pop  r12                   \n\t"
        "pop  r13                   \n\t"
        "pop  r14                   \n\t"
        "pop  r15                   \n\t"
        "reti                       \n\t"
        ::"m"(TAR):);
}

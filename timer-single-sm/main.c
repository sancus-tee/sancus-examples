#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>

volatile char c = '0';
volatile int timer_latency;
void* SM_DATA(foo) __isr_sp = (void*) &__isr_stack[ISR_STACK_SIZE-1];
   
/*
 * Foo module
 */

DECLARE_SM(foo, 0x1234);

void SM_ENTRY(foo) foo_exit(void)
{
    /* NOTE: only SM 1 can exit on Aion */
    FINISH();
}

void SM_ENTRY(foo) foo_enter()
{
    pr_info("Hello from Foo");
}

void SM_ENTRY(foo) secure_timer_start(void)
{
    TACTL = TACTL_DISABLE;
    TACCR0 = 0;
    TACCTL0 = TACCTL_ENABLE_CONT;
    TACTL = TACTL_CONTINUOUS;
}

int SM_ENTRY(foo) secure_timer_end(void)
{
    return TAR;
}

void SM_FUNC(foo) secure_timer_disable(void)
{
    TACTL = TACTL_DISABLE;
}


void SM_FUNC(foo) timerA_isr(void)
{
    secure_timer_disable();
    pr_info1("Hi from Timer_A ISR!\n",
                timer_latency);
    if (c == '0')
        c = '1';
}

void SM_FUNC(foo) __attribute__((naked, used)) __sm_foo_isr_func(unsigned __attribute__ ((unused)) num_name){
                                                           
    __asm__ __volatile__(                                                                     
            "call #timerA_isr\n\t"                                                                              
            "ret\n\t"                                              
            :::);       
}              
/* ======== TIMER A ISR ======== */
SM_HANDLE_IRQ(foo, 9);
SM_HANDLE_IRQ(foo, 8);

/*
 * Untrusted context
 */
int main()
{   
    pr_info("Secure timer example. Entering main..\n");
    int tsc1, tsc2;
    msp430_io_init();
    asm("eint\n\t");
    
    sancus_enable(&foo);
    pr_info("SM Foo enabled.\n");

    /* First measure timer function overhead */
    pr_info("Measuring timer function overhead...\n");
    secure_timer_start();
    tsc1 = secure_timer_end();
    pr_info1("Mere ISR took %d cycles\n", tsc1);

    /* Now measure the operation of interest */
    secure_timer_start();
    asm("nop\n\t"
        "nop\n\t");
    tsc2 = secure_timer_end();
    pr_info1("dummy operation took %d cycles\n", tsc2-tsc1);
    
    pr_info("waiting for ISR...");
    while (c == '0');
    foo_enter();
    pr_info("foo done...");
    
    pr_info("Creating violation by directly accessing timer A...\n");
    int timer_violation = TAR;
    pr_info1("This should not be possible: %d", timer_violation);
    // jo: @fritz what do you mean here? Use an ASSERT if possible?

    foo_exit();
    ASSERT(0 && "should never reach here");
}



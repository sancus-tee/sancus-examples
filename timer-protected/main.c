#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>

volatile char c = '0';
void* SM_DATA(foo) __isr_sp = (void*) &__isr_stack[ISR_STACK_SIZE-1];
   
/*
 * Foo module
 */

DECLARE_SM(foo, 0x1234);
DECLARE_SM(violations, 0x1234);

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


/* ======== TIMER A ISR + assembly stub + registration ======== */
void SM_FUNC(foo) timerA_isr(void)
{
    secure_timer_disable();
    pr_info("Hi from Timer_A ISR!");
    if (c == '0')
        c = '1';
}

void SM_FUNC(foo) __attribute__((naked, used)) __sm_foo_isr_func(unsigned __attribute__ ((unused)) num_name){
                                                           
    __asm__ __volatile__(                                                                     
            "call #timerA_isr\n\t"                                                                              
            "ret\n\t"                                              
            :::);       
}              
SM_HANDLE_IRQ(foo, 9);
SM_HANDLE_IRQ(foo, 8);

/* ======== Violation handler ======== */
void SM_FUNC(violations) handle_violation(void){
    // Print an info and shut down device.
    pr_info("[Violation handler]: Violation occurred. Shutting down!");
    // Exit will not work since first has SM ID 2 (which is not privileged to modify CPUOFF)
    // --> call foo_exit
    foo_exit();
}

void SM_FUNC(violations) __attribute__((naked)) __sm_violations_isr_func(void) {
    // This is a naked function, so we need to set up our stack first
    // In this example, we don't care that we overwrite the old stack.
    asm("mov &__isr_stack, r1");

    // Now just call our rich handler function written in C
    __asm__("br %0": : "i"(handle_violation));

}
SM_HANDLE_IRQ(violations, 13);


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
    sancus_enable(&violations);
    pr_info("SM Violation handler enabled.\n");

    /* First measure timer function overhead */
    pr_info("Measuring timer function overhead...");
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
    
    pr_info("Creating violation by directly accessing timer A. We expect to shutdown after that.\n");
    int timer_violation = TAR;
    ASSERT(0 && "should never reach here");
}



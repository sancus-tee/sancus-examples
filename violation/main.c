#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>


DECLARE_SM(first, 0x1234);
// An ISR stack of 256 byte
SM_DATA(first) char __isr_stack[256];

DECLARE_SM(foo, 0x1234);
int SM_DATA(foo) foo_data;

void SM_ENTRY(first) first_init(void)
{
    pr_info("Hello from first SM. I will handle the violation later.");
}

void SM_FUNC(first) handle_violation(void){
    // Print an info and shut down device.
    pr_info("[Violation handler]: Violation occurred. Shutting down!");
    // Exit will work even from Aion since first has SM ID 1 (which is privileged to modify CPUOFF)
    EXIT();
}

void SM_FUNC(first) __attribute__((naked)) __sm_first_isr_func(void) {
    // This is a naked function, so we need to set up our stack first
    // In this example, we don't care that we overwrite the old stack.
    asm("mov &__isr_stack, r1");

    // Now just call our rich handler function written in C
    __asm__("br %0": : "i"(handle_violation));

}


/* ======== HELLO WORLD SM ======== */
void SM_ENTRY(foo) foo_entry(void)
{
    foo_data++;
}

int main()
{
    msp430_io_init();
    
    pr_info("enabling first SM..");
    sancus_enable(&first);
    first_init();
    pr_sm_info(&first);

    pr_info("enabling foo SM..");
    sancus_enable(&foo);
    pr_sm_info(&foo);

    printf("accessing foo private data at %p\n", &foo_data);
    foo_data++;

    ASSERT(0 && "should never reach here");
}
// Register first to handle the Violation IRQ (IRQ 13)
SM_HANDLE_IRQ(first, 13);

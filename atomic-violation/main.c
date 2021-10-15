#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

/* ======== HELLO WORLD SM ======== */

DECLARE_SM(first, 0x1234);
DECLARE_SM(hello, 0x1234);

// An ISR stack of 256 byte
SM_DATA(first) char __isr_stack[256];

void SM_ENTRY(first) first_init(void)
{
    pr_info("Hello from first SM");
    // Do a clix for 3 cycles
    __asm__("mov #3, r15");
    __asm__(".word 0x1389");
    // Do 4 nops
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    pr_info("Normal clix works!");
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

void SM_ENTRY(hello) hello_init(void)
{
    pr_info("Hello from second SM");
    // Do a clix for 3 cycles
    __asm__("mov #3, r15");
    __asm__(".word 0x1389");
    // Do 4 nops
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    pr_info("Normal clix works!");
}

void SM_ENTRY(hello) hello_violation(void)
{
    pr_info("Second SM attempting to create violation.. (handled by violation handler)");
    // Do another clix for 3 cycles
    __asm__("mov #3, r15");
    __asm__(".word 0x1389");

    // Wait just 2 cycles to try following a clix with another one
    __asm__("nop");
    __asm__("nop");

    // Attempt to create violation by an second clix right after the first one expired
    __asm__(".word 0x1389");

}


/* ======== UNTRUSTED CONTEXT ======== */

int main()
{
    msp430_io_init();

    sancus_enable(&first);
    sancus_enable(&hello);
    pr_sm_info(&first);
    pr_sm_info(&hello);

    first_init();
    hello_init();

    pr_info("Creating atomic violation...");
    hello_violation();

    // should never reach here
    pr_info("fail: should never reach here!");
    ASSERT(0);

}
// Register first to handle the Violation IRQ (IRQ 13)
SM_HANDLE_IRQ(first, 13);
#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

void exit_success(void);

/* ======== HELLO WORLD SM ======== */

DECLARE_SM(first, 0x1234);
DECLARE_SM(hello, 0x1234);

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
    pr_info("Second SM attempting to create violation..");
    // Do another clix for 3 cycles
    __asm__("mov #3, r15");
    __asm__(".word 0x1389");

    // Wait just 3 cycles to try following a clix with another one
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    // Attempt to create violation by an second clix right after the first one expired
    __asm__(".word 0x1389");

}


/* ======== UNTRUSTED CONTEXT ======== */

int main()
{
    msp430_io_init();

    sancus_enable_wrapped(&first, SM_GET_WRAP_NONCE(first), SM_GET_WRAP_TAG(first));
    sancus_enable_wrapped(&hello, SM_GET_WRAP_NONCE(hello), SM_GET_WRAP_TAG(hello));
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

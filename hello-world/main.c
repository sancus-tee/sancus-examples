#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

void exit_success(void);

/* ======== HELLO WORLD SM ======== */

DECLARE_SM(hello, 0x1234);

int       SM_DATA(hello) hello_secret;
int const SM_DATA(hello) hello_const = 0xbeef;

void SM_FUNC(hello) hello_init(void)
{
    hello_secret = 0xdead;
}

void SM_ENTRY(hello) hello_greet(void)
{
    hello_init();
    pr_info2("Hi from SM with ID %d, called by %d\n",
        sancus_get_self_id(), sancus_get_caller_id());
}

void SM_ENTRY(hello) hello_disable(void)
{
    sancus_disable(exit_success);
}

/* ======== UNTRUSTED CONTEXT ======== */

int main()
{
    msp430_io_init();
    sancus_enable_info(&hello);

    hello_greet();
    hello_disable();

    // should never reach here
    ASSERT(0);
}

void exit_success(void)
{
    // TODO unprotect instruction should also clear caller ID
    //ASSERT(!sancus_get_caller_id());

    ASSERT(!sancus_get_id(hello_greet));
    ASSERT(!hello_secret);

    // TODO text section does not seem to be properly cleared?
    // (we need a unit test with Verilog stimulus in sancus-core)
    // printf("const is %x\n", hello_const);
    // ASSERT(hello_const == 0x0);

    pr_info("SM disabled; all done!");
    EXIT();
}

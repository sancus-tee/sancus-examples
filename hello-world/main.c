#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include "../common.h"

DECLARE_SM(hello, 0x1234);

int SM_DATA(hello) hello_secret;
const int SM_DATA(hello) hello_const = 0xbeef;

static void SM_FUNC(hello) hello_internal_fn(sm_id id)
{
    pr_info2("Hi from a Sancus module with ID %d, called by %d\n",
        sancus_get_self_id(), id);
}

void SM_ENTRY(hello) hello_entry_fn(void)
{
    sm_id caller_id = sancus_get_caller_id();
    hello_internal_fn(caller_id);
}

void exit_success(void)
{
    // TODO unprotect instruction should also clear caller ID
    //ASSERT(!sancus_get_caller_id());

    ASSERT(!sancus_get_id(hello_entry_fn));
    ASSERT(!hello_secret);

    // TODO text section does not seem to be properly cleared?
    // (we need a unit test with Verilog stimulus in sancus-core)
    // printf("const is %x\n", hello_const);
    // ASSERT(hello_const == 0x0);

    pr_info("SM disabled; all done!");
    EXIT();
}

void SM_ENTRY(hello) hello_disable(void)
{
    hello_secret = 0xffff;
    sancus_disable(exit_success);
}

int main()
{
    msp430_init();

    do_sancus_enable(&hello);

    hello_entry_fn();
    hello_disable();

    // should never reach here
    ASSERT(0);
}

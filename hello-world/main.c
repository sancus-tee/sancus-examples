#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include "../common.h"

DECLARE_SM(hello, 0x1234);

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

int main()
{
    msp430_init();

    pr_info("enabling hello SM..");
    do_sancus_enable(&hello);

    pr_info("entering hello SM..");
    hello_entry_fn();

    pr_info("all done!");
    return 0;
}

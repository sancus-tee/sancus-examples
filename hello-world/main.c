#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

void exit_success(void);

/* ======== HELLO WORLD SM ======== */

DECLARE_SM(hello, 0x1234);

void SM_ENTRY(hello) hello_greet(void)
{
    ASSERT(sancus_get_caller_id() == SM_ID_UNPROTECTED);
    ASSERT(sancus_get_self_id() == 1);
    pr_info2("Hi from SM with ID %d, called by %d\n",
        sancus_get_self_id(), sancus_get_caller_id());
}

void SM_ENTRY(hello) hello_disable(void)
{
    ASSERT(sancus_get_caller_id() == SM_ID_UNPROTECTED);
    ASSERT(sancus_get_self_id() == 1);
    sancus_disable(exit_success);
}

/* ======== UNTRUSTED CONTEXT ======== */
int main()
{
    msp430_io_init();

    sancus_enable(&hello);
    pr_sm_info(&hello);

    ASSERT(sancus_get_caller_id() == SM_ID_IRQ);
    hello_greet();
    ASSERT(sancus_get_caller_id() == 1);
    hello_disable();

    // should never reach here
    ASSERT(0);
}

void exit_success(void)
{
    // TODO unprotect instruction should also clear caller ID
    //ASSERT(!sancus_get_caller_id());
    ASSERT(!sancus_get_id(hello_greet));

    pr_info("SM disabled; all done!\n\n");
    FINISH();
}

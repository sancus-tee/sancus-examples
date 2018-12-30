#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

int SM_DATA(foo) foo_data;

void SM_ENTRY(foo) foo_entry(void)
{
    foo_data++;
}

int main()
{
    msp430_io_init();

    pr_info("enabling foo SM..");
    sancus_enable(&foo);
    pr_sm_info(&foo);

    printf("accessing foo private data at %p\n", &foo_data);
    foo_data++;

    pr_info("fail: should never reach here!");
    EXIT();
}

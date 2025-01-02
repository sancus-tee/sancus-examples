#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

int enter_foo(int arg);

int main()
{
    msp430_io_init();

    pr_info("enabling foo SM..");
    sancus_enable(&foo);
    pr_sm_info(&foo);

    pr_info("entering foo..");
    int null = enter_foo(0);
    int one = enter_foo(1);
    printf("foo(0)=%#x; foo(1)=%#x\n", null, one);

    FINISH();
}

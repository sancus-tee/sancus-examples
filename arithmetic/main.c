#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include "foo.h"
#include "bar.h"

int unpr_mul(int a, int b, int c)
{
    return (a * b) % c;
}

/* Also include a protected foo function here to test arithmetic inlining
   across compilation units. */
int SM_ENTRY(foo) foo_mul(int a, int b, int c)
{
    return (a * b) % c;
}

int main()
{
    int rv, u, f;
    msp430_io_init();

    pr_info("enabling SMs..");
    sancus_enable(&foo);
    pr_sm_info(&foo);
    sancus_enable(&bar);
    pr_sm_info(&bar);

    pr_info("entering foo..");
    rv = enter_foo(128);
    pr_info1("SM foo returned %d\n", rv);
    
    pr_info("comparing unprotected/SM calculations..");
    u = unpr_mul(rv, 100, 35);
    f = foo_mul(rv, 100, 35);
    ASSERT( u==f );

    pr_info("all done!");
    EXIT();
}

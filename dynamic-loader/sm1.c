#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <stdio.h>
#include "global_callback.h"

int SM_ENTRY(sm1) init(int arg1, int arg2)
{
    puts("hello world from sm1!");
    pr_info2("I was called with arg1=%d; arg2=%d\n", arg1, arg2);
    return global_callback_func(arg1, arg2);
}

void SM_ENTRY(sm1) sm1_exit(void)
{
    /* NOTE: only SM 1 can exit on Aion */
    FINISH();
}

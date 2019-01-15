#include "foo.h"
#include "bar.h"
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

int SM_FUNC(foo) foo_div( int i, unsigned int j)
{
    return (i / j);
}

int SM_ENTRY(foo) enter_foo( int i )
{
    int j, k = bar_lookup(i);
    pr_info1("bar returned %d\n", k);

    j = foo_div(k, 5) % i;
    return i * j;
}

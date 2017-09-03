#include "bar.h"
#include <sancus_support/sm_io.h>

DECLARE_SM(bar, 0x1234);

typedef struct {
    int a;
    int b;
    int c;
} element_t;

#define BAR_TABLE_SIZE  3

/* The following data should be stored in the immutable text section. */
const element_t SM_DATA(bar) bar_table[BAR_TABLE_SIZE] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9}
};

int SM_FUNC(bar) bar_ispow2( int i )
{
    while (((i % 2) == 0) && i > 1) i /= 2;
    return (i == 1);
}

int SM_FUNC(bar) bar_in_text( void *p )
{
    return (p >= (void*) &__PS(bar)) && (p < (void*) &__PE(bar));
}

void SM_FUNC(bar) bar_assert(void)
{
    ASSERT( !bar_ispow2(sizeof(element_t)) );
    ASSERT( bar_in_text((void*) bar_table ));
    ASSERT( bar_in_text((void*) bar_table + BAR_TABLE_SIZE - 1) );
}

int SM_ENTRY(bar) bar_lookup( int idx )
{
    bar_assert();

    /* Array indexing below generates muliplication library call that should be
       intercepted and inlined by sancus-ld. */
    return bar_table[idx % BAR_TABLE_SIZE].a;
}


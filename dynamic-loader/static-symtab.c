#include <sancus_support/private/symbol.h>
#include <sancus/sm_support.h>

#include <stdlib.h>
#include <stdio.h>

STATIC_SYMBOLS_START
    SYMBOL(exit),
    SYMBOL(malloc),
    SYMBOL(free),
    SYMBOL(putchar),
    SYMBOL(puts),
    SYMBOL(printf),
    SYMBOL(__unprotected_entry)
STATIC_SYMBOLS_END

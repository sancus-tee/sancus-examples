#include <sancus_support/private/symbol.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <stdlib.h>
#include <stdio.h>
#include "global_callback.h"

/* Include all external undefined symbols below, as output by `nm -u sm1.elf` */
STATIC_SYMBOLS_START
    SYMBOL(exit),
    SYMBOL(global_callback_func),
    SYMBOL(printf2),
    SYMBOL(puts),
    SYMBOL(__unprotected_entry)
STATIC_SYMBOLS_END

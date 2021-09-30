#include <msp430.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sm_control.h>
#include <sancus_support/global_symtab.h>

#include "global_callback.h"
#include "sm1.h"

void check_symbols(void)
{
    size_t current_symbol;
    for (current_symbol = 0; current_symbol < symtab_get_num_symbols(); current_symbol++) {
      Symbol symbol;
      int i, is_section;
      ElfModule* module;

      ASSERT(symtab_get_symbol(current_symbol, &symbol, &is_section, &module));
      printf("\t%s:%d\n", symbol.name, strlen(symbol.name));

      for (i=0; i < strlen(symbol.name); i++)
      {
          ASSERT(isprint(symbol.name[i]));
      }
    }
}

int main()
{
    uint16_t id = 0, retval = 0;
    uint16_t args[] = {100, 200};
    msp430_io_init();

    pr_info("Loading SM from buffer..");
    id = sm_load((void *) SM_BINARY, SM_NAME, VENDOR_ID);
    printf("Loaded SM id: %d\n", id);
    ASSERT(id);

    pr_info("Dumping and validating SM symbol table..");
    check_symbols();

    pr_info("Calling into dynamically loaded SM..");
    sm_call_id(/*id=*/id, /*index=*/0, /*args=*/args, /*nargs=*/2, &retval);
    pr_info1("SM returned %d", retval);
    ASSERT(cb_fired);
    ASSERT(retval == global_callback_func(args[0], args[1]));

    EXIT();
}

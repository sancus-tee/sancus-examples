#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sm_control.h>
#include <sancus_support/global_symtab.h>

#include "sm1.h"

int main()
{
    msp430_io_init();
    printf("Loading SM from buffer..\n");
    int id = sm_load((void *) SM_BINARY, SM_NAME, VENDOR_ID);
    printf("Loaded SM id: %d\n", id);

    size_t current_symbol;
    for (current_symbol = 0; current_symbol < symtab_get_num_symbols(); current_symbol++) {
      Symbol symbol;
      int is_section;
      ElfModule* module;

      if (!symtab_get_symbol(current_symbol,
                             &symbol, &is_section, &module)) {
          printf("Cannot retrieve symbol at index %d\n", current_symbol);
          return 1;
      }

      printf("%s:%d\n", symbol.name, strlen(symbol.name));
    }

    EXIT();
}


#include "contiki.h"

#include "config.h"


/*---------------------------------------------------------------------------*/
/* The Trust Assessment Process                                              */

#include "ta_mod.h"


/*---------------------------------------------------------------------------*/
/* Some processes and threads to keep the system busy:                       */

#ifdef WITH_EXAMPLE_NET
#include "apps/networking.h"
#endif

#ifdef WITH_ANTELOPE
#include "apps/shell-db.h"
#endif

#include "apps/hello-world.h"
#include "apps/attacker.h"
#include "apps/multi-threading.h"


/*---------------------------------------------------------------------------*/

AUTOSTART_PROCESSES(
    &ta_mod_process,
#ifdef WITH_EXAMPLE_NET
    &example_psock_server_process,
#endif
#ifdef WITH_ANTELOPE
    &db_shell,
#endif
    &hello_world_process,
    &attacker_process,
    &multi_threading_process
  );


#include <string.h>

int putchar(int c)
{
#ifdef __SANCUS_SIM
    P1OUT = c;
    P1OUT |= 0x80;
    return c;
#endif
#ifdef __SANCUS_LOAD
    if (c == '\n')
        uart2_write_byte('\r');
    uart2_write_byte(c);
    return c;
#endif
}

int puts_n(char *s)
{
  int i;

  if (s) {
    for (i = 0; i < strlen(s); i++) {
      putchar(s[i]);
    }
  }

  return (0);
}

void puthex_byte(uint8_t v)
{
  const char hexconv[] = "0123456789abcdef";
  putchar(hexconv[v >> 4]);
  putchar(hexconv[v & 0x0f]);
}

void puthex(uint8_t *v, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    puthex_byte(v[i]);
  }
}




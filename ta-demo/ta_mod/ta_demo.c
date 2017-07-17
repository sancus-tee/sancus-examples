
#include "contiki.h"
#include "config.h"


/*---------------------------------------------------------------------------*/
#include <helpers.h>
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


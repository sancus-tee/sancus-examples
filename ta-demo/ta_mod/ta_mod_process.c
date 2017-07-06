
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "contiki.h"
#include "contiki-net.h"
#include "sys/mt.h"

#include "config.h"
#include "ta_mod.h"
#include "ta_internals.h"


PROCESS(ta_mod_process, "TA Module Process");



/*---------------------------------------------------------------------------*/
/* Functions needed for debugging                                            */
/*---------------------------------------------------------------------------*/

void __attribute__ ((noinline))
  printf1 (const char *format, void *v1)
{
  printf (format, v1);
  return;
}

#ifdef __DEBUG_PRINTF_WRAPPERS
void __attribute__ ((noinline))
  printf2 (const char *format, void *v1, void *v2)
{ 
  printf (format, v1, v2);
  return;
}

void __attribute__ ((noinline))
  printf3 (const char *format, void *v1, void *v2, void *v3)
{
  printf (format, v1, v2, v3);
  return;
}
#endif


/*---------------------------------------------------------------------------*/
/* Helpers for hashing and crypto                                            */
/*---------------------------------------------------------------------------*/

char * __attribute__ ((noinline))
  TABinToString (unsigned char *in, size_t in_len)
{
  if (in == NULL || in_len == 0) { return (NULL); }

  size_t bufsize = (2 * in_len * sizeof(unsigned char)) + 1;
  char *buf = malloc (bufsize);
  if (buf == NULL) { return (NULL); }

  int i;
  for (i = 0; i < in_len; i++) {
    snprintf (buf + (2 * i), 3, "%02x", (unsigned int) in[i]);
  }
  buf[bufsize - 1] = '\0';

  return (buf);
}

char * __attribute__ ((noinline))
  TAHashToString (unsigned char *h)
{
  return (TABinToString(h, HASH_SIZE));
}



/*---------------------------------------------------------------------------*/
/* Process and unprotected code for the TA module.                           */

int TAStatus (enum TAStatusQuerry q)
{
  static unsigned char* status;
  static char*          sout;
  static int            status_len;

  dint();
  switch (q) {
    case TAStatusInvars: {
      status = TAInvarsStatus ((unsigned char *) "12345678", 9, &status_len);
      break;
    }
    case TAStatusProcesses: {
      status = TAProcessStatus ((unsigned char *) "12345678", 9, &status_len);
      break;
    }
    default: {
      break;
    }
  }
  eint();

  if (status && status_len > 0) {
    sout = TABinToString(status, status_len);
    if (sout) {
      printf ("Status %i: %i -- %s\n", q, status_len, sout);
      free (sout); sout = NULL;
    }
    free (status); status = NULL;
  }

  return 0;
}

int TARegInvar (void *data, unsigned int size)
{
  static int res;

  dint();
  res = TARegisterInvar (data, size);
  eint();

  return (res);
}

void ta_core_enable (void)
{
  __TIMING_INIT();
  if (sancus_enable(&TACore) > 0) {
    __TIMING_PRINT("+++ ta_core_enable(): ");
#ifdef __TIMING
    printf ("     == %llu\n", tscd_);
#endif
    puts("TACore: enabled.");
  } else {
    puts("*** sancus_enable(&TACore) failed.");
    exit(1);
  }


#ifdef __TIMING
  volatile int tmp;
  dint();
  __TIMING_REINIT();
  tmp = NoTAEntryPoint();
  __TIMING_PRINT("+++ unprot call from unprot code: ");
  __TIMING_REINIT();
  tmp = TAEntryPoint();
  __TIMING_PRINT("+++ sm_entry call from unprot code: ");
  eint();
#endif

}

PROCESS_THREAD(ta_mod_process, ev, data)
{
  PROCESS_BEGIN();
  
  static struct etimer timer;
  static int rounds = 0;
  
#if (TA_SCHEDULER_PROTECTION != 1)
  ta_core_enable();
#endif

  while (1) {
    printf ("TA process: event 0x%x\n", ev);
    dint(); 
    TAMainFunc(ev);
    eint();
    
    rounds++;
    if (rounds == 2) {
      TAStatus (TAStatusInvars);
      TAStatus (TAStatusProcesses);
      rounds = 0;
    }

    etimer_set(&timer, CLOCK_TA * CLOCK_SECOND);
    PROCESS_WAIT_EVENT();
  } 
    
  PROCESS_END();
}


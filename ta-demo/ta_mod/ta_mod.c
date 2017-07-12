
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "contiki.h"
#include "contiki-net.h"
#include "sys/mt.h"

#include "helpers.h"
#include "config.h"
#include "ta_mod.h"
#include "ta_internals.h"

#include <sancus_support/elf.h>
#include <sancus_support/global_symtab.h>
#include <sancus_support/sm_control.h>


DECLARE_SM(TACore, 0x1234);


#ifdef __DEBUG
#ifdef __SANCUS_MSP430
  #include "util/memcpy.c"
#endif
#endif


int SM_DATA(TA_SM_NAME) TAInitialised = 0;

#if (TA_SCHEDULER_PROTECTION == 1)
  #include "ta_mod_scheduler.c"
#endif


/*---------------------------------------------------------------------------*/
/* TAInit()                                                                  */

static int SM_FUNC(TA_SM_NAME) TAInit (void)
{

  if (! TAInitialised) {
    TAInvarInit ();
    TAProcInit ();

    TAInitialised = 1;
  }

  return (TA_SUCCESS);
}


/*---------------------------------------------------------------------------*/
/* Interface Function: TAMainFunc() -- main() of the TA module.              */

int SM_ENTRY(TA_SM_NAME) TAMainFunc (process_event_t ev)
{
  __TIMING_INIT();

#ifdef __TIMING
  volatile int tmp;
  if (!TAInitialised) {
    tmp = NoTAEntryPoint();
    __TIMING_PRINT("+++ unprot call from prot code: ");
    __TIMING_REINIT();
    tmp = TAEntryPoint();
    __TIMING_PRINT("+++ sm_entry call from prot code: ");
    __TIMING_REINIT();
  }
#endif

  if (!TAInitialised) {
    TAInit ();
  }

  // This needs to be done after processes are initialised.
  if (ev == PROCESS_EVENT_TIMER && TAInitialised == 1) {
#if (PROTECT_IVTBL == 1)
    // protect Interrupt Vector Table
    // 0000ffe0 <__ivtbl_16>: 32 bytes
    TARegisterInvar((void *) 0xffe0,32);
#endif

#if (TA_PROCESS_INSPECTOR == 1)
    TAScanProcessesInit();
#endif

    TAInitialised = 2;
  }

  __TIMING_PRINT("+++ TAMainFunc(): initialisation ");
  __TIMING_REINIT();

#if (TA_PROCESS_INSPECTOR == 1)
  TACheckProcesses();
  __TIMING_PRINT("+++ TACheckProcesses(): ");
  __TIMING_REINIT();
#endif
  TACheckInvars ();
  __TIMING_PRINT("+++ TACheckInvars(): ");

  return (TA_SUCCESS);
}

/*---------------------------------------------------------------------------*/


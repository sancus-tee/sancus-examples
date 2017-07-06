
#include "contiki.h"
#include "config.h"
#include "ta_mod.h"
#include "ta_internals.h"


#ifdef __TIMING
int SM_ENTRY(TA_SM_NAME) TAEntryPoint (void)
{
  return (0);
}

int __attribute__ ((noinline))
  NoTAEntryPoint (void)
{
  return (0);
}
#endif


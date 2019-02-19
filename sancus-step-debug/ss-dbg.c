#include "ss-dbg.h"
#include <sancus_support/sm_io.h>
#include <sancus_support/timer.h>
#include <sancus/sm_support.h>

DECLARE_SM(ssdbg, 0x1234);

int ss_dbg_interrupted = 0;
int entry_delay = 0;

int SM_ENTRY(ssdbg) ss_dbg_get_isr_latency(void)
{
    entry_delay = TAR;
    if (!ss_dbg_interrupted)
    {
        return 0;
    }
    else
    {
        return TAR;
    }
}

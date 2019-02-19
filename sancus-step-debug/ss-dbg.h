#ifndef SS_DBG_H
#define SS_DBG_H

#include <sancus/sm_support.h>

extern struct SancusModule ssdbg;

int SM_ENTRY(ssdbg) ss_dbg_get_isr_latency(void);

#endif

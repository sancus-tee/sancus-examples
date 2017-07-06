#ifndef __TA_INTERNALS_H

#include "contiki.h"
#include "config.h"
#include "ta_mod.h"


extern int TAInitialised;


int SM_FUNC(TA_SM_NAME) TAInvarInit (void);
int SM_FUNC(TA_SM_NAME) TACheckInvars (void);

int SM_FUNC(TA_SM_NAME) TAScanProcessesInit (void);
int SM_FUNC(TA_SM_NAME) TACheckProcesses (void);
int SM_FUNC(TA_SM_NAME) TAProcessUpdate (void *thread);
int SM_FUNC(TA_SM_NAME) TAProcInit (void);

#ifdef __DEBUG
#ifdef __SANCUS_MSP430
void * SM_FUNC(TA_SM_NAME)
  TAmemcpy(void *dst0, const void *src0, size_t length);
#endif
#endif

char * __attribute__ ((noinline))
  TABinToString (unsigned char *in, size_t in_len);
char * __attribute__ ((noinline))
  TAHashToString (unsigned char *h);


void printf1 (const char *format, void *v1);
#ifdef __DEBUG_PRINTF_WRAPPERS
void printf2 (const char *format, void *v1, void *v2);
void printf3 (const char *format, void *v1, void *v2, void *v3);
#endif

#ifdef __TIMING
int SM_ENTRY(TA_SM_NAME) TAEntryPoint (void);
int NoTAEntryPoint (void);
#endif



#define __TA_INTERNALS_H
#endif


#ifndef __TA_MOD_EXTERNAL_H

#include "contiki.h"

extern struct process ta_mod_process;


#define TA_SUCCESS   0
#define TA_FAILURE   1

enum TAStatusQuerry {
  TAStatusInvars,
  TAStatusProcesses
};

void ta_core_enable (void);
int TAStatus (enum TAStatusQuerry q);
int TARegInvar (void *data, unsigned int size);

#define __TA_MOD_EXTERNAL_H
#endif


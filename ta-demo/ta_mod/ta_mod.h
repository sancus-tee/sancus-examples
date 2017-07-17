#ifndef __TA_MOD_H

#include "config.h"
#include "ta_mod_external.h"

#define TA_SM_NAME   TACore


extern struct SancusModule TACore;


int SM_ENTRY(TA_SM_NAME) TAMainFunc (process_event_t ev);

int SM_ENTRY(TA_SM_NAME) TARegisterInvar (void *data, unsigned int size);
int SM_ENTRY(TA_SM_NAME) TASecureCallProcess (struct process *p,
  process_event_t ev, process_data_t data);

unsigned char* SM_ENTRY(TA_SM_NAME) TAInvarsStatus (unsigned char *nonce,
  int nonce_len, int* status_len);
unsigned char* SM_ENTRY(TA_SM_NAME) TAProcessStatus (unsigned char *nonce,
  int nonce_len, int* status_len);


#define __TA_MOD_H
#endif


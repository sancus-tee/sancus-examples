
#include <stdio.h>
#include <stdlib.h>

#include <sancus_support/tsc.h>
#include "helpers.h"
#include "ta_internals.h"


typedef struct TAProcess {
  int               is_used;
  struct process   *process;
  void             *thread;
  void             *thread_end;
  unsigned long int invoked_n;
  uint64_t          invoked_t;
#ifdef __SANCUS_MSP430
  unsigned char     padding[12];
#endif 
} TAProcess;

static TAProcess     SM_DATA(TA_SM_NAME) TAProcesses[MAX_PROCESSES];


static void * SM_FUNC(TA_SM_NAME)
  TAFindRet (void *start)
{
  unsigned char *addr = start;

// addr:       30 41           ret    
  while (addr < (unsigned char *) 0xffff) {
//    printf3("%p %x %x - ", addr, (void *) addr[0], (void *) addr[1]);
    if (addr[0] == 0x30 && addr[1] == 0x41) {
      return (addr + 1);
    }
    addr++;
  }

  return (NULL);
}

static int SM_FUNC(TA_SM_NAME)
  TAAddProcess (struct process *p)
{
  int i;

  __TIMING_INIT();

  // already in there?
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (TAProcesses[i].is_used != 0 &&
        TAProcesses[i].process == p &&
        TAProcesses[i].thread  == p->thread) {
      return (TA_SUCCESS);
    }
  }

  // new process?
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (TAProcesses[i].is_used == 0) {
      TAProcesses[i].is_used    = 1;
      TAProcesses[i].process    = p;
      TAProcesses[i].thread     = p->thread;
      TAProcesses[i].invoked_n  = 0;
      TAProcesses[i].invoked_t  = 0;
      if (TAProcesses[i].thread != NULL) {
        TAProcesses[i].thread_end = TAFindRet(p->thread);
      }
#ifdef __DEBUG
      printf3 ("TA: adding process: %s @ %p..%p\n", (void *) p->name,
        TAProcesses[i].thread, TAProcesses[i].thread_end);
#endif

  __TIMING_PRINT("+++ TAAddProcess(): ");

      if (TAProcesses[i].thread != NULL &&
          TAProcesses[i].thread_end != NULL) {
        TARegisterInvar (TAProcesses[i].thread,
          (TAProcesses[i].thread_end - TAProcesses[i].thread));
      }
      return (TA_SUCCESS);
    }
  }

  return (TA_FAILURE);
}

int SM_FUNC(TA_SM_NAME)
  TAProcessUpdate (void *thread)
{
  int i;
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (TAProcesses[i].thread  == thread) {
      TAProcesses[i].invoked_n += 1;
      TAProcesses[i].invoked_t = (uint64_t) tsc_read();
      return (TA_SUCCESS);
    }
  }
  return (TA_FAILURE);
}

int SM_FUNC(TA_SM_NAME)
  TAScanProcessesInit (void)
{
  struct process *p;

  p = process_list;
  while (p) {
#ifdef __DEBUG
    printf2 ("TA: process found: %s @ %p\n", (void *) p->name, p->thread);
#endif
    TAAddProcess (p);
    p = p->next;
  }

  return (TA_SUCCESS);
}

int SM_FUNC(TA_SM_NAME)
  TACheckProcesses (void)
{
  int i, j, found;
  struct process *p;

  j = 0;
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (TAProcesses[i].is_used != 0) {
      j++;
      found = 0;
      p = process_list;
      while (p) {
        if (TAProcesses[i].process == p &&
            TAProcesses[i].thread  == p->thread) {
          found = 1;
        }
        p = p->next;
      }
      if (! found) {
        printf1 ("TA: *** process @ %p removed!\n",
          TAProcesses[i].thread);
      }
    }
  }

#ifdef __DEBUG
  printf1 ("TA: %i processes checked.\n", (void *) j);
#endif
  return (TA_SUCCESS);
}


int SM_FUNC(TA_SM_NAME)
  TAProcInit (void)
{
  int i;

  for (i = 0; i < MAX_PROCESSES; i++) {
    TAProcesses[i].is_used = 0;
  }

  return (TA_SUCCESS);
}


unsigned char* SM_ENTRY(TA_SM_NAME) TAProcessStatus (unsigned char *nonce,
  int nonce_len, int* status_len)
{
  static unsigned char *ProcessStatus = NULL;
  static int sl;

  __TIMING_INIT();

  sl = (sizeof(struct TAProcess) * MAX_PROCESSES) + nonce_len;
  *status_len = sl + HASH_SIZE;
  ProcessStatus = malloc (*status_len);
  if (!ProcessStatus) { goto FAIL; }

  sancus_wrap(nonce, nonce_len, TAProcesses, *status_len, ProcessStatus,
    ProcessStatus + sl);

  __TIMING_PRINT("+++ TAProcessStatus(): ");

  return (ProcessStatus);

FAIL:
  *status_len = 0;
  return (NULL); 
}


/*---------------------------------------------------------------------------*/


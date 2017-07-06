
#if (BUILTIN_PROT_SCHED == 1)

#include "ta_internals.h"


#define PROCESS_STATE_NONE        0
#define PROCESS_STATE_RUNNING     1
#define PROCESS_STATE_CALLED      2

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void call_process(struct process *p, process_event_t ev, process_data_t data);
void exit_process(struct process *p, struct process *fromprocess);

static int __attribute__ ((noinline))
__TASecureCallProcess (struct process *p,
  process_event_t ev, process_data_t data)
{
  static int ret = -1;
  eint();
  ret = p->thread(&p->pt, ev, data);
  dint();
  return (ret);
}


int SM_ENTRY(TA_SM_NAME) TASecureCallProcess (struct process *p,
  process_event_t ev, process_data_t data)
{
  static int ret = -1;

/* Check __TASecureCallProcess()
 * Check p->thread
 * Store call and timing somewhere
 *
 * Q: check caller? Do we gain anything from that?
 * Q: Why noth check all code sections at every call?
 * A: We want to minimise overhead and check a minimum subset of
 *    components that enable us to give relatively strong guarantees
 *    already. Importantly, this is not a countermeasure against a specific
 *    known attack but a pretty much generic approach to mitigate unknown
 *    attacks and to detect faulty corruption of software components.
 */


#ifdef __DEBUG
  printf2("TA: calling process '%s' with event %d.\n",
    (void *) PROCESS_NAME_STRING(p), (void *) ((int) ev));
#endif
#if (TA_PROCESS_INSPECTOR == 1)
  __TIMING_INIT();
  if (TAInitialised) {
    TAProcessUpdate (p->thread);
  }
  __TIMING_PRINT("+++ TASecureCallProcess(): update inv & time ");
#endif
  ret = __TASecureCallProcess (p, ev, data); 

  return (ret);
}



/* modified from contiki/core/sys/process.c */
void
exit_process(struct process *p, struct process *fromprocess)
{
  register struct process *q;
  struct process *old_current = process_current;

  PRINTF("process: exit_process '%s'\n", PROCESS_NAME_STRING(p));

  /* Make sure the process is in the process list before we try to
     exit it. */
  for(q = process_list; q != p && q != NULL; q = q->next);
  if(q == NULL) {
    return;
  }

  if(process_is_running(p)) {
    /* Process was running */
    p->state = PROCESS_STATE_NONE;

    /*
     * Post a synchronous event to all processes to inform them that
     * this process is about to exit. This will allow services to
     * deallocate state associated with this process.
     */
    for(q = process_list; q != NULL; q = q->next) {
      if(p != q) {
        call_process(q, PROCESS_EVENT_EXITED, (process_data_t)p);
      }
    }

    if(p->thread != NULL && p != fromprocess) {
      /* Post the exit event to the process that is about to exit. */
      process_current = p;
      dint();
      TASecureCallProcess(p, PROCESS_EVENT_EXIT, NULL);
      eint();
    }
  }

  if(p == process_list) {
    process_list = process_list->next;
  } else {
    for(q = process_list; q != NULL; q = q->next) {
      if(q->next == p) {
        q->next = p->next;
        break;
      }
    }
  }

  process_current = old_current;
}


/* modified from contiki/core/sys/process.c */
void
call_process(struct process *p, process_event_t ev, process_data_t data)
{
  int ret;

#if DEBUG
  if(p->state == PROCESS_STATE_CALLED) {
    printf("process: process '%s' called again with event %d\n", PROCESS_NAME_STRING(p), ev);
  }
#endif /* DEBUG */

  if((p->state & PROCESS_STATE_RUNNING) &&
     p->thread != NULL) {
    PRINTF("process: calling process '%s' with event %d\n", PROCESS_NAME_STRING(p), ev);
    process_current = p;
    p->state = PROCESS_STATE_CALLED;
    dint();
    ret = TASecureCallProcess(p, ev, data);
    eint();
    if(ret == PT_EXITED ||
       ret == PT_ENDED ||
       ev == PROCESS_EVENT_EXIT) {
      exit_process(p, p);
    } else {
      p->state = PROCESS_STATE_RUNNING;
    }
  }
}


#endif /* #if (BUILTIN_PROT_SCHED == 1) */


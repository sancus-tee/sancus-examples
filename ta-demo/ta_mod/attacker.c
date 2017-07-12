
#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "sys/mt.h"

#include "helpers.h"
#include "config.h"
#include "attacker.h"

#include <string.h>


PROCESS(attacker_process, "Attacker process");

/*---------------------------------------------------------------------------*/
#define ATTACKER_STUB_SIZE  10
static void attacker_stub (void)
{
  puts("A was here!");
  return;
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(attacker_process, ev, data)
{
  PROCESS_BEGIN();


  static struct etimer timer;
  static int r;
  static int i;

  etimer_set(&timer, CLOCK_SECOND / CLOCK_DIV);


  while (1) {
    PROCESS_WAIT_EVENT();


    dint();
    r = abs(rand() % 11);
    i = 0;
    printf("--- Attacker (%i).\n", r);
    switch (r) {
      // hijack user proccess pointer
      case 0: {
        struct process *p;
        p = process_list;
        while (p) {
          if (attacker_process.thread == p->thread) {
            p = p->next;
            i++;
          } else {
            printf("    hijacking p.%i: %s\n", i, p->name);
            p->thread = attacker_process.thread; // overwrite process ptr
            break;
          }
        }
        break;
      }
      // remove process
      case 1: {
        struct process *p;
        struct process *prev;
        p    = process_list;
        prev = NULL;
        while (p && p->next) {
          if (attacker_process.thread == p->thread) {
            prev = p;
            p = p->next;
            i++;
          } else {
            if (! prev) {
              printf("    removing p.%i: %s\n", i, p->name);
              process_list = p;                  // remove process
            } else {
              printf("    removing p.%i: %s\n", i, p->name);
              prev->next = p->next;              // remove process
            }
            break;
          }
        }
        break;
      }
      // hijack user proccess code
      case 2: {
        struct process *p;
        p = process_list;
        while (p) {
          if (attacker_process.thread == p->thread) {
            p = p->next;
            i++;
          } else {
            /* overwrite process code */
            printf("    overwriting p.%i @ %p: %s\n", i, p->thread, p->name);
            memcpy(p->thread, &attacker_stub, ATTACKER_STUB_SIZE);
            break;
          }
        }
        break;
      }
      // hijack ISRs
      case 3: {
        // 0000ffe0 <__ivtbl_16>: 32 bytes
        printf("    messing with ivtbl\n");
        memcpy((void *) 0xffe0, &r, 2);
        break;
      }
      // hang
      case 4: {
        while ( 1 ) { };
        break;
      }
      default: {
        break;
      }
    }
    eint();


    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/



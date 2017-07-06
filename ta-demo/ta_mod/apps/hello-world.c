
#include <stdio.h>
#include "contiki.h"
#include "sys/mt.h"

#include "../config.h"
#include "hello-world.h"

#ifdef __DEBUG
  #include <stdlib.h>
#endif


PROCESS(hello_world_process, "Hello world process");

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();


  static struct etimer timer;
  static int    rounds = 0;

  etimer_set(&timer, CLOCK_SECOND / CLOCK_DIV);

//  printf ("wait time = %lu\n", CLOCK_SECOND/10);

  while (1) {
//    puts("D: hello_world_process...suspended.");
    PROCESS_WAIT_EVENT();
//    puts("D: hello_world_process...continued.");

    puts("--- Hello, world.");

#ifdef __DEBUG
    rounds++;
    if (rounds < 0) { rounds = 0; }
    if (rounds == ROUNDS) { exit(0); }
#endif

    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/



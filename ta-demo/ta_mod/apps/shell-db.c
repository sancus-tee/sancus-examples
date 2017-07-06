/*
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *	A small command-line interface for the database system.
 * \author
 * 	Nicolas Tsiftes <nvt@sics.se>
 */

#include <stdio.h>
#include <stdio.h>
#include "contiki.h"
#include "sys/mt.h"
#include "dev/serial-line.h"
#include "antelope.h"

#include "../config.h"
#include "../ta_integrity.h"
#include "shell-db.h"


PROCESS(db_shell, "DB shell");

PROCESS_THREAD(db_shell, ev, data)
{
  PROCESS_BEGIN();

  static db_handle_t handle;
  db_result_t result;
  static tuple_id_t matching;
  static tuple_id_t processed;
  static struct etimer timer;


  static char *q[] = {
    "CREATE RELATION sensor;",
    "CREATE ATTRIBUTE id DOMAIN INT IN sensor;",
    "CREATE ATTRIBUTE position DOMAIN LONG IN sensor;",
    "CREATE INDEX sensor.id TYPE INLINE;",
    "CREATE INDEX sensor.position TYPE MAXHEAP;",
    "INSERT (1, 45) INTO sensor;",
    "INSERT (2, 42) INTO sensor;"
  };
  static unsigned int c = 0;

  etimer_set(&timer, CLOCK_SECOND / CLOCK_DIV);

  db_init();

  while (1) {
//    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message && data != NULL);
    PROCESS_WAIT_EVENT();

    etimer_set(&timer, CLOCK_SECOND / CLOCK_DIV);
    if (c > 6) { c = 5; }
    printf("+++ Query \"%s\"...\n", q[c]);

    result = db_query(&handle, q[c]);
    c += 1;
    if(DB_ERROR(result)) {
      printf("Query \"%s\" failed: %s\n",
             (unsigned char *)q[c], db_get_result_message(result));
      db_free(&handle);
      continue;
    }

    if(!db_processing(&handle)) {
      printf("OK\n");
      continue;
    }

    db_print_header(&handle);

    matching = 0;
    processed = 0;

    while(db_processing(&handle)) {
      PROCESS_PAUSE();
      result = db_process(&handle);
      switch(result) {
      case DB_GOT_ROW:
        /* The processed tuple matched the condition in the query. */
        matching++;
        processed++;
        db_print_tuple(&handle);
        break;
      case DB_OK:
        /* A tuple was processed, but did not match the condition. */
        processed++;
        continue;
      case DB_FINISHED:
        /* The processing has finished. Wait for a new command. */
        printf("[%ld tuples returned; %ld tuples processed]\n",
               (long)matching, (long)processed);
        printf("OK\n");
      default:
        if(DB_ERROR(result)) {
          printf("Processing error: %s\n", db_get_result_message(result));      
        }
        db_free(&handle);
        break;
      }
    }
  }

  PROCESS_END();
}

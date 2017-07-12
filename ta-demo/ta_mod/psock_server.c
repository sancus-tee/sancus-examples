
#include <stdio.h>
#include <string.h>

#include "contiki.h"
#include "contiki-net.h"

#include "config.h"
#include "networking.h"


// http://senstools.gforge.inria.fr/doku.php?id=contiki:examples


static PT_THREAD(handle_connection(struct psock *p, unsigned char *buffer))
{
  PSOCK_BEGIN(p);
  PSOCK_SEND_STR(p, "Welcome, please type something and press return.\n");
  PSOCK_READTO(p, '\n');
  PSOCK_SEND_STR(p, "Got the following data: ");
  PSOCK_SEND(p, buffer, PSOCK_DATALEN(p));
  PSOCK_SEND_STR(p, "Good bye!\r\n");
  PSOCK_CLOSE(p);
  PSOCK_END(p);
}

PROCESS(example_psock_server_process, "Example protosocket server");

PROCESS_THREAD(example_psock_server_process, ev, data)
{
  PROCESS_BEGIN();

  static struct psock net_ps;
  static unsigned char net_buffer[50];

  tcp_listen(UIP_HTONS(12345));
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_connected()) {
      PSOCK_INIT(&net_ps, net_buffer, sizeof(net_buffer));
      while(!(uip_aborted() || uip_closed() || uip_timedout())) {
        PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
        puts("example psock: handling request.");
        handle_connection(&net_ps, net_buffer);
      }
    }
  }
  PROCESS_END();
}



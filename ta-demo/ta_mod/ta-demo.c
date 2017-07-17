
#include "contiki.h"
#include "config.h"


/*---------------------------------------------------------------------------*/
/* The Trust Assessment Process                                              */

#include "ta_mod_external.h"


/*---------------------------------------------------------------------------*/
/* Some processes and threads to keep the system busy:                       */

#ifdef WITH_EXAMPLE_NET
#include "psock_server.h"
#endif

#ifdef WITH_ANTELOPE
#include "shell-db.h"
#endif

#include "hello-world.h"
#include "attacker.h"
#include "multi-threading.h"


/*---------------------------------------------------------------------------*/

AUTOSTART_PROCESSES(
    &ta_mod_process,
#ifdef WITH_EXAMPLE_NET
    &example_psock_server_process,
#endif
#ifdef WITH_ANTELOPE
    &db_shell,
#endif
    &hello_world_process,
    &attacker_process,
    &multi_threading_process
  );

/*---------------------------------------------------------------------------*/


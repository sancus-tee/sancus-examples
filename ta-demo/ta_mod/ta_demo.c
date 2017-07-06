
#include "contiki.h"

#include "config.h"


/*---------------------------------------------------------------------------*/
/* The Trust Assessment Process                                              */

#include "ta_mod.h"


/*---------------------------------------------------------------------------*/
/* Some processes and threads to keep the system busy:                       */

#ifdef WITH_EXAMPLE_NET
#include "apps/networking.h"
#endif

#ifdef WITH_ANTELOPE
#include "apps/shell-db.h"
#endif

#include "apps/hello-world.h"
#include "apps/attacker.h"
#include "apps/multi-threading.h"


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


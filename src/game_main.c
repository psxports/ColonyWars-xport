#include "game_functions.h"
#include "native_dispatch.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Unresolved until the original PsyQ graph type storage is audited */
const uint32 xport_gpu_graph_type_address = 0u;

/* Unresolved CD bindings until original storage is audited */
const uint32 xport_cd_sync_callback_address = 0u;
const uint32 xport_cd_ready_callback_address = 0u;
const uint32 xport_cd_status_address = 0u;
const uint32 xport_cd_setloc_table_address = 0u;

static void game_load_initial_storage(void)
{
    size_t size;
    size_t copied;
    if (!xport_file_size("DATA/CW_BOOT.BIN", &size) || size != 0x64800u ||
        !xport_file_read("DATA/CW_BOOT.BIN", psx_addr(0x80010000, 0x64800), 0x64800u, &copied) || copied != 0x64800u)
    {
#ifndef NDEBUG
        fprintf(stderr, "Cannot load exact native boot data: %d\n", errno);
#endif
        abort();
    }
}

void xport_main(void)
{
    game_load_initial_storage();
    game_bind_native_callbacks();
    game_entry();
}

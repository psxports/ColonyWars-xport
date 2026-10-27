#include "game_functions.h"
#include "native_dispatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The loaded image selects native code at overlapping overlay addresses */
static uint32 native_overlay;

static void game_unbound_native_call(uint32 target, uint32 count)
{
#ifndef NDEBUG
    fprintf(stderr, "Unbound native callback %08X/%u in overlay %u\n", target, count, native_overlay);
#endif
    abort();
}

#include "native_scalar_dispatch.inc"

static uint32 game_native_dispatch(uint32 target, uint32 count, uint32 a0, uint32 a1, uint32 a2)
{
    if (target < 0x800744A8)
        return game_dispatch_main(target, count, a0, a1, a2);
    switch (native_overlay)
    {
    case 1:
        return game_dispatch_front(target, count, a0, a1, a2);
    case 2:
        return game_dispatch_game(target, count, a0, a1, a2);
    case 3:
        return game_dispatch_anim(target, count, a0, a1, a2);
    default:
        game_unbound_native_call(target, count);
        return 0;
    }
}

void game_select_native_overlay(const char *name)
{
    if (strcmp(name, "FRONT") == 0)
        native_overlay = 1;
    else if (strcmp(name, "GAME") == 0)
        native_overlay = 2;
    else if (strcmp(name, "ANIM") == 0)
        native_overlay = 3;
    else
    {
#ifndef NDEBUG
        fprintf(stderr, "Unknown native overlay %s\n", name);
#endif
        abort();
    }
}

static sint32 game_native_bios_callback(void *context, uint32 target)
{
    game_native_dispatch(target, 0, 0, 0, 0);
    return 1;
}

void game_bind_native_callbacks(void)
{
    native_overlay = 0;
    psx_bind_guest_dispatch(game_native_dispatch);
    psx_bios_bind_guest_callback_service(game_native_bios_callback, NULL);
}

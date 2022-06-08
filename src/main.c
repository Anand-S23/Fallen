#define GS_IMPL
#include <gunslinger/gs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "main.h"
#include "renderer.c"
#include "game.c"

// TODO: This must be the same order as app_state_t
// enum, maybe look into for robust solution
static void (*update_state[])(app_t *app) = {
    update_menu,
    update_game,
    update_editor,
    update_none
};

static app_t app = {0};

// Core //

void init(void)
{
    app = (app_t) {
        .renderer = (renderer_t){0},
        .game_data = (game_data_t){0},
        .menu_data = (menu_data_t){0},
        .editor_data = (editor_data_t){0},
        .current_state = GAME
    };

    init_renderer(&app.renderer);
}

void update(void)
{
    // Update app based on state
    (*update_state[app.current_state])(&app);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) {
        .init = init,
        .update = update
    };
}


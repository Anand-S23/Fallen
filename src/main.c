#define GS_IMPL
#include <gunslinger/gs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "game.h"
#include "menu.h"
#include "editor.h"

#include "main.h"
#include "game.c"
#include "menu.c"
#include "editor.c"

static app_t app = {0};

void (*update_state[])(app_t *app) = {
    update_none,
    update_game,
    update_menu,
    update_editor
};

// Core //

void init(void)
{
    app = (app_t) {
        .cb = gs_command_buffer_new(),
        .current_state = GAME,
        .game_data = (game_data_t){0},
        .menu_data = (menu_data_t){0},
        .editor_data = (editor_data_t){0}
    };

    init_game(&app);
    init_menu(&app);
    init_editor(&app);
}

void update(void)
{
    // Update app based on state
    (*update_state[(int)app.current_state])(&app);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) {
        .init = init,
        .update = update
    };
}


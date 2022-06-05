#include <gunslinger/gs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dll_loader.h"

#include "main.h"
#include "renderer.c"
#include "game.c"

static app_t app = {0};

/*
static void (*update_state[])(app_t *app) = {
    update_none,
    update_game,
    update_menu,
    update_editor
};
*/

static game_code_t load_code(void)
{
    game_code_t gc = {0};

    dl_copy_dll("./build/fallen.so", "./build/temp_fallen_2.so");
    gc.game_handle = dl_open("./build/temp_fallen_2.so");
    if (gc.game_handle)
    {
        gc.update_game = (game_func_t *)dl_load_function(gc.game_handle, "update_game");

        gc.is_valid = !!(gc.update_game);
    }

    if (!gc.is_valid)
    {
        printf("game code is not valid\n");
        gc.update_game = (game_func_t *)game_func_stub;
    }

    return gc;
}

static void unload_code(game_code_t *game_code)
{
    if (game_code->game_handle)
    {
        dl_close(game_code->game_handle);
    }

    game_code->is_valid = 0;
    game_code->update_game = (game_func_t *)game_func_stub;
}

// Core //

game_code_t game_code;

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
    game_code = load_code();
}

void update(void)
{
    // game_code_t game_code = load_code();
    if (gs_platform_key_pressed(GS_KEYCODE_LSHIFT))
    {
        printf("Pressing\n");
        unload_code(&game_code);
        game_code = load_code();
    }
    
    // Update app based on state
    begin_render_pass(&app.renderer);
    game_code.update_game(&app);
    end_render_pass(&app.renderer);

    // unload_code(&game_code);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) {
        .init = init,
        .update = update
    };
}


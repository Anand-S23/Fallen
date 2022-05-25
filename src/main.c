#include <gunslinger/gs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h> 

#include "main.h"
#include "renderer.c"
#include "game.c"

static app_t app = {0};
static reload_t reloader = {0};

/*
static void (*update_state[])(app_t *app) = {
    update_none,
    update_game,
    update_menu,
    update_editor
};
*/

static void load_code(reload_t *reloader)
{
    reloader->is_valid = 0;
    reloader->game_handle = dlopen("./build/fallen.so", RTLD_NOW);

    if (reloader->game_handle)
    {
        reloader->update_game = (update_game_func_t *)dlsym(reloader->game_handle,
                                                            "update_game");

        if (reloader->update_game)
        {
            reloader->is_valid = 1;
        }
        else
        {
            printf("invalid\n");
        }
    }
    else
    {
        printf("dlopen error: %s\n", dlerror());
    }
}

static void unload_code(reload_t *reloader)
{
    if (reloader->is_valid)
    {
        if (dlclose(reloader->game_handle) != 0)
        {
            printf("dlclose error: %s\n", dlerror());
        }
    }
}

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

    reloader = (reload_t) {0};
}

void update(void)
{
    load_code(&reloader);
    
    // Update app based on state
    if (reloader.is_valid)
    {
        begin_render_pass(&app.renderer);
        update_game(&app);
        end_render_pass(&app.renderer);
    }

    unload_code(&reloader);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) {
        .init = init,
        .update = update
    };
}


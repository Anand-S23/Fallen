#include "main.h"

// Core //

static void init_game(app_t *app)
{
    game_data_t *data = &app->game_data;
}

extern void update_game(app_t *app)
{
    renderer_t *renderer = &app->renderer;
    game_data_t *data = &app->game_data;

    if (!data->initialized)
    {
        init_game(app);
        data->initialized = 1;
        renderer->set_clear_color(renderer, 1, 1, 1, 0);
    }

    int32_t width, height, num_comps;
    void *texture_data = NULL;
    gs_util_load_texture_data_from_file("./assets/characters.png",
                                        &width, &height, &num_comps,
                                        &texture_data, 1);
}


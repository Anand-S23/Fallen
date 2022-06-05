#include "main.h"

// Core //

extern void update_game(app_t *app)
{
    renderer_t *renderer = &app->renderer;
    game_data_t *data = &app->game_data;

    if (!data->initialized)
    {
        renderer->set_clear_color(renderer, 1.f, 0.f, 0.f, 1.f);

        data->x = 100;
        data->y = 150;

        void *texture_data = NULL;
        gs_util_load_texture_data_from_file("./assets/fire.png",
                                            &data->width, &data->height, &data->num_comps,
                                            &texture_data, 1);

        data->tex = gs_graphics_texture_create (
            &(gs_graphics_texture_desc_t) {
                .type = GS_GRAPHICS_TEXTURE_2D, 
                .width = data->width,
                .height = data->height, 
                .data = texture_data,
                .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,
                .min_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST, 
                .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST
            }
        );


        data->initialized = 1;
    }

    renderer->draw_texture(&renderer->render_queue, gs_v2(0, 0),
                           gs_v2(data->width, data->height), data->tex);
}


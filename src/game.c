#include "game.h"

void update_none(app_t *app) {}

// Menu //

void update_menu(app_t *app)
{
}

// Game //

void update_game(app_t *app)
{
    renderer_t *renderer = &app->renderer;
    game_data_t *data = &app->game_data;

    if (!data->initialized)
    {
        renderer_set_clear_color(renderer, 1.f, 1.f, 0.f, 1.f);

        data->x = 100;
        data->y = 100;

        void *texture_data = NULL;
        gs_util_load_texture_data_from_file("./assets/characters.png",
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

    // Update

    // Render
    begin_render_pass(renderer);
    renderer_draw_texture(renderer, gs_v2(data->x, data->y), gs_v2(data->width, data->height), data->tex);
    renderer_draw_texture(renderer, gs_v2(0, 0), gs_v2(data->width, data->height), data->tex);
    end_render_pass(renderer);
}

// Editor //

void update_editor(app_t *app)
{
}


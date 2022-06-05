#ifndef GAME_H
#define GAME_H

typedef struct menu_data_t
{
} menu_data_t;

typedef struct game_data_t
{
    b32 initialized;
    int x;
    int y;

    int32_t width;
    int32_t height;
    int32_t num_comps;
    gs_handle(gs_graphics_texture_t) tex;
} game_data_t;

typedef struct editor_data_t
{
} editor_data_t;

#endif

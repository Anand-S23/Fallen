#ifndef MAIN_H
#define MAIN_H

#include <gunslinger/gs.h>
#include "renderer.h"
#include "game.h"

typedef enum app_state_t
{
    NONE,
    GAME,
    MENU,
    EDITOR
} app_state_t;

typedef struct game_data_t
{
    gs_handle(gs_graphics_vertex_buffer_t) vbo;
    gs_handle(gs_graphics_index_buffer_t) ibo;
    gs_handle(gs_graphics_pipeline_t) pip;
    gs_handle(gs_graphics_shader_t) shader;
    gs_handle(gs_graphics_uniform_t) u_projection;
    gs_handle(gs_graphics_uniform_t) u_tex;
    gs_handle(gs_graphics_texture_t) tex;
    f32 vertices[16];
    b32 initialized;
} game_data_t;

typedef struct menu_data_t
{
} menu_data_t;

typedef struct editor_data_t
{
} editor_data_t;

typedef struct app_t
{
    renderer_t renderer;
    app_state_t current_state;
    game_data_t game_data;
    menu_data_t menu_data;
    editor_data_t editor_data;
} app_t;

typedef void update_game_func_t(app_t *app);

// TODO: Make reload cross platform
typedef struct reload_t
{
    void *game_handle; // DLL
    // TODO: Time last written
    b32 is_valid;
    update_game_func_t *update_game;
} reload_t;

static void update_none(app_t *app)
{
    // NOTE: Current State is NONE when the game needs to quit
    gs_quit();
}

#endif

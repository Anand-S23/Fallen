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

typedef struct app_t
{
    renderer_t renderer;
    app_state_t current_state;
    game_data_t game_data;
    menu_data_t menu_data;
    editor_data_t editor_data;
} app_t;

typedef void game_func_t(app_t *app);
typedef struct game_code_t
{
    void *game_handle;
    game_func_t *update_game;
    b32 is_valid;
} game_code_t;

static void game_func_stub(app_t *app) {}

#endif

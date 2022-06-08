#ifndef MAIN_H
#define MAIN_H

#include "renderer.h"
#include "game.h"

typedef enum app_state_t
{
    MENU,
    GAME,
    EDITOR,
    NONE
} app_state_t;

typedef struct app_t
{
    renderer_t renderer;
    app_state_t current_state;
    game_data_t game_data;
    menu_data_t menu_data;
    editor_data_t editor_data;
} app_t;

#endif

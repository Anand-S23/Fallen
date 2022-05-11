#ifndef MAIN_H
#define MAIN_H

typedef enum app_state_t
{
    NONE,
    GAME,
    MENU,
    EDITOR
} app_state_t;

typedef struct app_t
{
    gs_command_buffer_t cb;
    app_state_t current_state;
    game_data_t game_data;
    menu_data_t menu_data;
    editor_data_t editor_data;
} app_t;

static void update_none(app_t *app)
{
    // NOTE: Current State is NONE when the game needs to quit
    gs_quit();
}

#endif

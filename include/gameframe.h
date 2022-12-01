#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <stdbool.h>
#include <stdint.h>
#include <SFML/Graphics.h>

typedef struct game_state_t {
    uint8_t state[256];
    double net_in[64];
    int score;
    bool game_ended;
} GameState;

typedef struct game_t {
    GameState* (*new_game_func)();
    void (*update_func)(GameState*, double[6]);
    sfRenderTexture* (*render_game)(uint8_t*);
    float framerate;
    int res[2];
} Game;

Game *new_game(GameState *(*new_game_func)(), void (*update_func)(GameState*, double[6]), sfRenderTexture* (*render_game)(uint8_t*), float framerate, int resx, int resy);
void free_gamestate(GameState *gamestate);

#endif
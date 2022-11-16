#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <stdbool.h>

typedef struct game_state_t {
    char state[1024];
    double net_in[64];
    double score;
    bool game_ended;
} GameState;

typedef struct game_t {
    GameState* (*new_game_func)();
    void (*update_func)(GameState*, double[6]);
} Game;

Game *new_game(GameState *(*new_game_func)(), void (*update_func)(GameState*, double[6]));
void free_gamestate(GameState *gamestate);

#endif
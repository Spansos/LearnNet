#ifndef GAMEFRAME_H
#define GAMEFRAME_H


typedef struct game_t {
    char* (*new_game_func)();
    int (*update_func)(char[1024], double[6]);
} Game;


#endif
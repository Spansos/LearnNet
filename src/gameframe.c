#include <gameframe.h>
#include <stdlib.h>


Game *new_game(GameState *(*new_game_func)(), void (*update_func)(GameState*, double[6]), sfRenderTexture* (*render_game)(uint8_t*), float framerate) {
    Game *r_game = calloc(1, sizeof(Game));
    r_game->new_game_func = new_game_func;
    r_game->update_func = update_func;
    r_game->render_game = render_game;
    r_game->framerate = framerate;
    return r_game;
}


void free_gamestate(GameState *gamestate) {
    free(gamestate->state);
    free(gamestate);
}
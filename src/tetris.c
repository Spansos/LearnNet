#include <gameframe.h>
#include <math.h>
#include <stdlib.h>

GameState* new_game() {
    GameState *gamestate = calloc(1, sizeof(gamestate));
    return gamestate;
}

void update(GameState* gamestate, double input[6]) {

}
#include <gameframe.h>
#include <math.h>
#include <stdlib.h>

GameState* new_game() {
    GameState *gamestate = calloc(1, sizeof(gamestate));
    return gamestate;
}

uint8_t rotate_piece_counterclockwise(uint8_t piece) {
    return piece<<2 | piece>>6;
}

uint8_t rotate_piece_clockwise(uint8_t piece) {
    return piece>>2 | piece<<6;
}

void new_piece(uint8_t *state) {
    if (rand()%2) {
        state[0] = 0b00'00'11'11;
    }
    else {
        state[0] = 0b11'00'00'11;
    }
    for (int i=0; i<rand()%4; i++) {
        state[0] = rotate_piece_clockwise(state[0]);
    }
    state[1] = 3;
    state[2] = 0;
}

bool is_collided(uint8_t piece, uint8_t x, uint8_t y, uint8_t field[50]) {
    if (piece & 0b11'00'00'00) {
        if (x < 0 || x > 4 || y-1 < 0 || y-1 > 9) {return true;}
        if (field[x + (y-1)*5]) {return true;}
    }
    if (piece & 0b00'11'00'00) {
        if (x+1 < 0 || x+1 > 4 || y < 0 || y > 9) {return true;}
        if (field[(x+1) + y*5]) {return true;}
    }
    if (piece & 0b00'00'11'00) {
        if (x < 0 || x > 4 || y+1 < 0 || y+1 > 9) {return true;}
        if (field[x + (y+1)*5]) {return true;}
    }
    if (piece & 0b00'00'00'11) {
        if (x-1 < 0 || x-1 > 4 || y < 0 || y > 9) {return true;}
        if (field[(x-1) + y*5]) {return true;}
    }
    return false;
}


void update(GameState* gamestate, double input[6]) {
    if (gamestate->state[0] == 0b000000) {new_piece(gamestate->state);}

}
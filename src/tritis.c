#include <tritis.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


uint8_t rotate_tritis_counterclockwise(uint8_t tritis) {
    return tritis<<2 | tritis>>6;
}

uint8_t rotate_tritis_clockwise(uint8_t tritis) {
    return tritis>>2 | tritis<<6;
}

void new_tritis(uint8_t *state) {
    if (state[0]) {state[100 + state[1]+state[2]*5] = 0xFF;}
    if (state[0] & 0b11000000) {state[100 + state[1]+(state[2]-1)*5] = 0xFF;}
    if (state[0] & 0b00110000) {state[100 + (state[1]+1)+state[2]*5] = 0xFF;}
    if (state[0] & 0b00001100) {state[100 + state[1]+(state[2]+1)*5] = 0xFF;}
    if (state[0] & 0b00000011) {state[100 + (state[1]-1)+state[2]*5] = 0xFF;}
    

    if (rand()%2) {
        state[0] = 0b00001111;
    }
    else {
        state[0] = 0b00110011;
    }
    for (int i=0; i<rand()%4; i++) {
        state[0] = rotate_tritis_clockwise(state[0]);
    }
    state[1] = 2;
    state[2] = 0;
}

bool tritis_collided(uint8_t tritis, uint8_t x, uint8_t y, uint8_t field[50]) {
    if (x < 0 || x > 4 || y > 9) {return true;}
    if (field[x + y*5]) {return true;}

    if (tritis & 0b11000000) {
        if (x < 0 || x > 4 || y-1 > 9) {return true;}
        if (field[x + (y-1)*5]) {return true;}
    }
    if (tritis & 0b00110000) {
        if (x+1 < 0 || x+1 > 4 || y > 9) {return true;}
        if (field[(x+1) + y*5]) {return true;}
    }
    if (tritis & 0b00001100) {
        if (x < 0 || x > 4 || y+1 > 9) {return true;}
        if (field[x + (y+1)*5]) {return true;}
    }
    if (tritis & 0b00000011) {
        if (x-1 < 0 || x-1 > 4 || y > 9) {return true;}
        if (field[(x-1) + y*5]) {return true;}
    }
    return false;
}


void clear_tritis_lines(GameState *gamestate) {
    uint8_t *field = &gamestate->state[100];
    for (int i=0; i < 10; i++) {
        uint64_t row_value = 0xffffffffffffffff;
        memcpy(&row_value, &field[i*5], 5);
        if (!~row_value) {
            memmove(field+5, field, i*5);
            gamestate->score += 8;
        }
    }
}


GameState* new_tritis_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    new_tritis(gamestate->state);
    return gamestate;
}

sfRenderTexture *render_tritis(uint8_t *state) {
    sfRenderTexture *r_text = sfRenderTexture_createWithSettings(5*16, 10*16, sfContextDefault);
    sfRenderTexture_clear(r_text, sfColor_fromRGB(128, 128, 128));
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setFillColor(rect, sfGreen);
    sfRectangleShape_setSize(rect, (sfVector2f){12, 12});

    
    for (int i=0; i<50; i++) {
        if (state[100+i]
            || (i == state[1] + state[2] * 5)
            || (state[0] & 0b11000000 && i == state[1] + (state[2]-1) * 5)
            || (state[0] & 0b00110000 && i == (state[1]+1) + state[2] * 5)
            || (state[0] & 0b00001100 && i == state[1] + (state[2]+1) * 5)
            || (state[0] & 0b00000011 && i == (state[1]-1) + state[2] * 5)) {
            sfRectangleShape_setPosition(rect, (sfVector2f){.x=(i%5)*16+2, .y=(i/5)*16+2});
            sfRenderTexture_drawRectangleShape(r_text, rect, NULL);
        }
    }
    sfRenderTexture_display(r_text);
    sfRectangleShape_destroy(rect);
    return r_text;
}


void update_tritis(GameState* gamestate, double input[6]) {
    uint8_t *state = gamestate->state;

    if (round(input[0])) {
        if (!tritis_collided(state[0], state[1]-1, state[2], &state[100])) {state[1]--;}
    }
    if (round(input[1])) {
        if (!tritis_collided(state[0], state[1]+1, state[2], &state[100])) {state[1]++;}
    }
    if (round(input[2])) {
        uint8_t rot_tritis = rotate_tritis_clockwise(state[0]);
        if (!tritis_collided(rot_tritis, state[1], state[2], &state[100])) {state[0] = rot_tritis;}
    }
    if (round(input[3])) {
        uint8_t rot_tritis = rotate_tritis_counterclockwise(state[0]);
        if (!tritis_collided(rot_tritis, state[1], state[2], &state[100])) {state[0] = rot_tritis;}
    }

    state[3] = !state[3];
    if (state[3]) {
        if (!tritis_collided(state[0], state[1], state[2]+1, &state[100])) {
            state[2]++;
        }
        else {
            if (state[1] == 2 && state[2] == 0) {
                gamestate->game_ended = true;
                return;
            }
            new_tritis(gamestate->state);
            clear_tritis_lines(gamestate);
            gamestate->score++;
        }
    }
    for (int i=0; i<50; i++) {
        gamestate->net_in[i] = (state[100+i]
                            || (i == state[1] + state[2] * 5)
                            || (state[0] & 0b11000000 && i == state[1] + (state[2]-1) * 5)
                            || (state[0] & 0b00110000 && i == (state[1]+1) + state[2] * 5)
                            || (state[0] & 0b00001100 && i == state[1] + (state[2]+1) * 5)
                            || (state[0] & 0b00000011 && i == (state[1]-1) + state[2] * 5));
    }
    gamestate->net_in[50] = state[3];
    for (int i=51; i < 64; i++) {gamestate->net_in[i] = 0;}
}


Game *tritis_game_type() {
    return new_game(new_tritis_game, update_tritis, render_tritis, 4.);
}
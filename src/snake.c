#include <snake.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


GameState* new_snake_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    gamestate->state[0] = rand()%8;
    gamestate->state[1] = rand()%8;
    gamestate->state[2] = 3;
    gamestate->state[3] = -1;
    gamestate->state[4] = 0;
    for (int i=0; i < 3; i++) {gamestate->state[5+i*2] = 3+i; gamestate->state[6+i*2] = 4;}
    return gamestate;
}

sfRenderTexture *render_snake(uint8_t *state) {
    sfRenderTexture *r_text = sfRenderTexture_createWithSettings(8*16, 8*16, sfContextDefault);
    sfRenderTexture_clear(r_text, sfColor_fromRGB(128, 128, 128));
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setFillColor(rect, sfGreen);
    sfRectangleShape_setSize(rect, (sfVector2f){12, 12});

    
    for (int i=0; i<state[2]; i++) {
        sfRectangleShape_setPosition(rect, (sfVector2f){.x=state[5+i*2]*16+2, .y=state[6+i*2]*16+2});
        sfRenderTexture_drawRectangleShape(r_text, rect, NULL);
    }
    sfRectangleShape_setPosition(rect, (sfVector2f){.x=state[0]*16+2, .y=state[1]*16+2});
    sfRenderTexture_drawRectangleShape(r_text, rect, NULL);
    sfRenderTexture_display(r_text);
    sfRectangleShape_destroy(rect);
    return r_text;
}


void update_snake(GameState* gamestate, double input[6]) {
    uint8_t *state = gamestate->state;

    if (gamestate->game_ended) {
        return;
    }


    sfVector2i move = {state[3], state[4]};
    if (round(input[0])) {
        move = (sfVector2i){-1, 0};
    }
    if (round(input[1])) {
        move = (sfVector2i){1, 0};
    }
    if (round(input[2])) {
        move = (sfVector2i){0, -1};
    }
    if (round(input[3])) {
        move = (sfVector2i){0, 1};
    }
    state[3] = move.x, state[4] = move.y;


    memmove(&state[7], &state[5], state[2]*2);
    state[5] = state[7] + move.x;
    state[6] = state[8] + move.y;

    if (state[5] == state[0] && state[6] == state[1]) {
        gamestate->score += 32;
        state[2]++;
        state[0] = rand()%8;
        state[1] = rand()%8;
    }


    if (state[5] < 0 || state[5] > 7 || state[6] < 0 || state[6] > 7) {
        gamestate->game_ended = true;
        return;
    }
    for (int i=0; i < state[2]-1; i++) {
        if (state[5] == state[7+i*2] && state[6] == state[8+i*2]) {
            gamestate->game_ended = true;
            return;
        }
    }


    gamestate->score++;
    for (int i=0; i<64; i++) {gamestate->net_in[i] = 0;}
    for (int i=0; i<state[2]; i++) {
        gamestate->net_in[state[5+i*2] + state[6+i*2] * 8] = 1;
    }
    gamestate->net_in[state[0] + state[1] * 8] = 1;
}


Game *snake_game_type() {
    return new_game(new_snake_game, update_snake, render_snake, 2, 8*16, 8*16);
}
#include <lander.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void gen_terrain(uint8_t *state) {
    state[100] = rand()%48;
    for (int i=1; i < 32; i++) {
        int new_height = state[99+i] + (rand()%32 - 16);
        new_height = (new_height > 48) ? 48 : new_height;
        new_height = (new_height < 0) ? 0 : new_height;
        state[100+i] = new_height;
    }
}


GameState* new_lander_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    return gamestate;
    
}

sfRenderTexture *render_lander(uint8_t *state) {
    sfRenderTexture *r_text = sfRenderTexture_createWithSettings(31*16, 256, sfContextDefault);
    sfRenderTexture_clear(r_text, sfColor_fromRGB(128, 128, 128));

    sfConvexShape *terrain = sfConvexShape_create();
    sfConvexShape_setFillColor(terrain, sfGreen);
    sfConvexShape_setPointCount(terrain, 34);
    
    for (int i=0; i < 32; i++) {
        sfConvexShape_setPoint(terrain, i, (sfVector2f){i*16, 256-state[100+i]});
    }
    sfConvexShape_setPoint(terrain, 32, (sfVector2f){31*16, 256});
    sfConvexShape_setPoint(terrain, 33, (sfVector2f){0, 256});

    sfRenderTexture_drawConvexShape(r_text, terrain, NULL);
    sfConvexShape_destroy(terrain);
    sfRenderTexture_display(r_text);
    return r_text;
}


void update_lander(GameState* gamestate, double input[6]) {
    uint8_t *state = gamestate->state;

    // if (gamestate->game_ended) {return;}

    gen_terrain(state);

    
    // sfVector2i move = {state[3], state[4]};
    // if (round(input[0])) {
    //     move = (sfVector2i){-1, 0};
    // }
    // if (round(input[1])) {
    //     move = (sfVector2i){1, 0};
    // }
    // if (round(input[2])) {
    //     move = (sfVector2i){0, -1};
    // }
    // if (round(input[3])) {
    //     move = (sfVector2i){0, 1};
    // }
    // state[3] = move.x, state[4] = move.y;


    // memmove(&state[7], &state[5], state[2]*2);
    // state[5] = state[7] + move.x;
    // state[6] = state[8] + move.y;

    // if (state[5] == state[0] && state[6] == state[1]) {
    //     gamestate->score += 32;
    //     state[2]++;
    //     state[0] = rand()%8;
    //     state[1] = rand()%8;
    // }


    // if (state[5] < 0 || state[5] > 7 || state[6] < 0 || state[6] > 7) {
    //     gamestate->game_ended = true;
    //     return;
    // }
    // for (int i=0; i < state[2]-1; i++) {
    //     if (state[5] == state[7+i*2] && state[6] == state[8+i*2]) {
    //         gamestate->game_ended = true;
    //         return;
    //     }
    // }


    // gamestate->score++;
    // for (int i=0; i<64; i++) {gamestate->net_in[i] = 0;}
    // for (int i=0; i<state[2]; i++) {
    //     gamestate->net_in[state[5+i*2] + state[6+i*2] * 8] = 1;
    // }
    // gamestate->net_in[state[0] + state[1] * 8] = 1;
}


Game *lander_game_type() {
    return new_game(new_lander_game, update_lander, render_lander, 2);
}
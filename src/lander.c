#include <lander.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void gen_terrain(uint8_t *state) {
    state[100] = rand()%32;
    for (int i=1; i < 16; i++) {
        int new_height = state[99+i] + (rand()%16 - 8);
        new_height = (new_height > 32) ? 32 : new_height;
        new_height = (new_height < 0) ? 0 : new_height;
        state[100+i] = new_height;
    }
    int flatn = rand()%10;
    state[101+flatn] = state[100 + flatn];
    state[102+flatn] = state[100 + flatn];
}


GameState* new_lander_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    uint8_t *state = gamestate->state;
    gen_terrain(state);
    state[0] = 15*8/2;
    state[1] = 48;
    state[2] = 128;
    state[3] = 128;
    state[4] = 0;
    return gamestate;
}

sfRenderTexture *render_lander(uint8_t *state) {
    sfRenderTexture *r_text = sfRenderTexture_createWithSettings(15*8, 128, sfContextDefault);
    sfRenderTexture_clear(r_text, sfColor_fromRGB(128, 128, 128));

    sfConvexShape *terrain = sfConvexShape_create();
    sfConvexShape_setFillColor(terrain, sfGreen);
    sfConvexShape_setPointCount(terrain, 18);
    
    for (int i=0; i < 16; i++) {
        sfConvexShape_setPoint(terrain, i, (sfVector2f){i*8, 128-state[100+i]});
    }
    sfConvexShape_setPoint(terrain, 16, (sfVector2f){15*8, 128});
    sfConvexShape_setPoint(terrain, 17, (sfVector2f){0, 128});

    sfConvexShape *lander = sfConvexShape_create();
    sfConvexShape_setFillColor(lander, sfGreen);
    sfConvexShape_setPointCount(lander, 3);
    sfConvexShape_setPoint(lander, 0, (sfVector2f){0, -4});
    sfConvexShape_setPoint(lander, 1, (sfVector2f){4, 4});
    sfConvexShape_setPoint(lander, 2, (sfVector2f){-4, 4});
    sfConvexShape_setPosition(lander, (sfVector2f){state[0], state[1]});
    sfConvexShape_setRotation(lander, (float)state[4]/256*360);


    sfRenderTexture_drawConvexShape(r_text, terrain, NULL);
    sfRenderTexture_drawConvexShape(r_text, lander, NULL);
    sfRenderTexture_display(r_text);
    
    sfConvexShape_destroy(terrain);
    sfConvexShape_destroy(lander);
    return r_text;
}


void update_lander(GameState* gamestate, double input[6]) {
    uint8_t *state = gamestate->state;

    if (gamestate->game_ended) {return;}

    if (round(input[0])) {
        state[4] -= 2;
    }
    if (round(input[1])) {
        state[4] += 2;
    }
    if (round(input[2])) {
        uint8_t d_x = (uint8_t)(sinf((float)state[4]/256*2*3.14) * 12);
        uint8_t d_y = (uint8_t)(cosf((float)state[4]/256*2*3.14) * 12);
        state[2] += state[2] > 255-d_x ? 255-state[3] : d_x;
        state[3] -= state[2] > 255-d_y ? 255-state[3] : d_y;
    }

    state[3] += state[3] > 255-4 ? 255-state[3] : 4;
    state[0] += (state[2]-128)>>4;
    state[1] += (state[3]-128)>>4;


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
    return new_game(new_lander_game, update_lander, render_lander, 30);
}
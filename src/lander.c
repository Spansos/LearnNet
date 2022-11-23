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
    int flatn = (rand()%14) + 1;
    state[100+flatn] = state[101 + flatn];
    state[102+flatn] = state[101 + flatn];
}


GameState* new_lander_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    uint8_t *state = gamestate->state;
    gen_terrain(state);
    state[0] = 15*8/2+32;
    state[1] = 48+32;
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
    sfConvexShape_setPosition(lander, (sfVector2f){state[0]-32, state[1]-32});
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
        state[4] -= 4;
    }
    if (round(input[1])) {
        state[4] += 4;
    }
    if (round(input[2])) {
        int d_x = (sinf((float)state[4]/256*2*3.14) * 12);
        int d_y = (cosf((float)state[4]/256*2*3.14) * 12);
        d_x = state[2]+d_x > 255 ? 255 : d_x;
        d_x = state[2]+d_x < 0 ? 0 : d_x;
        d_y = state[3]-d_y > 255 ? 255 : d_y;
        d_y = state[3]-d_y < 0 ? 0 : d_y;
        state[2] += d_x;
        state[3] -= d_y;
    }

    state[3] += state[3] > 255-4 ? 255-state[3] : 4;
    state[0] += (state[2]-128)>>4;
    state[1] += (state[3]-128)>>4;

    if (state[0]-32 > 120 || state[0]-32 < 0) {gamestate->game_ended = true;}
    if (state[1]-32 > 128 || state[1]-32 < 0) {gamestate->game_ended = true;}

    int index = state[0]/8;
    float d = state[101+index]-state[100+index];

    printf("%i,\t%f,\t%f\n", index, d, (float)-(state[index]-32) + d * ((state[0]-32)%8));
    if ((float)state[index] - d * (float)(state[0]%8) < state[1]) {
        printf("1\n");
    }


    // int index = (state[0]-32-4)/8;
    // index = index < 0 ? 0 : index;
    // index = index > 13 ? 13 : index;
    // uint8_t land_terrain[3] = {state[100+index], state[101+index], state[102+index]};

    // float d1 = land_terrain[0]-land_terrain[1];
    // float d2 = land_terrain[1]-land_terrain[2];
    
    // printf("%i,\t%i,\t%i,\t%i,\t,%lf,\t%lf", index, land_terrain[0], land_terrain[1], land_terrain[2], d1, d2);
    // if (fabsf(d1-d2) < 2 && fabsf(d1) < 8 && fabsf(d2) < 8) {
    //     // gen_terrain(state);
    //     // state[0] = 15*8/2+32;
    //     // state[1] = 48+32;
    //     // state[2] = 128;
    //     // state[3] = 128;
    //     // state[4] = 0;
    //     printf("\tTrue");
    // }
    // printf("\n");

    // gamestate->score++;
    // for (int i=0; i<64; i++) {gamestate->net_in[i] = 0;}
    // for (int i=0; i<state[2]; i++) {
    //     gamestate->net_in[state[5+i*2] + state[6+i*2] * 8] = 1;
    // }
    // gamestate->net_in[state[0] + state[1] * 8] = 1;
}


Game *lander_game_type() {
    return new_game(new_lander_game, update_lander, render_lander, 20);
}
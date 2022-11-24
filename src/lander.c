#include <lander.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


void gen_terrain(uint8_t *state) {
    state[100] = rand()%32;
    for (int i=1; i < 16; i++) {
        int new_height = state[99+i] + (rand()%16 - 8);
        new_height = (new_height > 32) ? 32 : new_height;
        new_height = (new_height < 0) ? 0 : new_height;
        state[100+i] = new_height;
    }
    int flatn = rand()%14;
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
    
    sfVertex *vertices = calloc(16, sizeof(sfVertex));

    for (int i=0; i < 16; i++) {
        vertices[i].color = sfGreen;
        vertices[i].position = (sfVector2f){i*8, 128-state[100+i]};
    }
    sfRenderTexture_drawPrimitives(r_text, vertices, 16, sfLineStrip, NULL);

    sfConvexShape *lander = sfConvexShape_create();
    sfConvexShape_setFillColor(lander, sfGreen);
    sfConvexShape_setPointCount(lander, 3);
    sfConvexShape_setPoint(lander, 0, (sfVector2f){0, -4});
    sfConvexShape_setPoint(lander, 1, (sfVector2f){4, 4});
    sfConvexShape_setPoint(lander, 2, (sfVector2f){-4, 4});
    sfConvexShape_setPosition(lander, (sfVector2f){state[0]-32, state[1]-32});
    sfConvexShape_setRotation(lander, (float)state[4]/256*360);

    sfRenderTexture_drawConvexShape(r_text, lander, NULL);

    sfCircleShape *circle = sfCircleShape_create();
    sfCircleShape_setRadius(circle, 2);
    sfCircleShape_setFillColor(circle, sfRed);
    sfCircleShape_setPosition(circle, (sfVector2f){state[0]-32-2, 128-*(float*)&state[50]-2});

    sfRenderTexture_drawCircleShape(r_text, circle, NULL);

    sfRenderTexture_display(r_text);
    
    free(vertices);
    sfCircleShape_destroy(circle);
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
    if (state[1]-32 < 0) {gamestate->game_ended = true;}

    int index = (state[0]-32)/8;
    float d = (float)(state[101+index]-state[100+index])/8;
    // d = 0;

    // printf("%i,\t%f\n", 128-state[1]+32, (float)(state[100+index]) + d * ((state[0]-32)%8));s
    *(float*)&state[50] = (float)(state[100+index]) + d * ((state[0]-32)%8);
    if ((float)(state[100+index]) + d * ((state[0]-32)%8) > 128 - state[1] + 32 - 4) {
        int index = (state[0]-32-4)/8;
        index = index < 0 ? 0 : index;
        index = index > 13 ? 13 : index;
        uint8_t land_terrain[3] = {state[100+index], state[101+index], state[102+index]};

        float d1 = land_terrain[0]-land_terrain[1];
        float d2 = land_terrain[1]-land_terrain[2];
        if (fabsf(d1-d2) < 2 && fabsf(d1) < 8 && fabsf(d2) < 8 && (state[2]-128) < 16 && (state[3]-128) < 48) {
            gamestate->score += 16;
            gen_terrain(state);
            state[0] = 15*8/2+32;
            state[1] = 48+32;
            state[2] = 128;
            state[3] = 128;
            state[4] = 0;
        }
        else {
            float d_tot = fabsf(d1) + fabsf(d2);
            float speed_tot = (state[2]-128) + (state[3]-128);
            gamestate->score += ((16.0/d_tot)/speed_tot);
            gamestate->game_ended = true;
        }
    }

    for (int i=0; i < 16; i++) {
        gamestate->net_in[i] = (float)state[100+i]/32;
    }
    gamestate->net_in[16] = (float)(state[0]-32)/120;
    gamestate->net_in[17] = (float)(state[1]-32)/128;
    gamestate->net_in[18] = (float)(state[2])/255;
    gamestate->net_in[19] = (float)(state[3])/255;
    gamestate->net_in[20] = (float)(state[4])/255;
    for (int i=21; i<64; i++) {
        gamestate->net_in[i] = 0;
    }
}


Game *lander_game_type() {
    return new_game(new_lander_game, update_lander, render_lander, 20);
}
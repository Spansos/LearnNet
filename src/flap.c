#include <flap.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


GameState* new_flap_game() {
    GameState *gamestate = calloc(1, sizeof(GameState));
    uint8_t *state = gamestate->state;
    *(float*)&state[0] = 32;
    state[9] = 80;
    state[10] = rand()%(64-8*2-24)+8;
    state[11] = 80+72/2;
    state[12] = rand()%(64-8*2-24)+8;
    return gamestate;
}

sfRenderTexture *render_flap(uint8_t *state) {
    sfRenderTexture *r_text = sfRenderTexture_createWithSettings(64, 64, sfContextDefault);
    sfRenderTexture_clear(r_text, sfColor_fromRGB(128, 128, 128));

    sfRectangleShape *bird = sfRectangleShape_create();
    sfRectangleShape_setFillColor(bird, sfGreen);
    sfRectangleShape_setSize(bird, (sfVector2f){6,6});
    sfRectangleShape_setPosition(bird, (sfVector2f){16-3, *(float*)&state[0]});
    
    sfRenderTexture_drawRectangleShape(r_text, bird, NULL);

    sfRectangleShape *top = sfRectangleShape_create();
    sfRectangleShape *bot = sfRectangleShape_create();
    sfRectangleShape_setFillColor(top, sfGreen);
    sfRectangleShape_setFillColor(bot, sfGreen);
    for (int i=0; i < 2; i++) {
        int t_height = state[10+i*2];
        int b_height = 64-t_height-24;
        sfRectangleShape_setSize(top, (sfVector2f){8, t_height});
        sfRectangleShape_setSize(bot, (sfVector2f){8, b_height});
        
        int x = state[9+i*2]-16;
        sfRectangleShape_setPosition(top, (sfVector2f){x, 0});
        sfRectangleShape_setPosition(bot, (sfVector2f){x, 64-b_height});

        sfRenderTexture_drawRectangleShape(r_text, top, NULL);
        sfRenderTexture_drawRectangleShape(r_text, bot, NULL);
    }

    sfRenderTexture_display(r_text);
    
    sfRectangleShape_destroy(bird);
    sfRectangleShape_destroy(top);
    sfRectangleShape_destroy(bot);

    return r_text;
}


void update_flap(GameState* gamestate, double input[6]) {
    uint8_t *state = gamestate->state;

    if (gamestate->game_ended) {
        return;
    }

    if (round(input[2])) {
        if (!state[8]) {
            *(float*)&state[4] = -4;
            state[8] = 1;
        }
    }
    else {
        state[8] = 0;
    }
    *(float*)&state[4] += .5;
    *(float*)&state[0] += *(float*)&state[4];

    if (*(float*)&state[0] > 64-6 || *(float*)&state[0] < 0) {
        gamestate->game_ended = true;
    }

    sfFloatRect bird = {.left = 16-3, .top = *(float*)&state[0], .height=6, .width=6};
    sfFloatRect top = {.left = 0, .top = 0, .height=0, .width=8};
    sfFloatRect bot = {.left = 0, .top = 0, .height=0, .width=8};
    for (int i=0; i < 2; i++) {
        state[9+i*2] -= 2;
        if (state[9+i*2] < 8) {
            state[9+i*2] = 80;
        }
        top.height = state[10+i*2];
        top.top = 0;
        top.left = state[9+i*2] - 16;
        bot.height = 64-state[10+i*2]-24;
        bot.top = 64-bot.height;
        bot.left = state[9+i*2] - 16;

        if (sfFloatRect_intersects(&bird, &top, NULL) || sfFloatRect_intersects(&bird, &bot, NULL)) {
            gamestate->game_ended = true;
        }
    }

    gamestate->score++;

    gamestate->net_in[0] = *(float*)&state[0];
    gamestate->net_in[1] = *(float*)&state[4];
    gamestate->net_in[2] = state[8];
    gamestate->net_in[3] = state[9];
    gamestate->net_in[4] = state[10];
    gamestate->net_in[5] = state[11];
    for (int i=6; i < 64; i++) {gamestate->net_in[i] = 0;}
}


Game *flap_game_type() {
    return new_game(new_flap_game, update_flap, render_flap, 10, 64, 64);
}
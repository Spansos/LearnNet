#include <SFML/Graphics.h>
#include <stdlib.h>
#include <learnnet.h>
#include <NeuralLib/network_t.h>
#include <NeuralLib/arithmatic.h>
#include <flap.h>
#include <lander.h>
#include <snake.h>
#include <tritis.h>

// #include <stdio.h>

Game *games[4];
Game *curGame;
int gamen = -1;

double tot_scores[25];
double cur_scores[25];
LearnNet *nets[25];
GameState *gamestates[25];

void render(sfRenderWindow *window);
void update();
void next_game();
void new_gen();

int main() {
    srand(1);

    games[0] = flap_game_type();
    games[1] = lander_game_type();
    games[2] = snake_game_type();
    games[3] = tritis_game_type();
    curGame = games[0];

    int layersizes[] = {65, 48, 32, 6};
    for (int i=0; i < 25; i++) {
        nets[i] = CreateLearnNet(64, layersizes, 4, NULL);
        nnInitRandom(nets[i]->neuralnet, 2, 2);
        gamestates[i] = curGame->new_game_func();
    }


    sfRenderWindow * window = sfRenderWindow_create((sfVideoMode){64, 64}, "LearnNet", sfDefaultStyle, sfContextDefault);
    next_game(window);

    int framen = 0;
    sfEvent event;
    sfClock *clock = sfClock_create();
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        framen++;
        if (framen > 300) {
            next_game(window);
            framen = 0;
        }

        update();
        render(window);
        sfRenderWindow_display(window);
        

        // sfSleep((sfTime){.microseconds = 1000000 * (1/curGame->framerate - sfTime_asSeconds(sfClock_getElapsedTime(clock)))});
        // sfClock_restart(clock);
    }
}


void next_game(sfRenderWindow *window) {
    gamen++;
    gamen %= 4;
    curGame = games[gamen];

    for (int i=0; i < 25; i++) {
        tot_scores[i] += cur_scores[i];
        cur_scores[i] = 0;

        free_gamestate(gamestates[i]);
        gamestates[i] = curGame->new_game_func();

        ResetNetMemory(nets[i]);
    }
    if (gamen == 0) {
        new_gen();
        for (int i=0; i < 25; i++) {tot_scores[i] = 0;}
    }


    sfRenderWindow_setSize(window, (sfVector2u){curGame->res[0]*5, curGame->res[1]*5});

    sfView *new_view = sfView_create();
    sfView_reset(new_view, (sfFloatRect){.left=0, .top=0, .width=curGame->res[0]*5, .height=curGame->res[1]*5});
    sfRenderWindow_setView(window, new_view);

    sfView_destroy(new_view);
}

void new_gen() {

}


void update() {
    for (int i=0; i < 25; i++) {

        SetLearnNetInput(nets[i], gamestates[i]->net_in, 64, 0);
        double fscore = (double)gamestates[i]->score;
        SetLearnNetInput(nets[i], &fscore, 1, 64);
        
        CalcLearnNet(nets[i]);
        
        double *inputs;
        GetLearnNetOutput(nets[i], &inputs);

        curGame->update_func(gamestates[i], inputs);
        
        free(inputs);

        if (gamestates[i]->game_ended) {
            free_gamestate(gamestates[i]);
            gamestates[i] = curGame->new_game_func();
        }

        cur_scores[i] = cur_scores[i] > fscore ? cur_scores[i] : fscore;
    }
}

void render(sfRenderWindow *window) {
    sfRenderWindow_clear(window, sfBlack);

    sfSprite *sprite = sfSprite_create();
    for (int i=0; i < 25; i++) {
        sfRenderTexture *game_r_text = curGame->render_game(gamestates[i]->state);
        sfTexture *game_text = sfRenderTexture_getTexture(game_r_text);
        sfSprite_setTexture(sprite, game_text, sfFalse);
        sfSprite_setPosition(sprite, (sfVector2f){(i%5)*curGame->res[0], (i/5)*curGame->res[1]});

        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderTexture_destroy(game_r_text);
        // sfTexture_destroy(game_text);
    }
    sfSprite_destroy(sprite);
}
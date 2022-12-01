#include <SFML/Graphics.h>
#include <stdlib.h>
#include <learnnet.h>
#include <flap.h>
#include <lander.h>
#include <snake.h>
#include <tritis.h>

#include <stdio.h>

Game *games[4];
Game *curGame;

LearnNet *nets[100];
GameState *gamestates[100];

void render(sfRenderWindow *window);
void update();

int main() {
    games[0] = flap_game_type();
    games[1] = lander_game_type();
    games[2] = snake_game_type();
    games[3] = tritis_game_type();
    curGame = games[0];

    int layersizes[] = {66, 64, 32, 6};
    for (int i=0; i < 100; i++) {
        nets[i] = CreateLearnNet(64, layersizes, 4, NULL);
        gamestates[i] = curGame->new_game_func();
    }


    sfRenderWindow * window = sfRenderWindow_create((sfVideoMode){64*11, 64*11}, "LearnNet", sfDefaultStyle, sfContextDefault);

    sfEvent event;
    sfClock *clock = sfClock_create();
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        update();

        render(window);
        sfRenderWindow_display(window);
        

        sfSleep((sfTime){.microseconds = 1000000 * (1/curGame->framerate - sfTime_asSeconds(sfClock_getElapsedTime(clock)))});
        sfClock_restart(clock);
    }
}

void update() {
    for (int i=0; i < 100; i++) {
        curGame->update_func(gamestates[i], (double[6]){0, 0, sfKeyboard_isKeyPressed(sfKeySpace), 0, 0, 0});

        if (gamestates[i]->game_ended) {
            free_gamestate(gamestates[i]);
            gamestates[i] = curGame->new_game_func();
        }
    }
}

void render(sfRenderWindow *window) {
    sfRenderWindow_clear(window, sfBlack);

    sfSprite *sprite = sfSprite_create();
    for (int i=0; i < 100; i++) {
        sfRenderTexture *game_r_text = curGame->render_game(gamestates[i]->state);
        sfTexture *game_text = sfRenderTexture_getTexture(game_r_text);
        sfSprite_setTexture(sprite, game_text, sfFalse);
        sfSprite_setPosition(sprite, (sfVector2f){(i%10)*curGame->res[0], (i/10)*curGame->res[1]});

        sfRenderWindow_drawSprite(window, sprite, NULL);

        sfRenderTexture_destroy(game_r_text);
        sfTexture_destroy(game_text);
    }
    sfSprite_destroy(sprite);
}
#include <SFML/Graphics.h>
#include <NeuralLib/network_t.h>
#include <lander.h>


int main() {
    sfRenderWindow * window = sfRenderWindow_create((sfVideoMode){800, 600}, "Test", sfDefaultStyle, sfContextDefault);

    Game *lander = lander_game_type();
    GameState *state = lander->new_game_func();

    sfSprite *sprite = sfSprite_create();
    sfSprite_setPosition(sprite, (sfVector2f){0,0});
    sfEvent event;
    sfClock *clock = sfClock_create();
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        
        lander->update_func(state, (double[]){sfKeyboard_isKeyPressed(sfKeyLeft), sfKeyboard_isKeyPressed(sfKeyRight), sfKeyboard_isKeyPressed(sfKeyUp), sfKeyboard_isKeyPressed(sfKeyDown), 0., 0.});
        sfRenderTexture *game_r_text = lander->render_game(state->state);
        sfTexture *game_text = sfRenderTexture_getTexture(game_r_text);

        sfSprite_setTexture(sprite, game_text, sfFalse);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);

        sfSleep((sfTime){.microseconds = 1000000 * (1/lander->framerate - sfTime_asSeconds(sfClock_getElapsedTime(clock)))});
        sfClock_restart(clock);
    }
}
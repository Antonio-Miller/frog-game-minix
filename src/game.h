#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>
#include <stdint.h>

#define NR_LANES 6
#define SCREEN_HEIGHT 600
#define LANE_HEIGHT 100

/*
 * @brief This function instantiates the elements that compose the game screen.
 * @param x_res The horizontal resolution of the screen space to be used by the game in pixels
 * @param y_res The vertical resolution of the screen space to be used by the game in pixels
 * @return Returns 0 if successful, -1 otherwise
 * @see game.c for the implementation
*/
int game_init(unsigned x_res, unsigned y_res);

/*
 * @brief This functions updates the game elements, such as the lanes, cars, player, etc and draws the screen.
 * @see game.c for the implementation
*/
void game_update();

/*
 * @brief This function updates the game state based on player input
 * @param scancode The scancode of the keyboard key pressed
 * @see game.c for the implementation
*/
void game_handle_input(uint8_t scancode);

/*
 * @brief This functions checks wether the game is over or not by checking if the player is still alive or if the Quit key (ESC) has been pressed
 * @return Return true if the game is over (dead player or ESC key pressed), false otherwise
 * @see game.c for the implementation
*/
bool game_is_over();

#endif // _GAME_H_


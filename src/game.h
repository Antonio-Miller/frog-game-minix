#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>
#include <stdint.h>

#define GAME_TICK 60

void game_init(unsigned x_res, unsigned y_res);
void game_update();
void game_draw();
void game_handle_input(uint8_t scancode);
bool game_is_over();

#endif // _GAME_H_

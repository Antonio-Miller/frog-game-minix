#ifndef _MENU_H
#define _MENU_H
#include "sprite.h"
#include "../sprite_images/menu_background.xpm"
#include "../sprite_images/play.xpm"
#include "../sprite_images/exit.xpm"
#include "../sprite_images/cursor.xpm"
#include "../sprite_images/pointer.xpm"
#include "../sprite_images/instructions.xpm"
#include "../sprite_images/back.xpm"

typedef enum {PLAY, EXIT, NEITHER, INSTRUCTIONS} MenuOption;
void game_draw_menu();
void draw_instructions_screen();
MenuOption select_menu_option();
void destroy_menu();
bool back_button_clicked();
void destroy_instructions();

#endif

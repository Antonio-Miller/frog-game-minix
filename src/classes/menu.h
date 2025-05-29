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
#include "../sprite_images/rules.xpm"

//Menu functions

typedef enum {PLAY, EXIT, NEITHER, INSTRUCTIONS} MenuOption;

/*
 * @brief This function creates the menu elements (buttons, background, etc.) if needed and draws the menu on the screen.
 * @see menu.c for the implementation
*/
void game_draw_menu();

/*
 * @brief This function checks if any of the menu buttons were clicked and returns the corresponding MenuOption.
 * @return Returns PLAY if the play button was clicked, EXIT if the exit button was clicked, NEITHER if no button was clicked, or INSTRUCTIONS if the instructions button was clicked.
 * @see menu.c for the implementation
*/
MenuOption select_menu_option();

/*
 * @brief This function frees the memory allocated for the menu elements and turns all pointers to NULL.
 * @see menu.c for the implementation
*/
void destroy_menu();




//Instructions screen functions

/*
 * @brief This function creates the instructions screen elements (buttons, background, etc.) if needed and draws the it on the screen.
 * @see menu.c for the implementation
*/
void draw_instructions_screen();

/*
 * @brief This function checks if the back button was clicked in the instructions screen.
 * @return Returns true if the back button was clicked, false otherwise.
 * @see menu.c for the implementation
*/
bool back_button_clicked();

/*
 * @brief This function frees the memory allocated for the instructions screen elements and turns all pointers to NULL.
 * @see menu.c for the implementation
*/
void destroy_instructions();

#endif

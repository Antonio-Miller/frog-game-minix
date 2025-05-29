#include "sprite.h"
#include "../sprite_images/frog.xpm"

#ifndef PLAYER
#define PLAYER

typedef struct {
	Sprite *player;
    bool alive;
} Player;

/*
 * @brief This function allocates space and initializes a Player struct.
 * @param x The x-coordinate of the player's position
 * @param y The y-coordinate of the player's position
 * @param xspeed The speed of the player in the x-direction (negative for left movement, positive for right)
 * @param yspeed The speed of the player in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the player (XPM_INDEXED, XPM_8_8_8, etc.)
 * @return Returns a pointer to a Player Struct if it manages to create it, NULL otherwise
 * @see player.c for the implementation
*/
Player *create_player(int x, int y, int xspeed, int yspeed,enum xpm_image_type img_typ);

/*
 * @brief This function frees the memory allocated for a Player struct and turns player into a NULL pointer.
 * @param player Pointer to the Player struct to be destroyed
 * @see player.c for the implementation
*/
void destroy_player(Player *player);

/*
 * @brief This function draws the player on the screen.
 * @param player Pointer to the Player struct to be drawn
 * @return Returns 0 if successful, -1 in case of failure
 * @see player.c for the implementation
*/
int draw_player(Player *player);

/*
 * @brief This function moves the player in the specified x_direction, based on the speed set up when the Player was created.
 * @param player Pointer to the Player struct to be moved
 * @param x_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Returns 0 if successful, -1 in case of failure
 * @see player.c for the implementation
*/
int move_player_x(Player *player, int x_direction);

/*
 * @brief This function moves the player in the specified y_direction, based on the speed set up when the Player was created.
 * @param player Pointer to the Player struct to be moved
 * @param y_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Returns 0 if successful, -1 in case of failure
 * @see player.c for the implementation
*/
int move_player_y(Player *player, int y_direction);

#endif // PLAYER

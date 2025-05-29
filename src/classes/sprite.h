#include <lcom/lcf.h>

#ifndef SPRITE
#define SPRITE

typedef struct {
	int x, y;
	int width, height;
	int xspeed, yspeed;
	uint8_t *pixmap;
	uint32_t transparency_color;
} Sprite;

/*
 * @brief This function allocates space and initializes a Sprite struct.
 * @param x The x-coordinate of the sprite's position
 * @param y The y-coordinate of the sprite's position
 * @param xspeed The speed of the sprite in the x-direction (negative for left movement, positive for right)
 * @param yspeed The speed of the sprite in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the sprite (XPM_INDEXED, XPM_8_8_8, etc.)
 * @return Returns a pointer to a Sprite Struct if it manages to create it, NULL otherwise
 * @see sprite.c for the implementation
*/
Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_typ);

/*
 * @brief This function allocates space and initializes a Sprite struct with a transparency colour, that is, with a colour that's not to be drawn.
 * @param x The x-coordinate of the sprite's position
 * @param y The y-coordinate of the sprite's position
 * @param xspeed The speed of the sprite in the x-direction (negative for left movement, positive for right)
 * @param yspeed The speed of the sprite in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the sprite (XPM_INDEXED, XPM_8_8_8, etc.)
 * @param transparency_color The code of the colour that is to be defined as the transparency color, that is, the color that's not to be drawn (Can't be 0).
 * @return Returns a pointer to a Sprite Struct if it manages to create it, NULL otherwise
 * @see sprite.c for the implementation
*/
Sprite *create_sprite_transparency(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type, uint32_t transparency_color);

/*
 * @brief This function frees the memory allocated for a Sprite struct and turns sprite into a NULL pointer.
 * @param sprite Pointer to the Sprite struct to be destroyed
 * @see sprite.c for the implementation
*/
void destroy_sprite(Sprite *sp);

/*
 * @brief This function draws the sprite on the screen.
 * @param sp Pointer to the Sprite struct to be drawn
 * @return Returns 0 if successful, -1 in case of failure
 * @see sprite.c for the implementation
*/
int draw_sprite(Sprite *sp);

/*
 * @brief This function moves a sprite based only on the speeds that were defined on its creation.
 * @param sp Pointer to the Sprite struct to be moved
 * @return Returns 0 if successful, -1 in case of failure
 * @see sprite.c for the implementation
*/
int move_sprite(Sprite *sp);

/*
 * @brief This function moves a sprite based only on the speeds that were defined on its creation and doesn't allow it to move past a certain limits (xf, yf).
 * @param sp Pointer to the Sprite struct to be moved
 * @param xf x value past which the sprite can't be moved
 * @param yf y value past which the sprite can't be moved
 * @return Returns 0
 * @note Not used in the project, only on the labs
 * @see sprite.c for the implementation
*/
int move_sprite_restricted(Sprite *sp, int xf, int yf);

/*
 * @brief This function moves the sprite in the specified x_direction and y_direction, based on the speeds set up when the Sprite was created.
 * @param sp Pointer to the Sprite struct to be moved
 * @param x_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @param y_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Return 0 if successful, -1 in case of failure
 * @see sprite.c for the implementation
*/
int move_sprite_any(Sprite *sp, int x_direction, int y_direction);

/*
 * @brief This function checks if two sprites collide.
 * @param sp1 Pointer to one of the Sprite structs to be checked
 * @param sp1 Pointer to the other Sprite structs to be checked
 * @return Returns true if the sprites collide, false otherwise (including if either isn't properly initialized)
 * @see sprite.c for the implementation
*/
bool check_collision(Sprite *sp1, Sprite *sp2);

#endif // SPRITE


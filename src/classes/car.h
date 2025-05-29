#include "sprite.h"
#include "player.h"
#include "../sprite_images/red_car_left.xpm"
#include "../sprite_images/red_car_right.xpm"
#include "../sprite_images/blue_car_left.xpm"
#include "../sprite_images/blue_car_right.xpm"
#include "../sprite_images/green_car_left.xpm"
#include "../sprite_images/green_car_right.xpm"

#ifndef CAR
#define CAR

typedef struct
{
	Sprite *car;
} Car;

static xpm_row_t *cars_left_xpm[] = {red_car_left, blue_car_left, green_car_left};
static xpm_row_t *cars_right_xpm[] = {red_car_right, blue_car_right, green_car_right};

/*
 * @brief This function allocates space and initializes a Car struct.
 * @param x The x-coordinate of the car's position
 * @param y The y-coordinate of the car's position
 * @param xspeed The speed of the car in the x-direction (negative for left movement, positive for right)
 * @param yspeed The speed of the car in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the car (XPM_INDEXED, XPM_8_8_8, etc.)
 * @return Returns a pointer to a Car Struct if it manages to create it, NULL otherwise
 * @see car.c for the implementation
*/
Car *create_car(int x, int y, int xspeed, int yspeed, enum xpm_image_type img_typ);

/*
 * @brief This function frees the memory allocated for a Car struct and turns cr into a NULL pointer.
 * @param cr Pointer to the Car struct to be destroyed
 * @see car.c for the implementation
*/
void destroy_car(Car *cr);

/*
 * @brief This function draws the car on the screen.
 * @param cr Pointer to the Car struct to be drawn
 * @return Returns 0 if successful, -1 in case of failure
 * @see car.c for the implementation
*/
int draw_car(Car *cr);

/*
 * @brief This function moves the car in the specified directions, based on the speeds set up when the Car was created.
 * @param x_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @param y_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Returns 0 if successful, -1 in case of failure
 * @see car.c for the implementation
*/
int move_car(Car *cr, int x_direction, int y_direction);

/*
 * @brief This function checks if the car is out of bounds of the screen.
 * @param cr Pointer to the Car struct to be checked
 * @param h_res Horizontal resolution of the screen
 * @param v_res Vertical resolution of the screen
 * @return Returns true if the car is out of bounds or wasn't properly initialized, false otherwise
 * @see car.c for the implementation
*/
bool car_out_of_bounds(Car *cr, int h_res, int v_res);

/*
 * @brief This function checks if the car collides with the player.
 * @param cr Pointer to the Car struct to be checked
 * @param player Pointer to the Player struct to be checked
 * @return Returns true if the car collides with the player, false otherwise (including if either isn't properly initialized)
 * @see car.c for the implementation
*/
bool car_collides(Car *cr, Player *player);

#endif // CAR

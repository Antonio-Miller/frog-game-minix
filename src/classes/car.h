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

Car *create_car(int x, int y, int xspeed, int yspeed, enum xpm_image_type img_typ);
void destroy_car(Car *cr);
int draw_car(Car *cr);
/*
@param x_direction can be -1 (up), 0 (don't move), or 1 (down)
@param y_direction can be -1 (left), 0 (don't move), or 1 (right)
*/
int move_car(Car *cr, int x_direction, int y_direction);
bool car_out_of_bounds(Car *cr, int h_res, int v_res);
bool car_collides(Car *cr, Player *player);

#endif // CAR

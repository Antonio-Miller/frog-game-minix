#ifndef POINTS
#define POINTS

#include <stdint.h>
#include "sprite.h"
#include "../sprite_images/points.xpm"

typedef struct {
	Sprite *points_sprite;
    int points;
} Points;

Points points;

/*
 * @brief This function allocates space and initializes a global Points struct.
 * @see points.c for the implementation
*/
void points_init();

/*
 * @brief This function increments the points value by one.
 * @see points.c for the implementation
*/
void points_up();

/*
 * @brief This function decrements the points value by one.
 * @see points.c for the implementation
*/
void points_down();

/*
 * @brief This function draws the points.
 * @see points.c for the implementation
*/
void draw_points();

/*
 * @brief This function frees the memory allocated to the points sprite.
 * @see points.c for the implementation
*/
void destroy_points();

static xpm_row_t* digits_xpm[] = {
    zero, one, two, three, four, five, six, seven, eight, nine
};

#endif // POINTS

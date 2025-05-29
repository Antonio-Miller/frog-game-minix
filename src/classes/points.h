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

void points_init();
void points_up();
void points_down();
void draw_points();
void destroy_points();

static xpm_row_t* digits_xpm[] = {
    zero, one, two, three, four, five, six, seven, eight, nine
};

#endif // POINTS

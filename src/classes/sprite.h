#include <lcom/lcf.h>

#ifndef SPRITE
#define SPRITE

typedef struct {
	int x, y;
	int width, height;
	int xspeed, yspeed;
	uint8_t *pixmap;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_typ);
void destroy_sprite(Sprite *sp);
int draw_sprite(Sprite *sp);
int move_sprite(Sprite *sp);
int move_sprite_restricted(Sprite *sp, int xf, int yf);
/*
@param x_direction can be -1 (up), 0 (don't move), or 1 (down)
@param y_direction can be -1 (left), 0 (don't move), or 1 (right)
*/
int move_sprite_any(Sprite *sp, int x_direction, int y_direction);

#endif // SPRITE


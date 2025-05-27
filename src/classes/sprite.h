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

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_typ);
Sprite *create_sprite_transparency(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type, uint32_t transparency_color);
void destroy_sprite(Sprite *sp);
int draw_sprite(Sprite *sp);
int move_sprite(Sprite *sp);
int move_sprite_restricted(Sprite *sp, int xf, int yf);
/*
@param x_direction can be -1 (up), 0 (don't move), or 1 (down)
@param y_direction can be -1 (left), 0 (don't move), or 1 (right)
*/
int move_sprite_any(Sprite *sp, int x_direction, int y_direction);
bool check_collision(Sprite *sp1, Sprite *sp2);

#endif // SPRITE


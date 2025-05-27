#include <lcom/lcf.h>

#include "sprite.h"
#include "../I_O/video_card.h"

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type) {
	Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
	xpm_image_t img;
	if(sp == NULL)
		return NULL;

	sp->pixmap = xpm_load(xpm, img_type, &img);
	if(sp->pixmap == NULL) {
		free(sp);
		return NULL;
	}
	sp->width = img.width;
	sp->height = img.height;
	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
	sp->transparency_color = 0;

	return sp;
}

Sprite *create_sprite_transparency(xpm_map_t xpm, int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type, uint32_t transparency_color) {
	Sprite *sp = create_sprite(xpm, x, y, xspeed, yspeed, img_type);
	if(sp == NULL)
		return NULL;

	sp->transparency_color = transparency_color;

	return sp;
}

void destroy_sprite(Sprite *sp) {
	if(sp == NULL)
		return;
	if(sp->pixmap)
		free(sp->pixmap);
	free(sp);
	sp = NULL;
}

int draw_sprite(Sprite *sp) {
	if(sp == NULL || sp->pixmap == NULL) {
		return -1;
	}

	if(sp->transparency_color == 0) {
		vg_draw_xpm(sp->x, sp->y, sp->width, sp->height, sp->pixmap);
	}
	else {
		vg_draw_xpm_with_transparency(sp->x, sp->y, sp->width, sp->height, sp->pixmap, sp->transparency_color);
	}
	
	return 0;
}


int move_sprite_restricted(Sprite *sp, int xf, int yf) {
	if(sp->x + sp->xspeed <= xf && sp->xspeed > 0)
		move_sprite(sp);
	else {
		sp->x = xf;
		draw_sprite(sp);
	}

	if(sp->y + sp->yspeed <= yf && sp->yspeed > 0)
		move_sprite(sp);
	else {
		sp->y = yf;
		draw_sprite(sp);
	}

	return 0;
}

int move_sprite(Sprite *sp) {
	if(sp->xspeed > 0)
		sp->x += sp->xspeed;
	else
		sp->x++;

	if(sp->yspeed > 0)
		sp->y += sp->yspeed;
	else
		sp->y++;

	draw_sprite(sp);
	return 0;
}

int move_sprite_any(Sprite *sp, int x_direction, int y_direction) {
	if(sp == NULL || sp->pixmap == NULL)
		return -1;

	int new_x = sp->x + sp->xspeed * x_direction;
	int new_y = sp->y + sp->yspeed * y_direction;

	if (new_x < 0)
		new_x = 0;

	else if (new_x + sp->width > (int32_t) get_h_res())
		new_x = get_h_res() - sp->width;

	if (new_y < 0)
		new_y = 0;

	else if (new_y + sp->height > (int32_t) get_v_res())
		new_y = get_v_res() - sp->height;

	sp->x = new_x;
	sp->y = new_y;

	return 0;
}

bool check_collision(Sprite *sp1, Sprite *sp2) {
	if (sp1 == NULL || sp2 == NULL)
		return false;

	int x1 = sp1->x;
	int y1 = sp1->y;
	int x2 = sp2->x;
	int y2 = sp2->y;

	return (x1 < x2 + sp2->width && x1 + sp1->width > x2 && y1 < y2 + sp2->height && y1 + sp1->height > y2);
}
//static int check_collision(Sprite *sp);


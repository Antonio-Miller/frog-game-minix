#include <lcom/lcf.h>

#include "sprite.h"
#include "video_card.h"

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
	uint16_t y = sp->y;
	uint16_t x = sp->x;
	uint16_t abs_final_y = y + sp->height;
	uint16_t abs_final_x = x + sp->width;
	uint32_t pos = 0;

	while(y < abs_final_y){
		for(int i = x; i < abs_final_x; i++){
			vg_draw_pixel(i, y, *(sp->pixmap + pos));
			pos++;
		}
		y++;
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
	if(sp->xspeed > 0){
		sp->x += sp->xspeed;
	}
	else {
		sp->x++;

	if(sp->yspeed > 0) {
		sp->y += sp->yspeed;
	}
	else
		sp->y++;
	}

	draw_sprite(sp);
	return 0;
}

//static int check_collision(Sprite *sp);

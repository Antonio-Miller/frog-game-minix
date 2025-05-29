#include <lcom/lcf.h>
#include "car.h"

Car *create_car(int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type){
	Car *result = (Car *) malloc(sizeof(Car));

	if(result == NULL)
		return NULL;

	if (xspeed >= 0) { //right direction
		int sprite_ind = rand() % 3; //Choose between the three available right-facing car sprites
		result->car = create_sprite(cars_right_xpm[sprite_ind], x, y, xspeed, yspeed, img_type);
	} else { //left direction
		int sprite_ind = rand() % 3; //Choose between the three available left-facing car sprites
		result->car = create_sprite(cars_left_xpm[sprite_ind], x, y, xspeed, yspeed, img_type);
	}

	if(result->car == NULL) {
		free(result);
		return NULL;
	}

	return result;
}

void destroy_car(Car *cr) {
	if(cr == NULL)
		return;

	if(cr->car) {
		destroy_sprite(cr->car);
	}
	free(cr);
	cr = NULL;
}

int draw_car(Car *cr) {
	if(cr == NULL || cr->car == NULL) {
		return -1;
	}
	draw_sprite(cr->car);
	return 0;
}

int move_car(Car *cr, int x_direction, int y_direction) {
	if(cr == NULL || cr->car == NULL) {
		return -1;
	}
	move_sprite_any(cr->car, x_direction, y_direction);
	return 0;
}

bool car_out_of_bounds(Car *cr, int h_res, int v_res) {
	if(cr == NULL || cr->car == NULL) {
		return true; // If the car is not initialized, consider it out of bounds
	}
	int x = cr->car->x;
	int y = cr->car->y;
	int width = cr->car->width;
	int height = cr->car->height;

	return (x <= 0 || x + width >= h_res || y  <= 0 || y + height >= v_res);
}

bool car_collides(Car *cr, Player *player) {
	if(cr == NULL || player == NULL) {
		return false; // If either car or player is not initialized, no collision
	}
	return check_collision(cr->car, player->player);
}


#include <lcom/lcf.h>
#include "car.h"

Car *create_car(int x, int y, int xspeed, int yspeed, enum xpm_image_type img_type){
	Car *result = (Car *) malloc(sizeof(Car));

	if(result == NULL)
		return NULL;

	result->car = create_sprite(car_xpm, x, y, xspeed, 0, img_type);

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

	return (x < 0 || x + width > h_res - 1 || y  < 0 || y + height > v_res - 1);
}

bool car_collides(Car *cr, Player *player) {
	if(cr == NULL || cr->car == NULL || player == NULL || player->player == NULL) {
		return false; // If either car or player is not initialized, no collision
	}
	int car_x = cr->car->x;
	int car_y = cr->car->y;
	int car_width = cr->car->width;
	int car_height = cr->car->height;

	int player_x = player->player->x;
	int player_y = player->player->y;
	int player_width = player->player->width;
	int player_height = player->player->height;

	return (car_x < player_x + player_width &&
			car_x + car_width > player_x &&
			car_y < player_y + player_height &&
			car_y + car_height > player_y);
}


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
	draw_sprite(cr->car);
	return 0;
}

int move_car(Car *cr) {
	move_sprite(cr->car);
	return 0;
}

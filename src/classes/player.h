#include "sprite.h"
#include "../sprite_images/frog.xpm"

#ifndef PLAYER
#define PLAYER

typedef struct {
	Sprite *player;
    bool alive;
} Player;

Player *create_player(int x, int y, int xspeed, int yspeed,enum xpm_image_type img_typ);
void destroy_player(Player *player);
int draw_player(Player *player);
/*
@param x_direction can be -1 (left), 0 (don't move), or 1 (right)
*/
int move_player_x(Player *player, int x_direction);
int move_player_y(Player *player, int y_direction);

#endif // PLAYER

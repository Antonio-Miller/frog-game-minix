#include "sprite.h"
#include "car.h"
#include "tunnel.h"
#include "../sprite_images/lane.xpm"

#ifndef LANE
#define LANE

#define CAR_WIDTH 100
#define TUNNEL_WIDTH 100
#define SCREEN_WIDTH 800

typedef struct {
    uint8_t index;
    Sprite *lane;
    int last_car;
    Car *car[3];
    Tunnel *tunnels[2]; //Two tunnels, one for each side, mainly to hide car destruction and generation
    enum xpm_image_type img_type;
} Lane;

Lane *create_lane(int lane_index, int x, int y, int yspeed, enum xpm_image_type img_typ);
void destroy_lane(Lane *lane);
int draw_lane(Lane *lane);

/*
@param y_direction can be -1 (up), 0 (don't move), or 1 (down)
*/
int move_lane(Lane *lane, int y_direction);
int update_lane(Lane *lane, Player *player);

#endif // LANE

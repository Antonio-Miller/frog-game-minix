#include "sprite.h"
#include "sprite_images/tunnel.xpm"

#ifndef TUNNEL
#define TUNNEL

typedef struct {
    Sprite *tunnel;
} Tunnel;

Tunnel *create_tunnel(int x, int y, int speed, enum xpm_image_type img_typ);
void destroy_tunnel(Tunnel *tunnel);
int draw_tunnel(Tunnel *tunnel);

/*
@param y_direction can be -1 (up), 0 (don't move), or 1 (down)
*/
int move_tunnel(Tunnel *tunnel, int y_direction);

#endif // TUNNEL

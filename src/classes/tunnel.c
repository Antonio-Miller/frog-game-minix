#include <lcom/lcf.h>
#include "tunnel.h"

Tunnel *create_tunnel(int x, int y, int speed, enum xpm_image_type img_typ) {
    Tunnel *result = (Tunnel *) malloc(sizeof(Tunnel));

    if (result == NULL)
        return NULL;

    result->tunnel = create_sprite(tunnel_xpm, x, y, speed, 0, img_typ);

    if (result->tunnel == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

void destroy_tunnel(Tunnel *tunnel) {
    if (tunnel == NULL)
        return;

    if (tunnel->tunnel) {
        destroy_sprite(tunnel->tunnel);
    }
    free(tunnel);
    tunnel = NULL;
}

int draw_tunnel(Tunnel *tunnel) {
    if (tunnel == NULL || tunnel->tunnel == NULL) {
        return -1;
    }
    draw_sprite(tunnel->tunnel);
    return 0;
}

int move_tunnel(Tunnel *tunnel, int x_direction) {
    move_sprite_any(tunnel->tunnel, x_direction, 0);
    return 0;
}

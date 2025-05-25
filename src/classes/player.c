#include <lcom/lcf.h>
#include "player.h"

Player *create_player(int x, int y, int xspeed, enum xpm_image_type img_typ) {
    Player *result = (Player *) malloc(sizeof(Player));

    if (result == NULL)
        return NULL;

    result->player = create_sprite(frog_xpm, x, y, xspeed, 0, img_typ);

    if (result->player == NULL) {
        free(result);
        return NULL;
    }

    result->alive = true;

    return result;
}

void destroy_player(Player *player) {
    if (player == NULL)
        return;

    if (player->player) {
        destroy_sprite(player->player);
    }
    free(player);
    player = NULL;
}

int draw_player(Player *player) {
    if (player == NULL || player->player == NULL) {
        return -1;
    }
    draw_sprite(player->player);
    return 0;
}

int move_player_x(Player *player, int x_direction){
    if (player == NULL || player->player == NULL) {
        return -1;
    }
    move_sprite_any(player->player, x_direction, 0);
    return 0;
}

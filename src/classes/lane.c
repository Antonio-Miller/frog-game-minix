#include <lcom/lcf.h>
#include "lane.h"
#include "../I_O/video_card.h"

static int generate_car(Lane *lane, int index) {
    if (lane == NULL || lane->lane == NULL) {
        return -1;
    }

    if (lane->car[index] != NULL) {
        return 0; // A car already exists, no need to generate a new one
    }

    int base_min_distance = CAR_WIDTH;
    int random_offset = rand() % CAR_WIDTH;
    int min_distance = base_min_distance + random_offset; // Min distance varies between CAR_WIDTH and 2*CAR_WIDTH

    bool moves_left = lane->index % 2;   //If even, generate on left, else on right

    int x = (SCREEN_WIDTH - CAR_WIDTH) * moves_left;

    if (index != 0 && lane->car[index - 1] != NULL) {   //Check if cars are too close, in case it exists
        int prev_x = lane->car[index - 1]->car->x;
        int new_x = x;

        if (moves_left) {   //Check left moving cars
            if (prev_x - new_x < min_distance) {
                return 0; // Too close
            }
        }
        else {  //Check right moving cars
            if (new_x - prev_x < min_distance) {
                return 0; // Too close
            }
        }
}


    int base_probability = 20;
    int lane_probability = lane->index * 5;
    int final_probability = base_probability + lane_probability;
    int generate_or_not = rand() % 100;

    if(generate_or_not < final_probability || index == 0) {
        int base_x_speed = 2;
        int lane_ind_based_speed = lane->index / 2; // The further from the "initial" lane, the faster the car;
        int speed = base_x_speed + lane_ind_based_speed;

        if(moves_left) {
            speed = -speed; // If the lane is odd, the car goes left
        }

        lane->car[index] = create_car(x, lane->lane->y + 6, speed, lane->lane->yspeed, lane->img_type);
        if (lane->car[index] == NULL) {
            return -1;
        }
        lane->last_car++;
    }
    return 0;
}

Lane *create_lane(int lane_index, int x, int y, int yspeed, enum xpm_image_type img_typ) {
    Lane *result = (Lane *) malloc(sizeof(Lane));

    if (result == NULL)
        return NULL;

    result->lane = create_sprite(lane_xpm, x, y, 0, yspeed, img_typ);
    result->img_type = img_typ;

    if (result->lane == NULL) {
        free(result);
        return NULL;
    }

    result->tunnels[0] = create_tunnel(0, y, yspeed, img_typ);
    result->tunnels[1] = create_tunnel(SCREEN_WIDTH - TUNNEL_WIDTH, y, yspeed, img_typ);
    if(result->tunnels[0] == NULL || result->tunnels[1] == NULL) {
        destroy_lane(result);
        return NULL;
    }

    result->car[0] = NULL;
    result->car[1] = NULL;
    result->car[2] = NULL;
    result->last_car = 0;
    result->index = lane_index;

    return result;
}

void destroy_lane(Lane *lane) {
    if (lane == NULL)
        return;

    if (lane->lane != NULL) {
        destroy_sprite(lane->lane);
    }

    if (lane->tunnels[0] != NULL) {
        destroy_tunnel(lane->tunnels[0]);
    }

    if (lane->tunnels[1] != NULL) {
        destroy_tunnel(lane->tunnels[1]);
    }

    for(int i = 0; i < lane->last_car; i++) {
        if (lane->car[i] != NULL) {
            destroy_car(lane->car[i]);
        }
    }

    free(lane);
    lane = NULL;
}

int draw_lane(Lane *lane) {
    if (lane == NULL || lane->lane == NULL) {
        return -1;
    }
    draw_sprite(lane->lane);
    for (int i = 0; i < lane->last_car; i++) {
        if (lane->car[i] != NULL) {
            draw_car(lane->car[i]);
        }
    }
    draw_tunnel(lane->tunnels[0]);
    draw_tunnel(lane->tunnels[1]);
    return 0;
}

int move_lane(Lane *lane, int y_direction) {
    if (lane == NULL || lane->lane == NULL) {
        return -1;
    }
    move_sprite_any(lane->lane, 0, y_direction);
    move_tunnel(lane->tunnels[0], y_direction);
    move_tunnel(lane->tunnels[1], y_direction);
    for(int i = 0; i < lane->last_car; i++) {
        if (lane->car[i] != NULL) {
            move_car(lane->car[i], 0, y_direction);
        }
    }
    return 0;
}

int update_lane(Lane *lane, Player *player) {
    if(lane == NULL || lane->lane == NULL) {
        return -1;
    }

    for(int i = 0; i < lane->last_car; i++) {
        if (lane->car[i] != NULL) {
            move_car(lane->car[i], 1, 0);
        }

        if(car_collides(lane->car[i], player)) {
            player->alive = false; // Player collides with car, set alive to false
        }
    }

    if (lane->car[0] != NULL && car_out_of_bounds(lane->car[0], get_h_res(), get_v_res())) {
        destroy_car(lane->car[0]); // Remove the car if it goes out of bounds
        lane->car[0] = lane->car[1]; // Shift the cars left
        lane->car[1] = lane->car[2];
        lane->car[2] = NULL;
        lane->last_car--;
    }

    for(int i = 0; i < 3; i++) {
        if (lane->car[i] == NULL) {
            generate_car(lane, i); // Generate a new car if there's space
            break;  // No reason to generate another after since the position on the screen will be filled
        }
    }

    return 0;
}

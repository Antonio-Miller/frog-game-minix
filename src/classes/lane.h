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

/*
 * @brief This function allocates space and initializes a Lane struct with all its elements.
 * @param lane_index The index of the lane
 * @param x The x-coordinate of the lane's position
 * @param y The y-coordinate of the lane's position
 * @param yspeed The speed of the lane in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the lane (XPM_INDEXED, XPM_8_8_8, etc.)
 * @return Returns a pointer to a Lane Struct if it manages to create it, NULL otherwise
 * @see lane.c for the implementation
*/
Lane *create_lane(int lane_index, int x, int y, int yspeed, enum xpm_image_type img_typ);

/*
 * @brief This function frees the memory allocated for a Lane struct and turns lane into a NULL pointer.
 * @param lane Pointer to the Lane struct to be destroyed
 * @see lane.c for the implementation
*/
void destroy_lane(Lane *lane);

/*
 * @brief This function draws the lane on the screen.
 * @param lane Pointer to the Lane struct to be drawn
 * @return Returns 0 if successful, -1 in case of failure
 * @see lane.c for the implementation
*/
int draw_lane(Lane *lane);

/*
 * @brief This function moves the lane and its elements in the specified y_direction, based on the speed set up when the Lane was created.
 * @param y_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Returns 0 if successful, -1 in case of failure
 * @see lane.c for the implementation
*/
int move_lane(Lane *lane, int y_direction);

/*
 * @brief This function updates the lane, checking if it needs to generate a new car, if it needs to remove an old one and collisions witht the player.
 * @param lane Pointer to the Lane struct to be updated
 * @param player Pointer to the Player struct, used to check if the player collides with any car
 * @return Returns 0 if successful, -1 in case of failure
 * @see lane.c for the implementation
*/
int update_lane(Lane *lane, Player *player);

#endif // LANE

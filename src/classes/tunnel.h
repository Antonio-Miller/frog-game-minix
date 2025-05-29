#include "sprite.h"
#include "sprite_images/tunnel.xpm"

#ifndef TUNNEL
#define TUNNEL

typedef struct {
    Sprite *tunnel;
} Tunnel;

/*
 * @brief This function allocates space and initializes a Tunnel struct.
 * @param x The x-coordinate of the Tunnel's position
 * @param y The y-coordinate of the Tunnel's position
 * @param speed The speed of the tunnel in the y-direction (negative for up movement, positive for down)
 * @param img_typ The type of image to use for the tunnel (XPM_INDEXED, XPM_8_8_8, etc.)
 * @return Returns a pointer to a Tunnel Struct if it manages to create it, NULL otherwise
 * @see tunnel.c for the implementation
*/
Tunnel *create_tunnel(int x, int y, int speed, enum xpm_image_type img_typ);

/*
 * @brief This function frees the memory allocated for a Tunnel struct and turns tunnel into a NULL pointer.
 * @param tunnel Pointer to the Tunnel struct to be destroyed
 * @see tunnel.c for the implementation
*/
void destroy_tunnel(Tunnel *tunnel);

/*
 * @brief This function draws the tunnel on the screen.
 * @param tunnel Pointer to the Tunnel struct to be drawn
 * @return Returns 0 if successful, -1 in case of failure
 * @see tunnel.c for the implementation
*/
int draw_tunnel(Tunnel *tunnel);

/*
 * @brief This function moves the tunnel in the specified y_direction, based on the speed set up when the Tunnel was created.
 * @param y_direction can be -1 (reversed), 0 (don't move), or 1 (normal)
 * @return Returns 0 if successful, -1 in case of failure
 * @see tunnel.c for the implementation
*/
int move_tunnel(Tunnel *tunnel, int y_direction);

#endif // TUNNEL

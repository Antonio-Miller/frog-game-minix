#include "sprite.h"

#ifndef PLAYER
#define PLAYER

typedef struct {
	Sprite *player;
    bool alive;
} Player;

Player *create_player(int x, int y, int xspeed, enum xpm_image_type img_typ);
void destroy_player(Player *player);
int draw_player(Player *player);
/*
@param x_direction can be -1 (left), 0 (don't move), or 1 (right)
*/
int move_player_x(Player *player, int x_direction);

static xpm_row_t frog_xpm[] = {
"32 32 5 1",
" 	c #5E5E5E",
".	c #000000",
"+	c #009700",
"@	c #FFFFFF",
"#	c #BAD318",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"           ..      ..           ",
"          .++.    .++.          ",
"         .+@@+....+@@+.         ",
"         .@..@++++@..@.         ",
"         .@..@++++@..@.         ",
"         .@@@@++++@@@@.         ",
"        .++++++++++++++.        ",
"        .+.++++++++++.+.        ",
"        .++.++++++++.++.        ",
"        .++#........#++.        ",
"         .+##########+.         ",
"         .+##########+.         ",
"         .+##########+.         ",
"        ..+##########+..        ",
"      ..+.+.########.+.+..      ",
"     .+++.+.########.+.+++.     ",
"     .+++.+.########.+.+++.     ",
"     .+++.+..######..+.+++.     ",
"      ..+.++..####..++.+..      ",
"       ..+.+++.##.+++.+..       ",
"      .+++..++.++.++..+++.      ",
"      ....................      ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                "};
#endif // PLAYER

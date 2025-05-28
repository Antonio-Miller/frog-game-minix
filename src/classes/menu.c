#include <lcom/lcf.h>
#include "sprite.h"
#include "menu.h"

static Sprite *background = NULL;
static Sprite *pointer = NULL;
static Sprite *cursor = NULL;
static Sprite *play_button = NULL;
static Sprite *exit_button = NULL;
static Sprite *to_draw = NULL;
static Sprite *instructions_button = NULL;
static Sprite *back_button = NULL;
extern int mouse_x, mouse_y;

static void create_cursor(int x, int y, enum xpm_image_type img_type) {
  cursor = create_sprite_transparency(cursor_xpm, x, y, 0, 0, img_type, 0xFF00FF);
}

static void create_pointer(int x, int y, enum xpm_image_type img_type) {
  pointer = create_sprite_transparency(pointer_xpm, x, y, 0, 0, img_type, 0xFF00FF);
}

static void create_background(enum xpm_image_type img_type) {
  background = create_sprite(menu_background_xpm, 0, 0, 0, 0, img_type);
}

static void create_play_button(int x, int y, enum xpm_image_type img_type) {
  play_button = create_sprite(play_xpm, x, y, 0, 0, img_type);
}

static void create_exit_button(int x, int y, enum xpm_image_type img_type) {
  exit_button = create_sprite(exit_xpm, x, y, 0, 0, img_type);
}

static void create_instructions_button(int x, int y, enum xpm_image_type img_type) {
  instructions_button = create_sprite(instructions_xpm, x, y, 0, 0, img_type);
}

static void create_back_button(int x, int y, enum xpm_image_type img_type) {
  back_button = create_sprite(back_xpm, x, y, 0, 0, img_type);
}

static void draw_menu_background() {
  if (background == NULL) 
    create_background(XPM_8_8_8);
  
  draw_sprite(background);
}





//We will probably have to create a mouse "class" to decide this
static void draw_menu_mouse() {
  if (pointer == NULL) 
    create_pointer(mouse_x, mouse_y, XPM_8_8_8);

  if (cursor == NULL) 
    create_cursor(mouse_x, mouse_y, XPM_8_8_8);

  bool over_play = mouse_x >= play_button->x && mouse_x <= play_button->x + play_button->width && mouse_y >= play_button->y && mouse_y <= play_button->y + play_button->height;
  bool over_exit = mouse_x >= exit_button->x && mouse_x <= exit_button->x + exit_button->width && mouse_y >= exit_button->y && mouse_y <= exit_button->y + exit_button->height;
  bool over_instructions = mouse_x >= instructions_button->x && mouse_x <= instructions_button->x + instructions_button->width && mouse_y >= instructions_button->y && mouse_y <= instructions_button->y + instructions_button->height;


  to_draw = (over_play || over_exit || over_instructions) ? pointer : cursor;
  to_draw->x = mouse_x;
  to_draw->y = mouse_y;
  draw_sprite(to_draw);
}

void draw_instructions_screen() {
  draw_sprite(background); 
  if (back_button == NULL)
    create_back_button(350, 520, XPM_8_8_8); 
  draw_sprite(back_button);

  bool over_back = mouse_x >= back_button->x && mouse_x <= back_button->x + back_button->width &&
                   mouse_y >= back_button->y && mouse_y <= back_button->y + back_button->height;

  Sprite *mouse_sprite = over_back ? pointer : cursor;
  if (mouse_sprite == NULL) {
    if (over_back)
      create_pointer(mouse_x, mouse_y, XPM_8_8_8);
    else
      create_cursor(mouse_x, mouse_y, XPM_8_8_8);
    mouse_sprite = over_back ? pointer : cursor;
  }
  mouse_sprite->x = mouse_x;
  mouse_sprite->y = mouse_y;
  draw_sprite(mouse_sprite);
}

static void draw_play_button() {
  if (play_button == NULL)
    create_play_button(200, 225, XPM_8_8_8);
  
  draw_sprite(play_button);
}

static void draw_exit_button() {
  if (exit_button == NULL) 
    create_exit_button(400, 325, XPM_8_8_8);
  
  draw_sprite(exit_button);
}

static void draw_instructions_button() {
  if (instructions_button == NULL)
    create_instructions_button(250, 425, XPM_8_8_8); 
  draw_sprite(instructions_button);
}

void game_draw_menu() {
  draw_menu_background();
  draw_play_button();
  draw_exit_button();
  draw_instructions_button();
  draw_menu_mouse();
}

MenuOption select_menu_option() {
  if (check_collision(to_draw, play_button))  
    return PLAY;
  else if (check_collision(to_draw, instructions_button))
    return INSTRUCTIONS;
  else if (check_collision(to_draw, exit_button)) 
    return EXIT;

  return NONE;
}

bool back_button_clicked() {
  return mouse_x >= back_button->x && mouse_x <= back_button->x + back_button->width &&
         mouse_y >= back_button->y && mouse_y <= back_button->y + back_button->height;
}

#include "I_O/video_card.h"
#include <lcom/lcf.h>
#include"sprite.h"
#include"../sprite_images/pointer.xpm"
#include"../sprite_images/tool.xpm"
#include"../sprite_images/cursor.xpm"
#include"../sprite_images/title.xpm"
#include"../sprite_images/play.xpm"
#include"../sprite_images/exit.xpm"
#include"../sprite_images/Untitled.xpm"





extern int mouse_x, mouse_y;
extern unsigned char *video_mem;


Sprite *create_mouse_pointer(int x, int y, enum xpm_image_type img_type) {

  Sprite *mouse = create_sprite((xpm_map_t)pointer_scifi_a_xpm, x, y, 0, 0, img_type);
  //Sprite *mouse = create_sprite((xpm_map_t)mouseCursor_xpm, x, y, 0, 0, img_type);

    if (mouse == NULL) {
        return NULL;
    }
    return mouse;
}
Sprite *create_cursor(int x, int y, enum xpm_image_type img_type) {
  Sprite *cursor = create_sprite((xpm_map_t)tool_sword_xpm, x, y, 0, 0, img_type);
  if (cursor == NULL) {
    return NULL;
  }
  return cursor;
}

Sprite *create_title(int x, int y, enum xpm_image_type img_type) {
  //Sprite *title = create_sprite((xpm_map_t)mouseCursor_xpm, x, y, 0, 0, img_type);

  Sprite *title = create_sprite((xpm_map_t)Untitled_xpm, x, y, 0, 0, img_type);
  if (title == NULL) {
    return NULL;
  }
  return title;
}

Sprite *create_play_button(int x, int y, enum xpm_image_type img_type) {
  Sprite *play_button = create_sprite((xpm_map_t)play_xpm, x, y, 0, 0, img_type);
  if (play_button == NULL) {
    return NULL;
  }
  return play_button;
}

Sprite *create_exit_button(int x, int y, enum xpm_image_type img_type) {
  Sprite *exit_button = create_sprite((xpm_map_t)exit_xpm, x, y, 0, 0, img_type);
  if (exit_button == NULL) {
    return NULL;
  }
  return exit_button;
}

static void draw_menu_title(void) {
  static Sprite *title = NULL;
  if (!title) {
    title = create_title(200, 0, XPM_8_8_8);
    if (!title) {
      return;
    }
  }
  draw_sprite(title);
}

static void draw_menu_mouse(void) {
  static Sprite *pointer = NULL;
  static Sprite *cursor = NULL;
  if (!pointer) pointer = create_mouse_pointer(mouse_x, mouse_y, XPM_8_8_8);
  if (!cursor) cursor = create_cursor(mouse_x, mouse_y, XPM_8_8_8);

  // Play button area
  int play_x = 200, play_y = 200, play_w = 400, play_h = 100;
  // Exit button area
  int exit_x = 200, exit_y = 400, exit_w = 400, exit_h = 100;

  bool over_play = (mouse_x >= play_x && mouse_x <= play_x + play_w &&
                    mouse_y >= play_y && mouse_y <= play_y + play_h);
  bool over_exit = (mouse_x >= exit_x && mouse_x <= exit_x + exit_w &&
                    mouse_y >= exit_y && mouse_y <= exit_y + exit_h);

  Sprite *to_draw = (over_play || over_exit) ? cursor : pointer;
  to_draw->x = mouse_x;
  to_draw->y = mouse_y;
  draw_sprite(to_draw);
}

static void draw_play_button(void) {
  static Sprite *play_button = NULL;
  if (!play_button) {
    play_button = create_play_button(200, 200, XPM_8_8_8);
    if (!play_button) {
      return;
    }
  }
  draw_sprite(play_button);
}

static void draw_exit_button(void) {
  static Sprite *exit_button = NULL;
  if (!exit_button) {
    exit_button = create_exit_button(200, 400, XPM_8_8_8);
    if (!exit_button) {
      return;
    }
  }
  draw_sprite(exit_button);
}

void game_draw_menu(void) {
  // background
  memset(video_mem, 0, get_h_res() * get_v_res() *get_bits_per_pixel() / 8);

  draw_menu_title();
  draw_play_button();
  draw_exit_button();
  draw_menu_mouse();
  vg_new_screen();

}


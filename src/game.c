#include "game.h"
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include <stdlib.h>

// Game state
static bool game_over = false;

typedef struct {
  int x, y, size;
} Frog;
static Frog frog;
static unsigned scr_w, scr_h;
static int lanes, lane_height;
static int *obs_x, *obs_dir, *obs_speed;

void game_init(unsigned x_res, unsigned y_res) {
  scr_w = x_res;
  scr_h = y_res;
  lanes = 5;
  lane_height = scr_h / (lanes + 1);
  frog.size = lane_height / 2;
  frog.x = scr_w / 2;
  frog.y = scr_h - lane_height;
  obs_x = malloc(lanes * sizeof(int));
  obs_dir = malloc(lanes * sizeof(int));
  obs_speed = malloc(lanes * sizeof(int));
  for (int i = 0; i < lanes; i++) {
    obs_speed[i] = 2 + i;
    obs_dir[i] = (i % 2) ? -1 : 1;
    obs_x[i] = (i % 2) ? scr_w : 0;
  }
  game_over = false;
}

void game_update() {
  for (int i = 0; i < lanes; i++) {
    obs_x[i] += obs_dir[i] * obs_speed[i];
    if (obs_x[i] < -frog.size)
      obs_x[i] = scr_w;
    if ((unsigned) obs_x[i] > scr_w)
      obs_x[i] = -frog.size;
  }
  for (int i = 0; i < lanes; i++) {
    int y = scr_h - (i + 2) * lane_height;
    if (frog.y < y + frog.size && frog.y + frog.size > y) {
      if (frog.x + frog.size > obs_x[i] && frog.x < obs_x[i] + frog.size) {
        game_over = true;
      }
    }
  }
  if (frog.y < lane_height)
    game_over = true;
}

void game_draw() {
  vg_new_screen(0xFF00FF);
  for (int i = 0; i < lanes; i++) {
    int y = scr_h - (i + 2) * lane_height;
    vg_draw_rectangle(0, y, scr_w, lane_height, 0x444444);
    vg_draw_rectangle(obs_x[i], y + lane_height / 4, frog.size, frog.size / 2, 0xFF0000);
  }
  vg_draw_rectangle(frog.x, frog.y, frog.size, frog.size, 0x00FF00);
  vg_flip_page();
}

void game_handle_input(uint8_t scancode) {
  switch (scancode) {
    case 0x48: frog.y -= lane_height; break;
    case 0x50: frog.y += lane_height; break;
    case 0x4B: frog.x -= lane_height; break;
    case 0x4D: frog.x += lane_height; break;
    case 0x81: game_over = true; break;
    default: break;
  }
}

bool game_is_over() {
  return game_over;
}

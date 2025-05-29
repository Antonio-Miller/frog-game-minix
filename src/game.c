#include "game.h"
#include <lcom/lcf.h>
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include <stdlib.h>
#include "classes/lane.h"
#include "classes/player.h"
#include "classes/points.h"

// Game state
static bool game_over = false;
static Lane *lanes[NR_LANES];
static Player *player;
static int top_lane_index = 0;
static int cur_lane_index = 0;
static int bottom_lane_index = 0;

void game_draw_screen() {
  for(int i = 0; i< NR_LANES; i++) {
    if (lanes[i] != NULL) {
      draw_lane(lanes[i]);
    }
  }
  draw_player(player);
  draw_points();
  draw_highscore();
  vg_new_screen();
}

static void move_up() {
  for (int i = 0; i < NR_LANES; i++) {  //Move lanes down
    if (lanes[i] != NULL) {
      move_lane(lanes[i], 1);
    }
  }
  destroy_lane(lanes[0]); // Remove the bottom lane
  for (int i = 1; i < NR_LANES; i++) {
    lanes[i - 1] = lanes[i]; // Shift lanes down
  }
  top_lane_index++;
  bottom_lane_index++;
  lanes[NR_LANES - 1] = create_lane(top_lane_index, 0, 0, LANE_HEIGHT, XPM_8_8_8); // Create a new lane at the top
}

static void move_down() {
  for (int i = 0; i < NR_LANES; i++) {  // Move lanes up
    if (lanes[i] != NULL) {
      move_lane(lanes[i], -1);
    }
  }
  destroy_lane(lanes[NR_LANES - 1]); // Remove the top lane
  for (int i = NR_LANES - 1; i > 0; i--) {
    lanes[i] = lanes[i - 1]; // Shift lanes up
  }
  top_lane_index--;
  bottom_lane_index--;
  int new_lane_y = lanes[1]->lane->y + LANE_HEIGHT;
  lanes[0] = create_lane(top_lane_index - 5, 0, new_lane_y, LANE_HEIGHT, XPM_8_8_8); // Create a new lane at the bottom
}

void update_lanes() {
  for(int i = 0; i < NR_LANES; i++) {
    if (lanes[i] != NULL) {
      update_lane(lanes[i], player);
    }
  }
}

int game_init(unsigned x_res, unsigned y_res) {
  game_over = false;
  top_lane_index = 0;
  cur_lane_index = 0;
  bottom_lane_index = 0;

  highscore = load_highscore();

  for (int i = 0; i < NR_LANES; i++) {
    lanes[i] = create_lane(i, 0, y_res - (i + 1) * LANE_HEIGHT, LANE_HEIGHT, XPM_8_8_8);
    if (lanes[i] == NULL) {
      for (int j = 0; j < i; j++) {
        destroy_lane(lanes[j]); // Clean up previously created lanes
      }
      return -1;
    }
  }

  top_lane_index = NR_LANES - 1;

  player = create_player(x_res / 2, y_res - 50, 25, LANE_HEIGHT,XPM_8_8_8);
  if (player == NULL) {
    for (int i = 0; i < NR_LANES; i++) {
      destroy_lane(lanes[i]); // Clean up lanes if player creation fails
    }
    return -1;
  }

  points_init();

  return 0;
}

void game_update() {
  update_lanes();
  game_draw_screen();
}

void game_handle_input(uint8_t scancode) {
  switch (scancode) {
    case 0x48: // up
      if(cur_lane_index - bottom_lane_index > 2)
        move_up();
      else
        move_player_y(player, -1);

      cur_lane_index++;
      points_up();
      break;
    case 0x50: // down
      if(bottom_lane_index == 0 && cur_lane_index == 0) // Prevent moving down if at the bottom
        break;
      else if(bottom_lane_index == cur_lane_index)
        move_down();
      else
        move_player_y(player, 1);

      cur_lane_index--;
      points_down();
      break;
    case 0x4B: // left
      move_player_x(player, -1);
      break;
    case 0x4D: // right
      move_player_x(player, 1);
      break;
    case 0x81: // ESC
      game_over = true;
      break;
    default:
      break;
  }
}

bool game_is_over() {
  if(game_over || player->alive == false) {
    destroy_player(player);
    player = NULL;
    for (int i = 0; i < NR_LANES; i++) {
      destroy_lane(lanes[i]);
      lanes[i] = NULL;
    }
    destroy_points();

    save_highscore(highscore);

    return true;
  }
  return false;
}


#include "game.h"
#include <lcom/lcf.h>
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include <stdlib.h>
#include "classes/lane.h"
#include "classes/player.h"

// Game state
static bool game_over = false;
static Lane *lanes[NR_LANES];
static Player *player;
static int cur_lane = 0;

void game_draw_screen() {
  for(int i = 0; i< NR_LANES; i++) {
    if (lanes[i] != NULL) {
      draw_lane(lanes[i]);
      cur_lane++;
    }
  }
  draw_player(player);
  vg_new_screen();
}

void update_lanes() {
  for(int i = 0; i < NR_LANES; i++) {
    if (lanes[i] != NULL) {
      update_lane(lanes[i], player);
    }
  }
}

int game_init(unsigned x_res, unsigned y_res) {
  for (int i = 0; i < NR_LANES; i++) {
    lanes[i] = create_lane(cur_lane, 0, y_res - (i + 1) * 100, 100, XPM_8_8_8);
    if (lanes[i] == NULL) {
      for (int j = 0; j < i; j++) {
        destroy_lane(lanes[j]); // Clean up previously created lanes
      }
      return -1; // Handle error
    }
  }

  player = create_player(x_res / 2, y_res - 50, 25, XPM_8_8_8);
  if (player == NULL) {
    for (int i = 0; i < NR_LANES; i++) {
      destroy_lane(lanes[i]); // Clean up lanes if player creation fails
    }
    return -1; // Handle error
  }

  return 0;
}

void game_update() {
  update_lanes();
  game_draw_screen();
}

void game_handle_input(uint8_t scancode) {
  switch (scancode) {
    case 0x48: // up
      for (int i = 0; i < NR_LANES; i++) {  //Move lanes down
        if (lanes[i] != NULL) {
          move_lane(lanes[i], -1);
        }
      }
      destroy_lane(lanes[0]); // Remove the bottom lane
      cur_lane--;
      for (int i = 1; i < NR_LANES; i++) {
        lanes[i - 1] = lanes[i]; // Shift lanes down
      }
      lanes[NR_LANES - 1] = create_lane(cur_lane, 0, 0, 100, XPM_8_8_8); // Create a new lane at the top
      cur_lane++;
      break;

    case 0x50: // down
      for (int i = 0; i < NR_LANES; i++) {  //Move lanes up
        if (lanes[i] != NULL) {
          move_lane(lanes[i], 1);
        }
      }
      destroy_lane(lanes[NR_LANES - 1]); // Remove the top lane
      cur_lane--;
      for (int i = 0; i < NR_LANES - 1; i++) {
        lanes[i + 1] = lanes[i]; // Shift lanes up
      }
      lanes[0] = create_lane(cur_lane - 5, 0, SCREEN_HEIGHT - 101, 100, XPM_8_8_8); // Create a new lane at the bottom
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
  return game_over || player->alive == false;
}


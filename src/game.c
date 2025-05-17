#include "game.h"
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include <stdlib.h>

// Game state
static bool game_over = false;

typedef struct {
  int x, y, width;
} Frog;

typedef struct {
  bool alive, side;
  int x, y, velocity, width, max_width;
} Car;

static Frog frog;
static Car cars[NR_LANES+1];
static int lane_height, lane_width, bord_width, bord_height;

void game_draw_frog() {
  vg_draw_rectangle(frog.x, frog.y, frog.width, lane_height, 0x00FF00);
}

void game_clear_frog() {
  vg_draw_rectangle(frog.x, frog.y, frog.width, lane_height, 0x000000);
}

void game_draw_cars(int color) {
  for (int i = 1; i <= NR_LANES; i++) {
    Car *car = &cars[i];
    int width = car->width;
    int x = car->x;
    if (car->alive) {
      if (car->side) {
        if (x + width > bord_width) {
          width = bord_width - x;
        }
      }
      else {
        if (x < 0) {
          width += x;
          x = 0;
        }
      }
      vg_draw_rectangle(x, car->y, width, lane_height, color);
    }
  }
}

void check_colision() {
  for (int i = 1; i <= NR_LANES; i++) {
    if (!cars[i].alive)
      continue;
    Car *car = &cars[i];
    if (frog.x < car->x + car->width && frog.x + frog.width > car->x &&
        frog.y < car->y + lane_height && frog.y + lane_height > car->y) {
      game_over = true;
      break;
    }
  }
}

static void init_car(int i) {
  cars[i].alive = true;
  bool sign = rand() & 1;
  cars[i].side = sign;
  int lower = lane_width / 2, upper = lane_width * 4;
  int width = (rand() % (upper - lower + 1)) + lower;
  cars[i].max_width = width;
  cars[i].width = width / 2;
  lower = width / 10, upper = width / 2;
  cars[i].velocity = (rand() % (upper - lower + 1)) + lower;
  cars[i].x = sign ? 0 : bord_width - width;
  cars[i].y = bord_height - ((i) * lane_height);
}

void game_init(unsigned x_res, unsigned y_res) {
  // number = (rand() % (upper - lower + 1)) + lower
  bord_height = y_res;
  bord_width = x_res;
  lane_height = y_res / NR_LANES;
  lane_width = x_res / NR_LANES;

  frog.width = lane_width;
  frog.x = x_res / 2 - frog.width / 2;
  frog.y = y_res - lane_height;

  cars[0].alive = true;
  cars[1].alive = true;
  for (int i = 2; i <= NR_LANES; i++) {
    init_car(i);
  }
  game_over = false;
  game_draw_frog();
  game_draw_cars(0xFF0000);
}

void game_update() {
  game_draw_cars(0x000000);
  for (int i = 0; i <= NR_LANES; i++) {
    Car *car = &cars[i];
    if (!car->alive)
      continue;

    if (car->side) {
      if (car->width < car->max_width) {
        if (car->width + car->velocity > car->max_width) {
          car->width = car->max_width;
          car->x += car->velocity - car->max_width;
        }
        else {
          car->width += car->velocity;
        }
      }
      else {
        car->x += car->velocity;
      }
      if (car->x > bord_width) {
        init_car(i);
      }
    }
    else {
      if (car->width < car->max_width) {
        if (car->width + car->velocity > car->max_width) {
          car->width = car->max_width;
          car->x -= car->velocity - car->max_width;
        }
        else {
          car->width += car->velocity;
        }
      }
      else {
        car->x -= car->velocity;
      }
      if (car->x < 0) {
        init_car(i);
      }
    }
  }
  game_draw_cars(0xFF0000);
  check_colision();
}

void game_handle_input(uint8_t scancode) {
  switch (scancode) {
    case 0x48: // up
      game_draw_cars(0x000000);
      Car temp_car1 = cars[NR_LANES];
      init_car(NR_LANES);
      for (int i = NR_LANES - 1; i >= 0 ; i--) {
        Car *car1 = &cars[i];
        Car temp1 = cars[i];
        car1->x = temp_car1.x;
        car1->width = temp_car1.width;
        car1->velocity = temp_car1.velocity;
        car1->max_width = temp_car1.max_width;
        car1->alive = temp_car1.alive;
        car1->side = temp_car1.side;
        temp_car1 = temp1;
      }
      game_draw_cars(0xFF0000);
      break;
    case 0x50: // down
      game_draw_cars(0x000000);
      if (!cars[1].alive){
        break;
      }
      else{
        init_car(0);
      }
      Car temp_car2 = cars[0];
      for (int i = 1; i <= NR_LANES; i++) {
        Car *car2 = &cars[i];
        Car temp2 = cars[i];
        car2->x = temp_car2.x;
        car2->width = temp_car2.width;
        car2->velocity = temp_car2.velocity;
        car2->max_width = temp_car2.max_width;
        car2->alive = temp_car2.alive;
        car2->side = temp_car2.side;
        temp_car2 = temp2;
      }
      game_draw_cars(0xFF0000);
      break;
    case 0x4B: // left
      game_clear_frog();
      frog.x -= lane_width;
      game_draw_frog();
      break;
    case 0x4D: // right
      game_clear_frog();
      frog.x += lane_width;
      game_draw_frog();
      break;
    case 0x81: // ESC
      game_over = true;
      break;
    default:
      break;
  }
  // Ensure frog stays within board
  if (frog.x < 0)
    frog.x = 0;
  if (frog.x + frog.width > bord_width)
    frog.x = bord_width - frog.width;

  check_colision();
}

bool game_is_over() {
  return game_over;
}

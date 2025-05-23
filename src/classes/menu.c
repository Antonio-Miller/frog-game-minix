#include "I_O/video_card.h"
#include <lcom/lcf.h>

void game_draw_menu(void) {
  // background
  vg_draw_rectangle(0, 0, get_h_res(), get_v_res(), 0x000000);

  int w = get_h_res() / 2;
  int h = get_v_res() / 4;
  int x = (get_h_res() - w) / 2;
  int y = (get_v_res() - h) / 2;
  vg_draw_rectangle(x, y, w, h, 0x3333FF);

  // draw_text(x + 20, y + h/2, "Pressione ENTER para jogar", 0xFFFFFF);
}


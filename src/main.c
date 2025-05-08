#include <lcom/lcf.h>
#include "I_O/timer.h"
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include "game.h"

static uint8_t timer_bit;
static uint8_t kbd_bit;

int(proj_main_loop)(int argc, char *argv[]) {
  /* Initialize game and devices */
  if (vg_init(0x117) == NULL) return 1;
  game_init(get_h_res(), get_v_res());

  if (timer_subscribe_int(&timer_bit)) return 1;
  if (kbd_subscribe_int(&kbd_bit)) return 1;

  int ipc_status;
  message msg;

  while (!game_is_over()) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            timer_int_handler();
            extern int global_counter;
            if (global_counter % GAME_TICK == 0) {
              game_update();
              game_draw();
            }
          }
          if (msg.m_notify.interrupts & BIT(kbd_bit)) {
            kbc_ih();
            extern bool valid_scancode;
            extern uint8_t scancode;
            if (valid_scancode)
              game_handle_input(scancode);
          }
          break;
        default:
          break;
      }
    }
  }

  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  vg_exit();
  return 0;
}

int main(int argc, char *argv[]) {
  /* Start LCF, which will call proj_main_loop */
  lcf_set_language("EN");
  lcf_trace_calls("output.txt");
  lcf_log_output("output.txt");
  return lcf_start(argc, argv);
}

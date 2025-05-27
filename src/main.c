#include <lcom/lcf.h>
#include "I_O/timer.h"
#include "I_O/keyboard.h"
#include "I_O/video_card.h"
#include "game.h"
#include "I_O/mouse.h"
#include "classes/menu.h"
#define GAME_TICK 2

static uint8_t timer_bit;
static uint8_t kbd_bit;
static uint8_t mouse_bit;

extern struct packet mouse_packet;
extern int mouse_x, mouse_y;

typedef enum {MENU_MODE, GAME_MODE, QUIT} ProgramMode;
ProgramMode mode = MENU_MODE;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  // Inicializar vídeo
  if (vg_init(0x115) == NULL) return 1;

  // Subscrever timer e mouse para o menu
  if (timer_subscribe_int(&timer_bit)) return 1;
  if (mouse_subscribe_int(&mouse_bit)) return 1;
  m_enable_data_reporting();

  int ipc_status;
  message msg;
  bool start_game = false;
  bool exit = false;
  
  // Loop do MENU
  while (!start_game && !exit) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            game_draw_menu();
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit)) {
            mouse_ih();
            if(mouse_packet.lb) {
              MenuOption option = select_menu_option();
              switch (option) {
                case PLAY:
                  start_game = true;
                  mode = GAME_MODE;
                  break;
                
                case EXIT:
                  mode = QUIT;
                  exit = true;
                  break;
                default:
                  break;
              }
            }
          }
          break;
        default:
          break;
      }
    }
  }

  // Sair do menu: unsubscribe mouse, subscribe teclado
  m_disable_data_reporting();
  mouse_unsubscribe_int();
  if (kbd_subscribe_int(&kbd_bit)) return 1;

  // Inicializar o jogo
  if(game_init(get_h_res(), get_v_res())) return 1;

  // Loop do JOGO
  while (!game_is_over() && !exit) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            global_counter++;
            if (global_counter % GAME_TICK == 0) {
              global_counter = 0;
              game_update();
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
  // Cleanup
  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  vg_exit();
  return 0;
}


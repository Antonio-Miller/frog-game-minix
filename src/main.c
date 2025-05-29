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
extern bool valid_scancode;
extern uint8_t scancode;
extern int global_counter;

typedef enum {MENU_MODE, GAME_MODE, INSTRUCTIONS_MODE, QUIT} ProgramMode;
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

void (menu_loop)() {
  if (mouse_subscribe_int(&mouse_bit)) {
    printf("Error subscribing mouse interrupts\n");
    return;
  }

  m_enable_data_reporting();

  while (mode == MENU_MODE) {
    int ipc_status;
    message msg;

    if (driver_receive(ANY, &msg, &ipc_status) != 0) 
      continue;
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            global_counter++;
            if (global_counter % GAME_TICK == 0) {
              global_counter = 0;
              game_draw_menu();
            }
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit)) {
            mouse_ih();
            if(mouse_packet.lb) {
              MenuOption option = select_menu_option();
              switch (option) {
                case PLAY:
                  mode = GAME_MODE;
                  break;

                case INSTRUCTIONS:
                  mode = INSTRUCTIONS_MODE;
                  break;
                
                case EXIT:
                  mode = QUIT;
                  break;
      
                default:
                  break;
              }
            }
            mouse_reset_packet();
          }
          break;
        default:
          break;
      }
    }
  }
  destroy_menu();
  m_disable_data_reporting();
  mouse_unsubscribe_int();
}

void (instructions_loop)() {
  if (mouse_subscribe_int(&mouse_bit)) {
    printf("Error subscribing mouse interrupts\n");
    return;
  }

  m_enable_data_reporting();

  while (mode == INSTRUCTIONS_MODE) {
    int ipc_status;
    message msg;
    if (driver_receive(ANY, &msg, &ipc_status) != 0) 
      continue;
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            global_counter++;
            if (global_counter % GAME_TICK == 0) {
              global_counter = 0;
              draw_instructions_screen();
            }
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit)) {
            mouse_ih();
            if (mouse_packet.lb && back_button_clicked()) {
              mode = MENU_MODE;
              break;
            }
            mouse_reset_packet();
          }
          break;
        default:
          break;
      }
    }
  }
  destroy_instructions();
  m_disable_data_reporting();
  mouse_unsubscribe_int();
}

void (game_loop)() {
  if (kbd_subscribe_int(&kbd_bit)) 
    return;

  if(game_init(get_h_res(), get_v_res())) {
    kbd_unsubscribe_int();
    return;
  }

  // Loop do JOGO
  while (!game_is_over()) {
    int ipc_status;
    message msg;

    if (driver_receive(ANY, &msg, &ipc_status) != 0) 
      continue;
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
            if (valid_scancode)
              game_handle_input(scancode);
          }
          break;
        default:
          break;
      }
    }
  }
  mode = MENU_MODE;
  kbd_unsubscribe_int();
}

int (proj_main_loop)(int argc, char *argv[]) {
  // Inicializar vídeo
  if (vg_init(0x115) == NULL) 
    return 1;
  // Subscrever timer
  if (timer_subscribe_int(&timer_bit)) {
    vg_exit();
    return 1;
  }
  
  while(true) {
    switch (mode) {
      case MENU_MODE:
        menu_loop();
        break;

      case GAME_MODE:
        game_loop();
        break;

      case INSTRUCTIONS_MODE:
        instructions_loop();
        break;

      default: //QUIT
        timer_unsubscribe_int();
        vg_exit();
        return 0;
    }
  }
}


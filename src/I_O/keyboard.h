#include "i8042.h"
#define DELAY_US 20000
#ifndef KBD_HEADER
#define KBD_HEADER
#define MAX_WAIT 10
extern int hook_id_kbd;
extern uint8_t scancode;
extern bool valid_scancode;
int(kbd_subscribe_int)(uint8_t *bit_no);
int(kbd_unsubscribe_int)();
int(kbd_read_status)(uint8_t *st);
void(kbc_ih)();
int(kbc_enable_keyboard)();
int(kbc_disable_keyboard)();
#endif // KBD_HEADER

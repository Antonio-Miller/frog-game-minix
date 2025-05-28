#include <lcom/lcf.h>
#include "i8042.h"

#define DELAY_US    20000

#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#define MAX_WAIT 10

extern int hook_id_mouse;
extern uint8_t pack;
extern bool valid_packet;

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)(void);
int (mouse_read_status)(uint8_t *st);
int (mouse_read_ack)(uint8_t *ack);
int (kbc_enable_keyboard)();
int (kbc_disable_keyboard)();
int (m_disable_data_reporting)();
int (m_enable_data_reporting)();
int (mouse_send_command)(uint8_t cmd);
void (mouse_reset_packet)();

#endif // MOUSE_HEADER

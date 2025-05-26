#include "timer.h"
#include <lcom/lcf.h>
#include <stdint.h>
int global_counter = 0;
int hook_id_timer = TIMER0_IRQ;
int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || timer < 0)
    return -1;
  uint8_t st;
  uint16_t actual_freq = TIMER_FREQ / freq;
  if (timer_get_conf(timer, &st))
    return -1;
  uint8_t control = (timer << 6) | TIMER_LSB_MSB | (st & 0xF);
  if (sys_outb(TIMER_CTRL, control))
    return -1;
  uint8_t lsb, msb;
  util_get_LSB(actual_freq, &lsb);
  util_get_MSB(actual_freq, &msb);
  if (sys_outb(TIMER_0 + timer, lsb))
    return -1;
  if (sys_outb(TIMER_0 + timer, msb))
    return -1;
  return 0;
}
int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_timer;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer);
  return 0;
}
int(timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id_timer);
  return 0;
}
void(timer_int_handler)() {
  global_counter++;
}
int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || timer < 0)
    return -1;
  uint8_t control = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, control))
    return -1;
  if (util_sys_inb(TIMER_0 + timer, st))
    return -1;
  return 0;
}
int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2 || timer < 0)
    return -1;
  union timer_status_field_val conf;
  switch (field) {
    case 0:
      conf.byte = st;
      break;
    case 1:
      switch ((st >> 4) & 0x3) {
        case 0:
          conf.in_mode = INVAL_val;
          break;
        case 1:
          conf.in_mode = LSB_only;
          break;
        case 2:
          conf.in_mode = MSB_only;
          break;
        case 3:
          conf.in_mode = MSB_after_LSB;
          break;
      }
      break;
    case 2: {
      uint8_t c_mode = (st >> 1) & 0x7;
      // Checking for 110 (mode 2) or 111 (mode 3)
      if (c_mode <= 5)
        conf.count_mode = c_mode;
      else
        conf.count_mode = c_mode & 0x3;
      break;
    }
    case 3:
      conf.bcd = (st & 0x1);
      break;
  }
  timer_print_config(timer, field, conf);
  return 0;
}


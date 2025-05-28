#include "keyboard.h"
#include <lcom/lcf.h>
#include <stdint.h>

int hook_id_kbd = KBD_IRQ;
uint8_t scancode = 0;
bool valid_scancode = 0;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_kbd;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != OK)
    return -1;

  return 0;
}

int(kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_kbd) != OK)
    return -1;
  return 0;
}

int(kbd_read_status)(uint8_t *st) {
  if (util_sys_inb(KBC_ST_REG, st))
    return -1;
  return 0;
}

void(kbc_ih)() {
  uint8_t st;
  int tries = 0;
  while (tries < MAX_WAIT) {
    kbd_read_status(&st);
    tries++;
    if (st & OBF) {
      util_sys_inb(KBC_OUT_BUF, &scancode);

      if (!(st & (PARITY | TIMEOUT)))
        valid_scancode = 1;
      else
        valid_scancode = 0;

      return;
    }
  }
}

void(kbc_read_cmd_byte)(uint8_t *cmd) {
  uint8_t st;
  int tries = 0;
  while (tries < MAX_WAIT) {
    kbd_read_status(&st);
    tries++;
    if (!(st & IBF)) {
      sys_outb(KBC_CMD_REG, RD_CMD);
      int tries_sec = 0;
      while (tries_sec < MAX_WAIT) {
        tries_sec++;
        kbd_read_status(&st);
        if (st & OBF) {
          util_sys_inb(KBC_OUT_BUF, cmd);
          break;
        }
        tickdelay(DELAY_US);
      }
      break;
    }
    tickdelay(DELAY_US);
  }
}

void(kbc_write_cmd_byte)(uint8_t cmd) {
  uint8_t st;
  int tries = 0;
  while (tries < MAX_WAIT) {
    kbd_read_status(&st);
    tries++;
    if (!(st & IBF)) {
      sys_outb(KBC_CMD_REG, WT_CMD);
      int tries_sec = 0;
      while (tries_sec < MAX_WAIT) {
        kbd_read_status(&st);
        tries_sec++;
        if (!(st & IBF)) {
          sys_outb(KBC_W_CMD_BYTE, cmd);
          break;
        }
        tickdelay(DELAY_US);
      }
      break;
    }
    tickdelay(DELAY_US);
  }
}

int(kbc_enable_keyboard)() {
  uint8_t cmd_byte;
  kbc_read_cmd_byte(&cmd_byte);
  kbc_write_cmd_byte(cmd_byte | INT);
  return 0;
}

int(kbc_disable_keyboard)() {
  uint8_t cmd_byte;
  kbc_read_cmd_byte(&cmd_byte);
  cmd_byte = cmd_byte & ~INT;
  kbc_write_cmd_byte(cmd_byte);
  return 0;
}


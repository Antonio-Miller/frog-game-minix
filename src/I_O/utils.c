#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0x00FF;
  return 0;
}
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;
  return 0;
}
int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  uint8_t status;
  int retries = 10; // Number of retries to avoid infinite loop

  while (retries--) {
    if (sys_inb(KBC_ST_REG, (uint32_t *)&status) != 0) // Read status register
      return 1;

    if (status & OBF) { // Output buffer full
      if ((status & (PARITY | TIMEOUT)) == 0) { // No errors
        if (sys_inb(port, &temp) != 0)
          return 1;
        *value = (uint8_t)(temp & 0xFF);
        return 0;
      } else {
        return 1; // Parity or timeout error
      }
    }

    tickdelay(micros_to_ticks(20)); // Short delay before retry
  }

  return 1; // Failed to read after retries
}


#include <lcom/lcf.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0x00FF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t receive;

	if(sys_inb(port, &receive)){
		return -1;
	}

	*value = receive;

  return 0;
}


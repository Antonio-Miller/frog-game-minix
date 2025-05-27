#ifndef _VIDEO_CARD_H_
#define _VIDEO_CARD_H_

#include <lcom/lcf.h>
#define DIR_COLOR_MEM_MODEL 0x06 /**< @brief MemoryModel value for Direct color modes */
#define IND_COLOR_MEM_MODEL 0x04 /**< @brief MemoryModel value for Indexed color modes (packed pixel) */

unsigned(get_h_res)();
unsigned(get_v_res)();
unsigned(get_bits_per_pixel)();
int(set_mode)(uint16_t mode);
vbe_mode_info_t(get_mode_info_t)();
void *(vg_init) (uint16_t mode);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(vg_new_screen)();
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int(vg_draw_xpm)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pixmap);
int vg_flip_page();
#endif


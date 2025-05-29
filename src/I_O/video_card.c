#include "video_card.h"
#include <lcom/lcf.h>
static vbe_mode_info_t inf;
static unsigned char *video_mem;
static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static uint8_t cur_buf = 0; // for page flipping

unsigned(get_h_res)() {
  return h_res;
}

unsigned(get_v_res)() {
  return v_res;
}

unsigned(get_bits_per_pixel)() {
  return bits_per_pixel;
}

vbe_mode_info_t(get_mode_info_t)() {
  return inf;
}

static int(get_mode_info)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &inf))
  {
    return -1;
  }
  h_res = inf.XResolution;
  v_res = inf.YResolution;
  bits_per_pixel = inf.BitsPerPixel;
  return 0;
}

static void(map_vram)(uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;
  int r;

  get_mode_info(mode);
  vram_base = inf.PhysBasePtr;
  vram_size = 2 * (h_res * v_res * (bits_per_pixel / 8));
  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl(ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (unsigned char *)mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory\n");
}

int(set_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = (mode | BIT(14)) & ~BIT(15);
  if (sys_int86(&r86) != OK)
  {
    printf("\rvg_init: sys_int86() failed\n");
    return -1;
  }
  return 0;
}

void *(vg_init)(uint16_t mode) {
  map_vram(mode);
  if (set_mode(mode))
    return NULL;
  return video_mem;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  unsigned char *screen = video_mem + h_res * v_res * cur_buf;
	uint8_t color_byte_size = (bits_per_pixel + 7)/8;
	uint32_t pos = (y * h_res + x) * color_byte_size;

	while(len) {
		memcpy(screen + pos, &color, color_byte_size);
		pos += color_byte_size;
		len--;
	}

	return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  while(height){
		vg_draw_hline(x, y, width, color);
		y++;
		height--;
	}

	return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t color_byte_size = (bits_per_pixel + 7)/8;
	unsigned char *screen = video_mem + h_res * v_res * color_byte_size * cur_buf;
	uint32_t pos = (y * h_res + x) * color_byte_size;

	memcpy(screen + pos, &color, color_byte_size);

	return 0;
}

int(vg_draw_xpm)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pixmap) {
  uint8_t color_byte_size = (bits_per_pixel + 7) / 8;
  unsigned char *base = video_mem + (h_res * v_res * color_byte_size * cur_buf);

  for (uint16_t row = 0; row < height; row++) {
        unsigned char *dst = base + ((y + row) * h_res + x) * color_byte_size;
        unsigned char *src = pixmap + row * width * color_byte_size;
        memcpy(dst, src, width * color_byte_size);
  }

  return 0;
}

// Only works for 0x115 mode
int(vg_draw_xpm_with_transparency)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pixmap, uint32_t transparency_color) {
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  for (uint16_t row = 0; row < height; row++) {
    //Beginning of the row
    unsigned char *pos = pixmap + row * width * cbs;

    for (uint16_t col = 0; col < width; col++) {
      uint32_t pixel_color = *pos + (*(pos + 1) << 8) + (*(pos + 2) << 16);
      if(pixel_color != transparency_color && x + col < h_res && y + row < v_res) { // Check if pixel is transparent or out of bounds
        vg_draw_pixel(x + col, y + row, pixel_color);
      }
      pos +=cbs;
    }
  }

  return 0;
}

int(vg_flip_page)() {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x07;
  r86.bh = 0x00;
  r86.bl = 0x80;
  r86.cx = 0x00;
  r86.dx = v_res * cur_buf;
  if (sys_int86(&r86) != OK)
  {
    printf("\rvg_flip_page: sys_int86() failed\n");
    return -1;
  }
  return 0;
}

int(vg_clean_page)() {
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  memset(video_mem + h_res * v_res * cbs * cur_buf, 0,
         h_res * v_res * cbs);
  return 0;
}

int(vg_new_screen)() {
  vg_flip_page();
  cur_buf = !cur_buf;
  vg_clean_page();
  return 0;
}

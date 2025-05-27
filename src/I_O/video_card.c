#include "video_card.h"
#include <lcom/lcf.h>
static vbe_mode_info_t inf;
static unsigned char *video_mem;
static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static uint8_t cur_buf = 0; // for page flipping

unsigned(get_h_res)()
{
  return h_res;
}

unsigned(get_v_res)()
{
  return v_res;
}

unsigned(get_bits_per_pixel)()
{
  return bits_per_pixel;
}

vbe_mode_info_t(get_mode_info_t)()
{
  return inf;
}

static int(get_mode_info)(uint16_t mode)
{
  if (vbe_get_mode_info(mode, &inf))
  {
    return 1;
  }
  h_res = inf.XResolution;
  v_res = inf.YResolution;
  bits_per_pixel = inf.BitsPerPixel;
  return 0;
}

static void(map_vram)(uint16_t mode)
{
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

int(set_mode)(uint16_t mode)
{
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = (mode | BIT(14)) & ~BIT(15);
  if (sys_int86(&r86) != OK)
  {
    printf("\rvg_init: sys_int86() failed\n");
    return 1;
  }
  return 0;
}

void *(vg_init)(uint16_t mode)
{
  map_vram(mode);
  if (set_mode(mode))
    return NULL;
  return video_mem;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
  /* Fallback in case someone still calls this: write len pixels at once */
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  unsigned char *base = video_mem + (h_res * v_res * cbs * cur_buf);
  unsigned char *dst = base + ((uint32_t)y * h_res + x) * cbs;

  switch (cbs)
  {
  case 1:
    memset(dst, (uint8_t)color, len);
    break;
  case 2:
  {
    uint16_t col16 = (uint16_t)color;
    uint16_t *p16 = (uint16_t *)dst;
    for (uint16_t i = 0; i < len; i++)
      p16[i] = col16;
    break;
  }
  case 3:
    for (uint16_t i = 0; i < len; i++)
    {
      memcpy(dst + i * 3, &color, 3);
    }
    break;
  case 4:
  {
    uint32_t *p32 = (uint32_t *)dst;
    for (uint16_t i = 0; i < len; i++)
      p32[i] = color;
    break;
  }
  default:
    /* unlikely */
    for (uint16_t i = 0; i < len; i++)
      memcpy(dst + i * cbs, &color, cbs);
    break;
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  unsigned char *base = video_mem + (h_res * v_res * cbs * cur_buf);

  for (uint16_t row = 0; row < height; row++)
  {
    unsigned char *dst = base + (((uint32_t)(y + row) * h_res) + x) * cbs;
    switch (cbs)
    {
    case 1:
      memset(dst, (uint8_t)color, width);
      break;
    case 2:
    {
      uint16_t col16 = (uint16_t)color;
      uint16_t *p16 = (uint16_t *)dst;
      for (uint16_t i = 0; i < width; i++)
        p16[i] = col16;
      break;
    }
    case 3:
      for (uint16_t i = 0; i < width; i++)
        memcpy(dst + i * 3, &color, 3);
      break;
    case 4:
    {
      uint32_t *p32 = (uint32_t *)dst;
      for (uint16_t i = 0; i < width; i++)
        p32[i] = color;
      break;
    }
    default:
      for (uint16_t i = 0; i < width; i++)
        memcpy(dst + i * cbs, &color, cbs);
      break;
    }
  }
  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  unsigned char *base = video_mem + (h_res * v_res * cbs * cur_buf);
  unsigned char *dst = base + ((uint32_t)y * h_res + x) * cbs;
  memcpy(dst, &color, cbs);
  return 0;
}

int(vg_draw_xpm)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pixmap){
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  unsigned char *base = video_mem + (h_res * v_res * cbs * cur_buf);

  for (uint16_t row = 0; row < height; row++) {
        unsigned char *dst = base + ((y + row) * h_res + x) * cbs;
        unsigned char *src = pixmap + row * width * cbs;
        memcpy(dst, src, width * cbs);
  }

  return 0;
}

int(vg_flip_page)()
{
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
    return 1;
  }
  return 0;
}

int(vg_clean_page)()
{
  uint8_t cbs = (bits_per_pixel + 7) / 8;
  memset(video_mem + h_res * v_res * cbs * cur_buf, 0,
         h_res * v_res * cbs);
  return 0;
}

int(vg_new_screen)()
{
  vg_flip_page();
  cur_buf = !cur_buf;
  vg_clean_page();
  return 0;
}

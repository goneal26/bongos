#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>

void init_fb();

// property accessor functions
uint32_t fb_width(void);
uint32_t fb_height(void);
uint32_t fb_pitch(void);
uint16_t fb_bpp(void);
size_t fb_size(void);

// drawing functions
void fb_pixel(uint32_t x, uint32_t y, uint32_t color);
void fb_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void fb_clear(uint32_t color);
void fb_char(uint8_t ch, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);

// in rainbows album cover color macros
#define IR_YELLOW (0xfeff37)
#define IR_BLUE (0x2f92d2)
#define IR_ORANGE (0xf25a16)
#define IR_GREEN (0x51b241)
#define IR_BROWN (0xedb41f)
#define IR_RED (0xf9120f)
#define IR_CYAN (0xa4dde6)

// gruvbox dark palette inspired color macros
#define GB_BLACK (0x282828) 
#define GB_DARK_RED (0xcc241d)
#define GB_DARK_GREEN (0x98971a)
#define GB_DARK_YELLOW (0xd79921)
#define GB_DARK_BLUE (0x458588)
#define GB_DARK_PURPLE (0xb16286)
#define GB_DARK_AQUA (0x689d6a)
#define GB_DARK_WHITE (0xa89984) // barely brighter than GRAY
#define GB_GRAY (0x928374)
#define GB_RED (0xfb4934)
#define GB_GREEN (0xb8bb26)
#define GB_YELLOW (0xfabd2f)
#define GB_BLUE (0x83a598)
#define GB_PURPLE (0xd3869b) 
#define GB_AQUA (0x8ec07c)
#define GB_WHITE (0xebdbb2)

#endif

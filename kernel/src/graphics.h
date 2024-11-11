#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "limine.h"
#include <stdint.h>
#include <stddef.h>

// in rainbows album cover colors
#define IR_YELLOW (0xfeff37)
#define IR_BLUE (0x2f92d2)
#define IR_ORANGE (0xf25a16)
#define IR_GREEN (0x51b241)
#define IR_BROWN (0xedb41f)
#define IR_RED (0xf9120f)
#define IR_CYAN (0xa4dde6)

// temporary color macros based on gruvbox dark palette
#define BLACK (0x282828) 
#define DARK_RED (0xcc241d)
#define DARK_GREEN (0x98971a)
#define DARK_YELLOW (0xd79921)
#define DARK_BLUE (0x458588)
#define DARK_PURPLE (0xb16286)
#define DARK_AQUA (0x689d6a)
#define DARK_WHITE (0xa89984) // barely brighter than GRAY
#define GRAY (0x928374)
#define RED (0xfb4934)
#define GREEN (0xb8bb26)
#define YELLOW (0xfabd2f)
#define BLUE (0x83a598)
#define PURPLE (0xd3869b) 
#define AQUA (0x8ec07c)
#define WHITE (0xebdbb2)

void init_screen(struct limine_framebuffer* fb);
uint64_t max_width(void);
uint64_t max_height(void);

void putpixel(uint64_t x, uint64_t y, uint32_t color);
void fillrect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color);

// draw an 8x8 character
void drawchar(uint8_t ch, uint64_t x, uint64_t y, uint32_t fg, uint32_t bg);

#endif

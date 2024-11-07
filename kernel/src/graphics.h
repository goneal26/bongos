#ifndef GRAPHICS_H
#define GRAPHICS_H 1

#include "limine.h"
#include <stdint.h>
#include <stddef.h>

// color macros! (from In Rainbows)
#define RED (0xf9120f)
#define ORANGE (0xf25a16)
#define YELLOW (0xfeff37)
#define GREEN (0x51b241)
#define BLUE (0x2f92d2)
#define WHITE (0xa4dde6)
#define BLACK (0xffffff)

void init_screen(struct limine_framebuffer* fb);
uint64_t max_width(void);
uint64_t max_height(void);

void putpixel(uint64_t x, uint64_t y, uint32_t color);
void fillrect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color);

#endif

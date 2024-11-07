#include "graphics.h"
#include "limine.h"
#include <stddef.h>

// holds a copy of the framebuffer to abstract stuff away
typedef struct {
  uint64_t width; // pixels per line
  uint64_t height; // number of lines
  uint64_t pitch; // amt of bytes to skip to go one pixel down
  volatile uint32_t *pixels;
} Screen;

Screen screen;

void init_screen(struct limine_framebuffer* fb) {
  screen.width = fb->width;
  screen.height = fb->height;
  screen.pitch = fb->pitch;
  screen.pixels = fb->address;
}

uint64_t max_width(void) {
  return screen.width;
}

uint64_t max_height(void) {
  return screen.height;
}

// a custom put-pixel function!!!
void putpixel(uint64_t x, uint64_t y, uint32_t color) {
  size_t offset = y * (screen.pitch / 4) + x;
  screen.pixels[offset] = color;
}

// fills a rectangle starting with top-left corner (x, y)
void fillrect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color) {
  size_t offset = y * (screen.pitch / 4) + x; // idx for top left corner

  for (uint64_t ycoord = 0; ycoord < h; ycoord++) {
    for (uint64_t xcoord = 0; xcoord < w; xcoord++) {
      screen.pixels[offset + xcoord] = color;
    }
    offset += screen.pitch / 4;
  }
}


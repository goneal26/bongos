#include "framebuffer.h"
#include "font.h"
#include "../limine.h" // TODO no relative paths, very bad
#include <stddef.h>

// grabbing the info from the bootloader about the frame buffer
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};

// holds a copy of the framebuffer info to abstract stuff away
typedef struct {
  uint32_t width; // pixels per row
  uint32_t height; // total row count
  uint32_t pitch; // bytes per row
  uint16_t bpp; // bits per pixel
  size_t size; // size of framebuffer memory block 
  volatile uint32_t *pixels; // TODO does it really need `volatile`?
} framebuffer_t;

framebuffer_t screen;

// accessors
uint32_t fb_width(void) {
  return screen.width;
}

uint32_t fb_height(void) {
  return screen.height;
}

uint32_t fb_pitch(void) {
  return screen.pitch;
}

uint16_t fb_bpp(void) {
  return screen.bpp;
}

size_t fb_size(void) {
  return screen.size;
}

void init_fb() {
  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
  // ^ TODO can be null, panic?

  screen.width = fb->pitch / (fb->bpp / 8);
  // ^ calculating width by hand seems less error-prone?
  screen.height = fb->height;
  screen.pitch = fb->pitch;
  screen.bpp = fb->bpp;
  screen.size = fb->height * fb->pitch;
  screen.pixels = (uint32_t*) fb->address;
}

// TODO mutex stuff?

// draws a pixel at x, y (where 0,0 is the top left corner) w/ hexcode color
void fb_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (x >= screen.width || y >= screen.height) return;
  screen.pixels[y * (screen.pitch / (screen.bpp / 8)) + x] = color;
}

// fills a rectangle starting with top-left corner (x, y)
// fills from x to w - 1, from y to h - 1
void fb_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
  if (x > screen.width || y > screen.height) return;
  size_t offset = y * (screen.pitch / (screen.bpp / 8)) + x; 
  // memory offset for top left corner

  for (uint32_t i = 0; i < h; i++) {
    for (uint32_t j = 0; j < w; j++) {
      screen.pixels[offset + j] = color;
    }
    offset += screen.pitch / (screen.bpp / 8);
  }
}

// clears the framebuffer, filling with provided color (hex code)
void fb_clear(uint32_t color) {
  size_t offset = 0;
  for (uint32_t i = 0; i < screen.height; i++) {
    for (uint32_t j = 0; j < screen.width; j++) {
      screen.pixels[offset + j] = color;
    }
    offset += screen.pitch / (screen.bpp / 8);
  }
}

// draw an 8x8 character at x, y with fg and bg hex colors
void fb_char(uint8_t ch, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg) {
  size_t offset = y * (screen.pitch / (screen.bpp / 8)) + x; 
  // idx for top left corner

  if (ch < 32 || ch > 126) { 
    // if not a visible char, don't bother drawing
    return;
  }
  
  for (uint32_t i = 0; i < 8; i++) { // 8x8 chars
    for (uint32_t j = 0; j < 8; j++) {
      if (Font8x8_basic[ch - 32][i] & (1 << j)) { // bit is 1 (foreground)
        screen.pixels[offset + j] = fg;
      } else { // bit is 0 (background)
        screen.pixels[offset + j] = bg;        
      }
    }
    offset += screen.pitch / 4;
  }
}

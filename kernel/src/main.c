#include "limine.h"
#include "memory.h"
#include "graphics.h"
#include <stdint.h>
#include <stddef.h>

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void hcf(void) {
  for (;;) {
#if defined (__x86_64__)
    asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
    asm ("wfi");
#elif defined (__loongarch64)
    asm ("idle 0");
#endif
  }
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == 0) { // is "false" really needed?
    hcf();
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL || 
  framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  // graphics functions
  init_screen(framebuffer);

  fillrect(0, 0, max_width(), max_height(), BLACK);

  /*
  for (uint64_t y = 0; y < max_height(); y += 8) {
    for (uint64_t x = 0; x < max_width(); x += 8) {
      drawchar('@', x, y, WHITE, BLACK);
    }
  }
  */

  uint8_t x = 100;
  uint8_t y = 50;
  for (uint8_t ch = 0; ch < 127; ch++) { // should print ascii 0-127
    drawchar(ch, x, y, WHITE, BLACK);
    x += 8;
    if (ch % 8 == 0) {
      y += 8;
      x -= 64;
    }
  }
  
  /*
  char *msg = "Hello World!";
  
  while (*msg) {
    drawchar(*msg, x, y, RED, WHITE);
    x += 8;
    //y += 8;
    msg += 1;
  }
  */

  // We're done, just hang...
  hcf();
}

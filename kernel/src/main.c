#include "limine.h"
#include "memory.h"
#include "graphics.h"
#include "tty.h"
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

__attribute__((used, section(".limine_requests")))
static volatile struct limine_bootloader_info_request bootloader_info_request = {
  .id = LIMINE_BOOTLOADER_INFO_REQUEST,
  .revision = 1 // right?
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
  if (LIMINE_BASE_REVISION_SUPPORTED == 0) {
    hcf();
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL || 
  framebuffer_request.response->framebuffer_count < 1) {
    hcf();
  }

  // get some info about the bootloader
  if (bootloader_info_request.response == NULL) {
    hcf();
  }

  // Fetch the first (and only?) framebuffer.
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  // setup screen + tty
  init_screen(framebuffer);
  fillrect(0, 0, max_width(), max_height(), DARK_BLUE);
  fillrect(2, 2, max_width() - 4, max_height() - 4, BLACK);
  init_tty(3, 3, max_width() - 6, max_height() - 6); // uses ~25% of screen

  // put whatever printing functions you want below!

  // bootloader info
  println("Bootloader name: \"%s\"", bootloader_info_request.response->name);
  println("Bootloader version: \"%s\"\n", bootloader_info_request.response->version);

  // framebuffer info
  println("Total framebuffer count: %d\n", framebuffer_request.response->framebuffer_count);
  println("Framebuffer info: ");
  println("Address: %p", framebuffer->address);
  println("Width: %d pixels", framebuffer->width);
  println("Height: %d pixels", framebuffer->height);
  println("Pitch (bytes per row): %d", framebuffer->pitch);
  println("BPP (bits per pixel): %d", framebuffer->bpp);
  
  // We're done, just hang...
  hcf();
}

#include "limine.h"
#include "memory.h"
#include "device/framebuffer.h"
#include "device/tty.h"
#include <stdint.h>
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_bootloader_info_request bootloader_info_request = {
  .id = LIMINE_BOOTLOADER_INFO_REQUEST,
  .revision = 1 // right?
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.
// TODO move these?

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

// kernel main entry point
void kmain(void) {
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == 0) {
    hcf();
  }

  // get some info about the bootloader
  if (bootloader_info_request.response == NULL) {
    hcf();
  }

  // setup screen + tty
  init_fb();
  fb_clear(GB_DARK_BLUE);
  fb_rect(2, 2, fb_width() - 4, fb_height() - 4, GB_BLACK);
  init_tty(3, 3, fb_width() - 6, fb_height() - 6); // uses ~25% of screen

  // put whatever printing functions you want below!

  // bootloader info
  println("Bootloader name: \"%s\"", bootloader_info_request.response->name);
  println("Bootloader version: \"%s\"\n", bootloader_info_request.response->version);

  // framebuffer info
  println("Framebuffer info: ");
  println("Width: %d pixels", fb_width());
  println("Height: %d pixels", fb_height());
  println("Pitch (bytes per row): %d", fb_pitch());
  println("BPP (bits per pixel): %d", fb_bpp());
  println("\nEasy like sunday morning!");
  
  // We're done, just hang...
  hcf();
}

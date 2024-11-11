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

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  // graphics functions
  init_screen(framebuffer);

  // why not, fill the screen and put a little "window" around the terminal
  fillrect(0, 0, max_width(), max_height(), DARK_BLUE);
  fillrect(23, 23, (max_width() / 2) + 4, (max_height() / 2) + 4, PURPLE);
  // should give a dark blue background and a 2-pixel purple border
  
  // terminal!
  init_tty(25, 25, max_width() / 2, max_height() / 2); // uses ~25% of screen

  printstr("Testing ASCII rendering: \n\n");
  
  for (uint8_t c = '!'; c < 127; c++) { // print every visible ascii char in our font
    printchar(c);
  }
  
  printstr("\n\nHello, terminal!\n");
  printstr("The max screen width is: ");
  printint(max_width(), 10);
  printstr(" pixels.\nThe max screen height is: ");
  printint(max_height(), 10);
  printstr(" pixels.\n");

  printstr("The screen pitch value is: ");
  printint(framebuffer->pitch, 10);
  printchar('\n');

  printstr("There are "); 
  printint(framebuffer_request.response->framebuffer_count, 10);
  printstr(" total framebuffers.\n");
  
  // We're done, just hang...
  hcf();
}

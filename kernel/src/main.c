#include "limine.h"
#include "memory.h"
#include "device/framebuffer.h"
#include "device/tty.h"
#include "sys/bootloader.h"

// initializing stuff for Limine:

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

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

  // setup screen + tty
  init_fb();
  fb_clear(GB_DARK_BLUE);
  fb_rect(2, 2, fb_width() - 4, fb_height() - 4, GB_BLACK);
  init_tty(3, 3, fb_width() - 6, fb_height() - 6); // uses ~25% of screen

  // put whatever printing functions you want below!

  // framebuffer info
  println("Framebuffer info: ");
  println("Width: %d pixels", fb_width());
  println("Height: %d pixels", fb_height());
  println("Pitch (bytes per row): %d", fb_pitch());
  println("BPP (bits per pixel): %d", fb_bpp());
  println("\nEasy like sunday morning!\n");

  // now let's get that hardware info from bootloader.c
  println("Hardware info provided by bootloader:");
  println("Bootloader name: %s", bl_get_bootloader_name());
  println("Bootloader version: %s", bl_get_bootloader_version());
  println("Kernel file address: %p", bl_get_kernel_file_address());
  println("Kernel file size: %d", bl_get_kernel_file_size());
  println("System disk UUID: %p", bl_get_gpt_system_disk_uuid());
  println("System partition UUID: %p", bl_get_gpt_system_partition_uuid());
  println("MBR system disk ID: %p", bl_get_mbr_system_disk_id());
  println("Kernel physical base: %x", bl_get_kernel_phys_base());
  println("Kernel virtual base: %x", bl_get_kernel_virt_base());
  println("Higher Half Direct Map offset: %d", bl_get_hhdm_offset());
  println("CPU count: %d", bl_get_cpu_count());
  println("LAPIC ID: %d", bl_get_bsp_lapic_id());
  println("x2apic flags: %x", bl_is_x2apic());
  println("RSDP address: %p", bl_get_rsdp_address());
  
  // We're done, just hang...
  hcf();
}

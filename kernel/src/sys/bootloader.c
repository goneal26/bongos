#include "bootloader.h"
#include "../limine.h"

// got all this from Garnek0's GarnOS, gonna replace it with original code
// but for now just using their code to test fetching + printing 
// basic hardware info. 
// Source: https://github.com/Garnek0/GarnOS/blob/alpha/kernel/sys/bootloader.c

// grabbing a bunch of requests for Limine features:
// TODO check if null? Theoretically all of these *could* fail
// at the same time what good is manually panicking when these are null;
// if these are null the OS would panic and crash anyways, right?

// bootloader name and version
__attribute__((used, section(".limine_requests")))
static volatile struct limine_bootloader_info_request bl_info_request = {
  .id = LIMINE_BOOTLOADER_INFO_REQUEST,
  .revision = 0
};

// ptr to limine file for the kernel file
__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_file_request kernel_file_request = {
  .id = LIMINE_KERNEL_FILE_REQUEST,
  .revision = 0
};

// physical and virtual base addresses of the kernel
__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_address_request kernel_address_request = {
  .id = LIMINE_KERNEL_ADDRESS_REQUEST,
  .revision = 0
};

// virtual address for the start of the higher half direct map
__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0
};

// SMP (symmetric multiprocessing) info
__attribute__((used, section(".limine_requests")))
static volatile struct limine_smp_request smp_request = {
  .id = LIMINE_SMP_REQUEST,
  .revision = 0,
  .flags = 0
};

// RDSP table address
__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
  .id = LIMINE_RSDP_REQUEST,
  .revision = 0
};

// Memory map info (entry count, etc) -> TODO move this somewhere else?
__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
  .id = LIMINE_MEMMAP_REQUEST,
  .revision = 0
};

// INFO RETRIEVAL FUNCTIONS START HERE VVVVV

char* bl_get_bootloader_name() {
  return bl_info_request.response->name;
}

char* bl_get_bootloader_version() {
  return bl_info_request.response->version;
}

void* bl_get_kernel_file_address() {
  return kernel_file_request.response->kernel_file->address;
}

uint64_t bl_get_kernel_file_size() {
  return kernel_file_request.response->kernel_file->size;
}

uint64_t* bl_get_gpt_system_disk_uuid() {
  return (uint64_t*)&kernel_file_request.response->kernel_file->gpt_disk_uuid;
}

uint64_t* bl_get_gpt_system_partition_uuid() {
  return (uint64_t*)&kernel_file_request.response->kernel_file->gpt_part_uuid;
}

uint64_t* bl_get_mbr_system_disk_id() {
  return (uint64_t*)&kernel_file_request.response->kernel_file->mbr_disk_id;
}

uint64_t bl_get_kernel_phys_base() {
  return kernel_address_request.response->physical_base;
}

uint64_t bl_get_kernel_virt_base() {
  return kernel_address_request.response->virtual_base;
}

uint64_t bl_get_hhdm_offset() {
  return hhdm_request.response->offset;
}

size_t bl_get_cpu_count() {
  return smp_request.response->cpu_count;
}

uint32_t bl_get_bsp_lapic_id() {
  return smp_request.response->bsp_lapic_id;
}

uint8_t bl_is_x2apic() {
  return smp_request.response->flags;
}

void* bl_get_rsdp_address() {
  return rsdp_request.response->address;
}

// struct limine_smp_info** bl_get_cpu_info_array() {
//   return smp_request.response->cpus;
// }
// 
// Won't need these for now, will use for mutex support later though
//
// struct limine_smp_info* bl_get_cpu_info(size_t index) {
//   return smp_request.response->cpus[index];
// }

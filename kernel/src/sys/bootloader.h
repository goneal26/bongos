#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include <stddef.h>

char* bl_get_bootloader_name();
char* bl_get_bootloader_version();
void* bl_get_kernel_file_address();
uint64_t bl_get_kernel_file_size();
uint64_t* bl_get_gpt_system_disk_uuid();
uint64_t* bl_get_gpt_system_partition_uuid();
uint64_t* bl_get_mbr_system_disk_id();
uint64_t bl_get_kernel_phys_base();
uint64_t bl_get_kernel_virt_base();
uint64_t bl_get_hhdm_offset();
size_t bl_get_cpu_count();
uint32_t bl_get_bsp_lapic_id();
uint8_t bl_is_x2apic();
void* bl_get_rsdp_address();

#endif

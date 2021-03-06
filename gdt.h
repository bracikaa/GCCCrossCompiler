#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry_struct
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access_byte;
	uint8_t flags_and_limit_high;
	uint8_t base_high;
}__attribute((packed));

typedef struct gdt_entry_struct gdt;

struct gdt_ptr_struct
{
	uint16_t limit;
	uint32_t base;
} __attribute((packed));

typedef struct gdt_ptr_struct gdt_pointer;

void init_gdt()

#endif

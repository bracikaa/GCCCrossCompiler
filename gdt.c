#include "gdt.h"
#include "terminal.h"
#include <stdint.h>
#include <stddef.h>


extern void load_gdt(gdt_pointer* gdt_ptr);

static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt gdt_entries[3];
gdt_pointer gdt_ptr;

void init_gdt()
{
	gdt_ptr.limit = (sizeof(gdt) * 3) - 1; //size, -1 because it limit always one less than the lenght of the table
	gdt_ptr.base =(uint32_t)&gdt_entries; //starting adress

	gdt_set_gate(0, 0, 0, 0, 0); //null descriptor
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //code segment descriptor
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //data segment descriptor
	//gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	//gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	//terminalwritestring
	load_gdt(&gdt_ptr);
	
}

static void gdt_set_gate(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[entry].base_low = (base & 0xFFFF);
	gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
	gdt_entries[entry].base_high = (base >> 24) & 0xFF;

	gdt_entries[entry].limit_low = (limit & 0xFFFF);
	gdt_entries[entry].flags_and_limit_high = (limit >> 16) & 0x0F;

	gdt_entries[entry].flags_and_limit_high |= gran & 0xF0;
	gdt_entries[entry].access_byte = access;
}

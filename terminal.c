#include <stddef.h>
#include <stdint.h>
#include "gdt.h"
#include "terminal.h"
#include "port.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will get an error"
#endif

#if !defined(__i386__)
#error "This tutorial needs a ix86 compiler"
#endif

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define VGA_CURSOR_HIGH 14
#define VGA_CURSOR_LOW 15


uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

uint16_t vga_entry(unsigned char c, uint8_t color)
{
	uint16_t char16 = c;
	uint16_t color16 = color;
	return char16 | color16 << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while(str[len])
		len++;
	return len;
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll()
{
	uint16_t empty_vga_entry = vga_entry(' ', terminal_color);
	for(size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];
		
	for(size_t i = (VGA_HEIGHT -1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
	{
		terminal_buffer[i] = empty_vga_entry;
		terminal_column = 0;
	}
}
void terminal_newline()
{
	terminal_column = 0;
	if(++terminal_row == VGA_HEIGHT)
	{
		terminal_row--;
		terminal_scroll();
	}
	move_cursor(terminal_column, terminal_row);	
}

void terminal_putchar(char c)
{
	switch(c)
	{
	case '\n':
		terminal_newline();
		break;

	default:
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if(++terminal_column == VGA_WIDTH)
		{
			terminal_column = 0;
			if(++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
		break;
	}
}

void terminal_write(const char* data, size_t size)
{
	for(size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_advance()
{
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_newline();
	}
	else
	{
		move_cursor(terminal_column, terminal_row);
	}
}


void move_cursor(uint8_t xpos, uint8_t ypos)
{
	//uint8_t xpos, uint8_t ypos
	uint16_t location = ypos * VGA_WIDTH + xpos;
	
	outb(VGA_COMMAND_PORT, VGA_CURSOR_HIGH);
	outb(VGA_DATA_PORT, location >> 8);
	outb(VGA_COMMAND_PORT, VGA_CURSOR_LOW);
	outb(VGA_DATA_PORT, location);

}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif


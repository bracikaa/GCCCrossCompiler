#if !defined(__cpluspus)
#include <stdbool.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
//#include "port.h"

#if !defined(__i386__)
#error "Needs ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C"
#endif

#if defined(__linux__)
#error "You are not using a cross compiler, you will run into errors"
#endif

//

void kernel_main()
{
	terminal_initialize();
	terminal_writestring("Hello, world. Skynet is back online!\n");
	terminal_writestring("Keep on testing new stuff");
	terminal_putentryat('c', 01101100, 120, 5);
	for(int i = 0; i < 11; i++)
	{
		terminal_writestring("Workss\n");
	}
	terminal_writestring("\n\n\n");
	terminal_writestring("SCROOLLL ISNT WORKINGGGG!");
}

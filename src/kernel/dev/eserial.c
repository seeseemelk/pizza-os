#include "dev/eserial.h"
#include "io.h"
#include <stdbool.h>

#define PORT 0x3E8
//#define PORT 0x3F8

/*
 * Code taken from:
 * https://wiki.osdev.org/Serial_Ports
 */
void eserial_init()
{
	outb(PORT+1, 0x00); // No interrupts
	outb(PORT+3, 0x80); // Set DLAB
	outb(PORT+0, 0x03); // Set divisor to 3 (38400 baud)
	outb(PORT+1, 0x00);
	outb(PORT+3, 0x03); // 8N1
	outb(PORT+2, 0xC7); // Enable FIFO
}

bool eserial_is_tx_empty()
{
	return (inb(PORT+5) & 0x20) > 0;
}

bool eserial_has_rx_data()
{
	return (inb(PORT+5) & 0x1) > 0;
}

void eserial_putchar(char c)
{
	while (!eserial_is_tx_empty());
	outb(PORT, c);
}

char eserial_readchar()
{
	while (!eserial_has_rx_data());
	return inb(PORT);
}

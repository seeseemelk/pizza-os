#include "config.hpp"
#include "debug.hpp"
#include "io.hpp"

#define DATA_AVAILABLE (1<<0)
#define TRANSMITTER_EMPTY (1<<1)
#define BREAK_ERROR (1<<2)
#define STATUS_CHANGE (1<<3)

static const IO::Port reg_data(Config::Int::SERIAL_PORT);
static const IO::Port reg_ie(Config::Int::SERIAL_PORT + 1);
static const IO::Port reg_fifo(Config::Int::SERIAL_PORT + 2);
static const IO::Port reg_line_ctrl(Config::Int::SERIAL_PORT + 3);
static const IO::Port reg_modem_ctrl(Config::Int::SERIAL_PORT + 4);
static const IO::Port reg_line_status(Config::Int::SERIAL_PORT + 5);
//static const IO::Port reg_modum_status(Config::Int::SERIAL_PORT + 6);
//static const IO::Port reg_scratch(Config::Int::SERIAL_PORT + 7);

void Debug::init_serial()
{
	// Disable interrupts
	reg_ie.outb(0x00);

	// Enable DLAB
	reg_line_ctrl.outb(0x80);

	// Set baudrate to 38400
	reg_data.outb(0x03);
	reg_ie.outb(0x00);

	// Set 8 bits, no parity, one stop bit
	reg_line_ctrl.outb(0x03);

	// Enable FIFO and clear
	reg_fifo.outb(0xC7);

	// IRQs enabled, RTS/DSR set
	reg_modem_ctrl.outb(0x0B);
}

static int is_transmit_empty()
{
	return reg_line_status.inb() & 0x20;
}

void Debug::put(const char c)
{
	while (!is_transmit_empty());
	reg_data.outb(c);
}

void Debug::puts(const char* c)
{
	while (*c != 0)
	{
		put(*c);
		c++;
	}
}













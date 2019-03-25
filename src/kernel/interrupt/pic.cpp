#include "interrupt.hpp"
#include "io.hpp"

using namespace Interrupt;

#define PIC_EOI 0x20
#define PIC_OCW3 0x0A

static const IO::Port msr_cmd(0x20);
static const IO::Port slv_cmd(0x21);
static const IO::Port msr_dat(0xA0);
static const IO::Port slv_dat(0xA1);

void PIC::send_eoi(u8 irq)
{
	if (irq >= 0x28)
		slv_cmd.outb(PIC_EOI);
	msr_cmd.outb(PIC_EOI);
}

[[nodiscard]] bool PIC::is_served(u8 irq)
{
	if (irq <= 0x27)
	{
		msr_cmd.outb(PIC_OCW3);
		u8 served = msr_cmd.inb();
		return (served & (1 << (irq - 0x20))) > 0;
	}
	else
	{
		slv_cmd.outb(PIC_OCW3);
		u8 served = slv_cmd.inb();
		return (served & (1 << (irq - 0x28))) > 0;
	}
}

void PIC::init()
{
	// Can't remember what this does...
	msr_cmd.outb(0x11);
	slv_cmd.outb(0x11);
	msr_dat.outb(0x20);
	slv_dat.outb(0x28);
	msr_cmd.outb(0x04);
	slv_dat.outb(0x02);

	msr_dat.outb(0x01);
	slv_dat.outb(0x01);
}






















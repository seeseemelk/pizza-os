#include "interrupt.hpp"
#include "io.hpp"

using namespace Interrupt;

#define PIC_EOI 0x20
#define PIC_OCW3_IRR 0x0A // Command to read In-Request Register (IRR)
#define PIC_OCW3_ISR 0x0B // Command to read In-Service Register (ISR)

#define ICW1_ICW4 0x01 // ICW4 (not) needed
#define ICW1_SINGLE 0x02 // Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL 0x08 // Level triggered (edge) mode
#define ICW1_INIT 0x10 // Initialization - required!

#define ICW4_8086 0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM 0x10 // Special fully nested (not)

static const IO::Port msr_cmd(0x20);
static const IO::Port msr_dat(0x21);
static const IO::Port slv_cmd(0xA0);
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
		msr_cmd.outb(PIC_OCW3_ISR);
		u8 served = msr_cmd.inb();
		return (served & (1 << (irq - 0x20))) > 0;
	}
	else
	{
		slv_cmd.outb(PIC_OCW3_ISR);
		u8 served = slv_cmd.inb();
		return (served & (1 << (irq - 0x28))) > 0;
	}
}

void PIC::init()
{
	// From https://wiki.osdev.org/8259_PIC

	// [ICW1] Starts the initialisation sequence (in cascade mode).
	msr_cmd.outb(ICW1_INIT | ICW1_ICW4);
	slv_cmd.outb(ICW1_INIT | ICW1_ICW4);

	// [ICW2] Set new offsets.
	msr_dat.outb(0x20);
	slv_dat.outb(0x28);

	// [ICW3] Tell master PIC that there is a slave PIC at IRQ2.
	msr_dat.outb(0x04);
	// [ICW3] Tell slave PIC its cascade identity.
	slv_dat.outb(0x02);

	// [ICW4] Switch PICs into 8086 mode.
	msr_dat.outb(ICW4_8086);
	slv_dat.outb(ICW4_8086);
}






















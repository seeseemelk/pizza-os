#include "cpu.hpp"
#include "kernel.hpp"
#include <cstdint>
#include <cstddef>

extern "C" void load_kernel_segment_registers();

enum DescriptorType
{
	CODE_OR_DATA = 1,
	SYSTEM = 0
};

enum Direction
{
	GROWS_UP = 0,
	GROWS_DOWN = 1
};

enum Granularity
{
	BYTE = 0,
	PAGE = 1
};

enum RWCode
{
	ALLOW_READ = 1,
	DISALLOW_READ = 0
};

enum RWData
{
	ALLOW_WRITE = 1,
	DISALLOW_WRITE = 0
};

enum OperationSize
{
	BIT16 = 0,
	BIT32 = 1
};

struct SegmentDescriptor
{
	u16 limit00_15;
	u16 base00_15;
	u8 base16_23;

	u8 accessed : 1;
	u8 rw : 1;
	u8 direction : 1;
	u8 executable : 1;

	u8 descriptor_type : 1;
	u8 dpl : 2;
	u8 present : 1;

	u16 limit16_19 : 4;
	u16 available : 1;
	u16 bit64 : 1; /* Should always be 0. */
	u16 operation_size : 1;
	u16 granularity : 1;
	u16 base24_31 : 8;

	void set_base_limit(u32 address, u32 bytes);
} __attribute__((packed));

struct GDTDescriptor
{
	u16 limit;
	u32 base;
} __attribute__((packed));

static SegmentDescriptor gdt[6];
static GDTDescriptor gdt_descriptor;

void SegmentDescriptor::set_base_limit(u32 address, u32 bytes)
{
	base00_15 = address & 0xFFFF;
	base16_23 = (address >> 16) & 0xFF;
	base24_31 = (address >> 24) & 0xFF;

	u32 limit = (address + bytes) / KB(4);
	limit00_15 = limit & 0xFFFF;
	limit16_19 = (limit >> 16);
}

static void lgdt(GDTDescriptor& descriptor)
{
	asm ("lgdt %0" :: "m" (descriptor));
}

void CPU::init_gdt()
{
	// Set all descriptors to 0
	for (size_t i = 0; i < 6; i++)
		gdt[i] = {};

	// Kernel code descriptor
	gdt[1].rw = RWCode::ALLOW_READ;
	gdt[1].direction = Direction::GROWS_UP;
	gdt[1].executable = true;
	gdt[1].descriptor_type = DescriptorType::CODE_OR_DATA;
	gdt[1].dpl = 0;
	gdt[1].present = 1;
	gdt[1].operation_size = OperationSize::BIT32;
	gdt[1].granularity = PAGE;
	gdt[1].set_base_limit(Kernel::kernel_start, GB(1) - 1);

	// Kernel data descriptor
	gdt[2].rw = RWData::ALLOW_WRITE;
	gdt[2].direction = Direction::GROWS_UP;
	gdt[2].executable = false;
	gdt[2].descriptor_type = DescriptorType::CODE_OR_DATA;
	gdt[2].dpl = 0;
	gdt[2].present = 1;
	gdt[2].operation_size = OperationSize::BIT32;
	gdt[2].granularity = PAGE;
	gdt[2].set_base_limit(Kernel::kernel_start, GB(1) - 1);

	// Userspace code descriptor
	gdt[3].rw = RWCode::ALLOW_READ;
	gdt[3].direction = Direction::GROWS_UP;
	gdt[3].executable = true;
	gdt[3].descriptor_type = DescriptorType::CODE_OR_DATA;
	gdt[3].dpl = 3;
	gdt[3].present = 1;
	gdt[3].operation_size = OperationSize::BIT32;
	gdt[3].granularity = PAGE;
	gdt[3].set_base_limit(0, Kernel::kernel_start - 1);

	// Userspace data descriptor
	gdt[4].rw = RWData::ALLOW_WRITE;
	gdt[4].direction = Direction::GROWS_UP;
	gdt[4].executable = false;
	gdt[4].descriptor_type = DescriptorType::CODE_OR_DATA;
	gdt[4].dpl = 3;
	gdt[4].present = 1;
	gdt[4].operation_size = OperationSize::BIT32;
	gdt[4].granularity = PAGE;
	gdt[4].set_base_limit(0, Kernel::kernel_start - 1);

	// GDT 5 is reserved for TSS

	// Setup GDT Descriptor
	gdt_descriptor.base = reinterpret_cast<u32>(&gdt);
	gdt_descriptor.limit = sizeof(gdt);
	lgdt(gdt_descriptor);
	load_kernel_segment_registers();
}



























#include "ps2.h"
#include "kernel.h"
#include "collections/list.h"

u8 ps2_read_status(ps2_bus_t* bus)
{
	return bus->read_status(bus);
}

void ps2_write_status(ps2_bus_t* bus, u8 data)
{
	return bus->write_status(bus, data);
}

u8 ps2_read_data(ps2_bus_t* bus)
{
	return bus->read_data(bus);
}

void ps2_write_data(ps2_bus_t* bus, u8 data)
{
	bus->write_data(bus, data);
}

void ps2_write_command(ps2_bus_t* bus, u8 data)
{
	bus->write_command(bus, data);
}

list_t* list = NULL;

void ps2_register(device_t* dev, ps2_bus_t* bus)
{
	if (list == NULL)
		list = list_new();

	bus->dev = dev;
	list_add(list, bus);
}

ps2_bus_t* ps2_get(device_t* dev)
{
	size_t size = list_size(list);
	for (size_t i = 0; i < size; i++)
	{
		ps2_bus_t* bus = list_get(list, i);
		if (bus->dev == dev)
			return bus;
	}
	kernel_panic("No PS/2 controller found");
	return NULL;
}

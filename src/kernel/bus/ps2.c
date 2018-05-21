#include "ps2.h"
#include "kernel.h"
#include "collections/list.h"

u8 ps2_read_status(ps2_bus_t* bus)
{
	return bus->read_status(bus->dev);
}

void ps2_write_status(ps2_bus_t* bus, u8 data)
{
	return bus->write_status(bus->dev, data);
}

u8 ps2_read_data(ps2_bus_t* bus)
{
	return bus->read_data(bus->dev);
}

void ps2_write_data(ps2_bus_t* bus, u8 data)
{
	bus->write_data(bus->dev, data);
}

void ps2_write_command(ps2_bus_t* bus, u8 data)
{
	bus->write_command(bus->dev, data);
}

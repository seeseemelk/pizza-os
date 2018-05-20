/**
 * This bus type allows communication with PS/2 devices.
 */

#ifndef BUS_PS2_H_
#define BUS_PS2_H_
#include "devices.h"
#include "cdefs.h"

typedef struct ps2_bus_t ps2_bus_t;
struct ps2_bus_t
{
	device_t* dev;
	u8 (*read_status)(device_t* dev);
	void(*write_status)(device_t* dev, u8 data);
	u8(*read_data)(device_t* dev);
	void(*write_data)(device_t* dev, u8 data);
	void(*write_command)(device_t* dev, u8 data);
};

/**
 * Reads the status from the PS/2 controller.
 * @param bus
 * @return The status word read.
 */
u8 ps2_read_status(ps2_bus_t* bus);

/**
 * Reads the data word from the PS/2 controller.
 * @param bus
 * @param The data word read
 */
u8 ps2_read_data(ps2_bus_t* bus);

/**
 * Writes a data word to the PS/2 controller.
 * @param bus
 * @param data The data word to write.
 */
void ps2_write_data(ps2_bus_t* bus, u8 data);

/**
 * Writes a command to the PS/2 device.
 * @param bus
 * @param data The command to send to the PS/2 device.
 */
void ps2_write_command(ps2_bus_t* bus, u8 data);

/**
 * Waits for the PS/2 device to send an interrupt.
 * @param bus The device to wait for.
 */
void ps2_wait(ps2_bus_t* bus);

#endif /* BUS_PS2_H_ */

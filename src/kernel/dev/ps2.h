/*
 * A general PS2 driver.
 */

#ifndef DEV_PS2_H_
#define DEV_PS2_H_

#include "devices.h"
#include "thread/lock.h"

typedef struct ps2ctrl_t
{
	int num_ports;
	lock_t lock;
} ps2ctrl_t;

/**
 * Reads the status register of a PS/2 controller.
 * @return The status register of the PS/2 controller.
 */
unsigned char ps2_read_status(ps2ctrl_t* dev);

/**
 * Reads the `data` register of a PS/2 controller.
 * @param dev The PS/2 controller to read from.
 * @return The data byte read.
 */
unsigned char ps2_read_data(ps2ctrl_t* dev);

/**
 * Writes a byte to the `data` register of the device.
 * @param dev The PS/2 controller to write to.
 * @param data The byte of data to write.
 */
void ps2_write_data(ps2ctrl_t* dev, unsigned char data);

/**
 * Writes a command byte to the device.
 * @param dev The PS/2 controller to write to.
 * @param command The command byte to write.
 */
void ps2_write_command(ps2ctrl_t* dev, unsigned char command);

/**
 * Gets the controller object of a PS/2 controller.
 * @param dev The device for which the get the controller object.
 * @return The PS/2 controller object.
 */
ps2ctrl_t* ps2_get_controller(const device_t* dev);

void ps2_init(void);

#endif /* DEV_PS2_H_ */

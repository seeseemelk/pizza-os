/**
 * @file
 * General interrupt management methods.
 * The architecture-dependent code should implement `arch_interrupt_init`
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "devices.h"

/**
 * Registers a device for handling a specific interrupt.
 * This will causes the device's event handler to receive
 * events for the specific interrupt.
 * @note Using `device` was chosen instead of `module` because a
 * `module_request` requires a `device` pointer.
 * @param dev The device that will be handling the interrupts.
 * @param interrupt The interrupt number to handle.
 */
void interrupt_register(device_t* dev, int interrupt);

/**
 * Ends the interrupt chain for a specific interrupt.
 * The function should be called if a device has determined that
 * it has to handle the interrupt and that the interrupt should
 * no longer be passed down to any other devices.
 * @param interrupt_number The uniquely identifying number of the interrupt.
 */
void interrupt_accept(int interrupt_id);

/**
 * @internal
 * Handles the platform-independent part of the interrupt.
 */
void interrupt_handle(int interrupt, int code);

/**
 * @internal
 * Initializes interrupts.
 * Should only be called by the kernel main function.
 */
void interrupt_init();

/**
 * @internal
 * @note To be implemented by arch-dependent code.
 * Lets the chosen architecture initialize interrupt.
 * This is called automatically by interrupt_init().
 */
void arch_interrupt_init();

/**
 * Enables interrupts
 * @note To be implement by arch-dependent code.
 */
void interrupt_enable();

/**
 * Disables interrupts
 * @note To be implement by arch-dependent code.
 */
void interrupt_disable();


#endif /* INTERRUPT_H_ */

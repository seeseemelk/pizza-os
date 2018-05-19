/**
 * @file
 * General interrupt management methods.
 * The architecture-dependent code should implement `arch_interrupt_init`
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "devices.h"
#include <stdbool.h>

/**
 * A driver that is processing an IRQ should return
 * one of these values in it's driver handle function
 * to indicate whether it has processed the IRQ, or
 * whether the interrupt was destined for a different
 * device.
 */
typedef enum
{
	INT_ACCEPT,
	INT_IGNORE
} irq_status_t;

/**
 * Contains information about an interrupt
 */
typedef struct
{
	int num;
	int error_code;
	bool finished;
} irq_t;

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
 * @param irq The IRQ to finish processing.
 */
void interrupt_finish(irq_t* irq);

/**
 * @internal
 * @note To be implemented by arch-dependent code.
 * Lets the chosen architecture finish handling the
 * interrupt.
 */
void arch_interrupt_finish();

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

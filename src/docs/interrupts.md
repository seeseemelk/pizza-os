##Interrupts {#Interrupts}
\brief Interrupts allow the kernel to respond to out-of-band requests from the hardware.

Interrupts allow the hardware to signal the kernel of any potential events.
They can also be triggered by software itself.

Interrupts can be handled in a device driver by including [interrupts.h](\ref kernel/interrupts.h)

The device driver needs to register that it wants to handle interrupts by calling `interrupts_register(device*, int)`.
The number of the interrupt to execute needs to be passed so it won't have to ignore a lot of other interrupts.

It should be important to note that the kernel cannot fully know what device caused a certain interrupt.
Therefore once a driver receives an interrupt it should check if it is really it's job to handle it.
If it is it's responsibility, it should it signify to the kernel by calling `interrupts_end(int)`.
A magic number has to be passed along that is received alongside the interrupt event itself.
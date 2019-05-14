# The PizzaOS micro-kernel
PizzaOS is a micro-kernel designed to be easy to understand and modify,
to run on computer systems with very low system requirements,
and that is harder to exploit by running many different components of a traditional kernel as userspace programs.

## What does the kernel do?
The kernel's job is only to manage several critical system components so that userspace programs can manage the rest.
The components that the kernel manages are:

- Physical Memory
- Virtual memory (a.k.a. paging)
- I/O Access
- IRQs
- CPU Time and process scheduling
- The virtual filesystem (but not the filesystem implementations)
- Access control to all of the above

## What will the kernel not do?
The kernel won't do any of the following things.
These tasks are for userspace programs to manage.

- Manage the DMA controller.
- Enumerate your PCI and USB devices.
- Talk to other computers over Ethernet.
- Play your favourite music on your shiny new Bluetooth headset.
- Cook dinner for you.
- And many, many more things...

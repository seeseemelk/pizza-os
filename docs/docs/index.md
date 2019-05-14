# The PizzaOS micro-kernel
![logo](logo.png)

PizzaOS is a micro-kernel designed to be easy to understand and modify,
to run on computer systems with very low system requirements,
and that is harder to exploit by running many different components of a traditional kernel as userspace programs.

## What does the kernel do?
The kernel's job is only to manage several critical system components so that userspace programs can manage the rest.
The components that the kernel manages are:

- [The Boot Process](/overview/booting)
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

## Rationale
The reason a micro-kernel design was chosen is because it allows for a much cleaner kernel.
As the kernel itself doesn't contain drivers, it can be kept incredibely small.
This allows makes it possible to have very light and fast memory allocation schemes within the kernel.

For a long time it was believed that micro-kernels are much slower than monolithic kernels because ever time a program wants to acces a driver, the system needs to perform a slow context switch.
A monolithic kernel does not suffer from this.
But with the discovery of Meltdown and Spectre, a monolithic kernel will have to perform context switches anyway in order to keep private data in the kernel safe.

Because the PizzaOS kernel only controls a very small part of the system, it is feasible to make sure that no secure data is kept in kernelspace, only in userspace.
No extra context-switches required to keep safe from certain attacks.

## System Requirements
PizzaOS is designed to be able to run on an Intel 386 with 4 megabytes of memory.
Why?
Because I (the project creator) own an ancient 386/DX-33 with 8 megabytes of memory.
The newest version of Linux that runs on this system is kernel 3.8.
Anything newer than that simply won't support it.
And although that system does have 8 megabytes of RAM,
I'd like to have a few megabytes of RAM left for programs.

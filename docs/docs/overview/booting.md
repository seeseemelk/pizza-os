# The Boot Process
The boot process for PizzaOS is fairly simple, and essentially boils down to these steps:

1. [Receive control from the bootloader.](/overview/bootloader)
2. Initialise the CPU and memory.
3. Load the Master Control Program (MCP) from the ramdisk given by the bootloader.
4. Start executing the MCP.
5. The MCP will start other deamons in the ramdisk.
6. The MCP passes control to the user.

First we'll discuss how a bootloader should be configure for PizzaOS, and how control is passed to it.

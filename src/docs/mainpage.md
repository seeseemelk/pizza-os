## PizzaOS Main Documentation Page
# Directory structure
PizzaOS maintains a relatively simple directory structure.
 - / - Contains all files that are too important or too commonly used to go into /api
 - /arch - Contains architecture dependent files.
 - /api - Contains APIs that do *not* communicate with device drivers.
 - /bus - Contains APIs to communicate with device drivers.
 - /collections - Contains objects such as dynamically-resizable array.
 - /dev - Contains common (non-architecure specific) device drivers.
 - /thread - Contains objects such as locks, required for implementing thread-safety.

# Kernel
Several general methods that do not fit in any specific category can be found in kernel/kernel.h.

# IO
General IO can be performed by including kernel/io.h.

# Interrupts
Interrupts are handled by kernel/interrupts.h.
A high-level overview of this can be found at \ref Interrupts.

# Virtual Filesystem
Information about the virtual filesystem can be found at \ref VFS. 
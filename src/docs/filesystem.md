# Virtual Filesystem {#VFS}
## Introduction
The virtual filesystem of PizzaOS resembles that of most UNIX-like operating systems.

The VFS is split up into two parts: the VFS itself and the filesystem bus.

### The VFS
The VFS manages filesystem mounts, and directory and file descriptors.
The descriptors are structs that contain data needed to perform actions on directories and files,
but they are completely internal to the VFS.
When a file or directory is opened, an integer is returned. This integer is used by the VFS
to point to the descriptor.
These integers are typedef'ed as `FILE` and `DIR`

### Filesystem bus
The filesystem bus is the part that implements a filesystem backend.
A filesystem can choose how it manages files and directories, but it
has to give each directory and file a distinct number, called the inode.
All calls to a filesystem pass the inode of the file or directory to operate on,
except for `get_inode`.
Calls to `get_inode` get the path relative to the mountpoint of the filesystem
passed along, and they should return the inode of the file.
The inode cannot be shared with any other file, and it should stay consistent to
that path, unless the path is deleted or otherwise changed.

Note that calls to `get_inode` with the same path can return, and even must, return
different inodes if the path was deleted. But if the file is deleted, the filesystem
is not to remove to free the file clusters. This is so that already opened instances
of the file can still read from it.
The clusters can only be permanently freed after `free_inode` has been called.
At that point the inode can be reused again by `get_inode`, and any filesystem
calls that use the freed inode (unless it has been reused) should be considered
bugs in the VFS.

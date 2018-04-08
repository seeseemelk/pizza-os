/**
 * @file
 * Contains CPU IO related functions.
 */

#ifndef IO_H_
#define IO_H_

#include "config.h"

/**
 * Writes a byte to an IO port.
 * Writes a byte to one of the CPU's IO ports.
 * @param port The port to write the byte of data too.
 * @param value The value to write to the port.
 */
void outb(u16 port, u8 value);

/**
 * Writes a word (2 bytes) to an IO port.
 * Writes a word to one of the CPU's IO ports.
 * @param port The port to write the word of data too.
 * @param value The value to write to the port.
 */
void outw(u16 port, u16 value);

/**
 * Writes a long (4 bytes) to an IO port.
 * Writes a long to one of the CPU's IO ports.
 * @param port The port to write the long of data too.
 * @param value The value to write to the port.
 */
void outl(u16 port, u32 value);

/**
 * Reads a byte of data from an IO port.
 * Reads a byte of data from one of the CPU's IO ports.
 * @param port The port to read from.
 * @return The byte read.
 */
u8 inb(u16 port);

/**
 * Reads a word (2 bytes) of data from an IO port.
 * Reads a word of data from one of the CPU's IO ports.
 * @param port The port to read from.
 * @return The word read.
 */
u16 inw(u16 port);

/**
 * Reads a long (8 bytes) of data from an IO port.
 * Reads a long of data from one of the CPU's IO ports.
 * @param port The port to read from.
 * @return The long read.
 */
u32 inl(u16 port);

#if TARGET==i386
#include <arch/i386/i386_io.h>
#endif

#endif /* IO_H_ */















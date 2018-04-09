#include "thread.h"
#include "kernel.h"
#include "cdefs.h"
#include "kernel.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct thread_data thread_data;

struct thread_data
{
	u16 ds;
	u16 ss;
	u16 cs;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	void* stack;
} __attribute__((packed));

void thread_save(thread_data* data)
{
	asm volatile (
			/*"movl %%eax, %0;"
			"movl %%ebx, %1;"
			"movl %%ecx, %2;"
			"movl %%edx, %3;"*/
			"movl %%esp, %4;"
			"movl %%ebp, %5;"
			: "=m" (data->eax), "=m" (data->ebx), "=m" (data->ecx), "=m" (data->edx), "=m" (data->esp),
			  "=m" (data->ebp)
	);
	asm volatile (
			"movl %%esi, %0;"
			"movl %%edi, %1;"
			"movw %%ds, %%ax; movw %%ax, %2;"
			"movw %%ss, %%ax; movw %%ax, %3;"
			"movw %%cs, %%ax; movw %%ax, %4;"
			"movl $1f, %5;"
			"1:"
			: "=m" (data->esi), "=m" (data->edi), "=m" (data->cs), "=m" (data->ss), "=m" (data->ds),
			  "=m" (data->eip)
	);
	//data->eip = (u32) __builtin_return_address(0);
}

void empty_func()
{
}

void thread_enter(thread_data* data)
{
	asm volatile (
			// Restore EBP and ESP so we can use the stack.
			"movl 30(%0), %%eax;"
			"movl %%eax, %%esp;"
			"movl 34(%0), %%eax;"
			"movl %%eax, %%ebp;"

			// Restore DS and SS
			"movl 0(%0), %%eax;"
			"movl %%eax, %%ds;"
			"movl 2(%0), %%eax;"
			"movl %%eax, %%ss;"

			// Load the values so we can load with pushal
			"movl %0, %%eax;"
			"add $4, %%eax;"
			//"pushw (%%eax);"
			"add $2, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"popal;" // Restore the values now from the stack.
			"popfl;"

			// Perform far jump
			"ret;"
			:
			: "r" (data)
			: "eax"

	);
}

thread_data thread_one;
thread_data thread_two;

void other_thread()
{
	kprintf("It works!\n");
	thread_enter(&thread_two);
	//while (1);
}

void thread_init()
{
	thread_save(&thread_one);
	thread_save(&thread_two);
	thread_one.stack = malloc(4096);
	thread_one.eip = (u32) &other_thread;
	thread_one.ebp = (u32)((char*)thread_one.stack + 4092);
	thread_one.esp = (u32)((char*)thread_one.stack + 4092);
	thread_enter(&thread_one);
}


















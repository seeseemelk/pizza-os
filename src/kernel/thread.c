#include "thread.h"
#include "kernel.h"
#include "cdefs.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct thread_data thread_data;

struct thread_data
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 ebp;
	u32 esp;
	u32 eip;
	u32 edi;
	u32 esi;
	u32 eflags;
	u16 cs;
	u16 ds;
	u16 ss;
	void* stack;
};

void thread_write_data(thread_data* data, u32 eip)
{
	register u32 eax asm("eax");
	register u32 ebp asm("ebp");
	register u32 resp asm("esp");
	/*register u16 rcs asm("cs");
	register u16 rds asm("ds");
	register u16 rss asm("ss");*/
	data->eax = eax;
	data->ebp = ebp;
	data->esp = resp;
	data->eip = eip;
	/*data->cs = rcs;
	data->ds = rds;
	data->ss = rss;*/
}

thread_data thread_one;
//thread_data thread_two;

void empty_func()
{
}

void other_thread()
{

}

void thread_resume(thread_data* data)
{

}

void thread_init()
{
	thread_one.stack = malloc(4096);
	thread_one.eip = &other_thread;
	thread_one.ebp = thread_one.stack;
	thread_one.esp = thread_one.stack;
	thread_resume(&thread_one);
	//thread_write_data(thread_two, &other_thread);
	//thread_write_data(thread_one, )
}


















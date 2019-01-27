#ifndef SLAB_HPP_
#define SLAB_HPP_

#include "paging.hpp"
#include "result.hpp"
#include "pmem.hpp"
#include "cpu.hpp"
#include "result.hpp"
#include "debug.hpp"
#include <cstdint>

template<typename T>
class Slab
{
public:
	Slab();
	Slab(Paging::PageTable& table);
	Slab(Paging::PageTable& table, size_t element_size);
	Result<T*> alloc();
	void free(T& element);

private:
	size_t& peek();
	void push(size_t);
	Result<size_t> pop();
	ResultState need_memory_at(void*);

	Paging::PageTable* m_table;
	size_t m_element_size;
	void* m_elements;
	size_t m_capacity;
	size_t* m_free;
	size_t m_free_index;
	size_t m_free_created;
};

template<typename T>
Slab<T>::Slab()
	: m_table(nullptr), m_element_size(0), m_elements(0), m_capacity(0), m_free(0), m_free_index(0), m_free_created(0)
{
}

template<typename T>
Slab<T>::Slab(Paging::PageTable& table)
	: Slab(table, sizeof(T))
{
}

template<typename T>
Slab<T>::Slab(Paging::PageTable& table, size_t element_size)
	: m_table(&table), m_element_size(element_size)
{
	if (element_size == 0)
	{
		log("Element size for Slab allocator is 0");
		CPU::hang();
	}
	m_elements = table.get_address();
	m_capacity = MB(4) / (element_size + sizeof(size_t));

	m_free = reinterpret_cast<size_t*>(static_cast<u8*>(m_elements) + MB(4)) - m_capacity;
	m_free_index = m_capacity - 1;
	m_free_created = m_free_index;
}

template<typename T>
Result<T*> Slab<T>::alloc()
{
	if (m_free_index != m_capacity)
	{
		Result<size_t> result = pop();
		if (result.is_success())
		{
			T* element = reinterpret_cast<T*>(static_cast<u8*>(m_elements) + result.result);
			if (need_memory_at(element) == ResultState::SUCCESS)
				return Result<T*>(element);
		}
	}
	return Result<T*>();
}

template<typename T>
void Slab<T>::free(T& element)
{
	void* ptr = &element;
	//size_t index = reinterpret_cast<size_t>(static_cast<u8*>(m_elements) - static_cast<u8*>(ptr));
	size_t index = static_cast<u8*>(ptr) - static_cast<u8*>(m_elements);
	push(index);
}

template<typename T>
size_t& Slab<T>::peek()
{
	return m_free[m_free_index];
}

template<typename T>
void Slab<T>::push(size_t num)
{
	m_free_index++;
	// Although we use dynamic memory here, it should already have been allocated by pop().
	peek() = num;
}

template<typename T>
Result<size_t> Slab<T>::pop()
{
	size_t* value;
	if (m_free_created == m_free_index)
	{
		ResultState state = need_memory_at(m_free + m_free_index);
		if (state == ResultState::FAIL)
			return Result<size_t>();
		log("Address: 0x%X", m_free + m_free_index);
		m_free[m_free_index] = (m_capacity - (m_free_created--) - 1) * m_element_size;
	}

	value = &peek();
	m_free_index--;
	return Result<size_t>(*value);
}

template<typename T>
ResultState Slab<T>::need_memory_at(void* address)
{
	Paging::PageTableEntry& entry = m_table->get_entry(address);
	if (entry.present)
		return ResultState::SUCCESS;

	Result<void*> result = PMem::alloc_end(KB(4));
	if (!result.is_success())
		return result.state;

	entry.set_address(reinterpret_cast<size_t>(result.result));
	entry.userspace = 0;
	entry.writable = 1;
	entry.present = 1;
	return ResultState::SUCCESS;
}

#endif











#ifndef SLAB_HPP_
#define SLAB_HPP_

#include "paging.hpp"
#include "result.hpp"
#include "pmem.hpp"
#include "cpu.hpp"
#include "result.hpp"
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
	size_t m_free_index = 0;
	size_t m_free_created = 0;
};

template<typename T>
Slab<T>::Slab()
	: m_table(nullptr), m_element_size(0), m_elements(0), m_capacity(0), m_free(0)
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
	m_elements = table.get_address();
	m_capacity = MB(4) / (element_size + sizeof(size_t));
	m_free = reinterpret_cast<size_t*>(static_cast<u8*>(m_elements) + MB(4) - sizeof(size_t));
}

template<typename T>
Result<T*> Slab<T>::alloc()
{
	if (m_free_index != m_capacity)
	{
		Result<size_t> result = pop();
		if (result.is_success())
		{
			void* element = static_cast<u8*>(m_elements) + result.result;
			if (need_memory_at(element) == ResultState::SUCCESS)
				return Result<T*>(reinterpret_cast<T*>(&element));
		}
	}
	return Result<T*>();
}

template<typename T>
void Slab<T>::free(T& element)
{
	void* ptr = *element;
	size_t index = reinterpret_cast<size_t>(static_cast<u8*>(m_elements) - static_cast<u8*>(ptr));
	push(index);
}

template<typename T>
size_t& Slab<T>::peek()
{
	return *(m_free - m_free_index);
}

template<typename T>
void Slab<T>::push(size_t num)
{
	m_free_index++;
	peek() = num;
	/*
	if (need_memory_at(&peek()) == ResultState::SUCCESS)
	{
		peek() = num;
		return ResultState::SUCCESS;
	}
	else
		return ResultState::FAIL;
	*/
}

template<typename T>
Result<size_t> Slab<T>::pop()
{
	if (m_free_created == m_free_index)
	{
		ResultState state = need_memory_at(m_free - m_free_index);
		if (state == ResultState::FAIL)
			return Result<size_t>();
		*(m_free - m_free_index) = (m_free_created++) * m_element_size;
	}

	size_t& value = peek();
	m_free_index--;
	return Result<size_t>(value);
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











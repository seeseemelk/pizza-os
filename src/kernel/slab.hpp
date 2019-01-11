#ifndef SLAB_HPP_
#define SLAB_HPP_

#include "paging.hpp"
#include "result.hpp"
#include "pmem.hpp"
#include "cpu.hpp"
#include <cstdint>

template<typename T>
class Slab
{
public:
	Slab(Paging::PageTable& table);
	Result<T> alloc();
	void free(T& element);

private:
	ResultState push(size_t);
	size_t pop();
	ResultState need_memory_at(size_t);

	Paging::PageTable& m_table;
	size_t m_capacity;
	T m_elements[];
	size_t m_free[];
	size_t m_free_index = 0;
};

template<typename T>
Slab<T>::Slab(Paging::PageTable& table)
	: m_table(table)
{
	m_capacity = MB(4) / (sizeof(T) + sizeof(size_t));

	m_elements = table.get_address();
	m_free = m_elements + MB(4) - sizeof(size_t);
}

template<typename T>
Result<T> Slab<T>::alloc()
{
	if (m_free_index >= 1)
	{
		T* element = m_elements[pop()];
		if (need_memory_at(element).is_success())
			return Result<T>(element);
	}
	return Result<T>();
}

template<typename T>
void Slab<T>::free(T& element)
{
	T* ptr = *element;
	size_t index = m_elements - ptr;
	push(index);
}

template<typename T>
ResultState Slab<T>::push(size_t num)
{
	if (need_memory_at(m_free[m_free_index]) == ResultState::SUCCESS)
	{
		m_free[m_free_index--];
		return ResultState::SUCCESS;
	}
	else
		return ResultState::FAIL;
}

template<typename T>
size_t Slab<T>::pop()
{
	return m_free[++m_free_index];
}

template<typename T>
ResultState Slab<T>::need_memory_at(size_t address)
{
	Paging::PageTableEntry& entry = m_table.get_entry(address);
	if (entry.present)
		return;

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











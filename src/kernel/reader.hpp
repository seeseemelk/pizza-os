#ifndef READER_HPP_
#define READER_HPP_

#include <cstddef>
#include <ramdisk.hpp>

class Reader // @suppress("Class has a virtual method and non-virtual destructor")
{
public:
	enum OffsetType
	{
		CURRENT,
		BEGINNING,
		ENDING
	};

	virtual size_t read(char* buf, size_t len) = 0;
	virtual size_t seek(OffsetType type, size_t address) = 0;
};

class TarReader : public Reader // @suppress("Class has a virtual method and non-virtual destructor")
{
public:
	TarReader(const RamDisk::TarFile* file);
	size_t read(char* buf, size_t len) final;
	size_t seek(OffsetType type, size_t address) final;
	size_t remaining();

private:
	const RamDisk::TarFile* m_file;
	const size_t m_size;
	size_t m_position = 0;
};

#endif

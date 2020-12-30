#include "libc/string.hpp"
#include "ramdisk.hpp"
#include "reader.hpp"

TarReader::TarReader(const RamDisk::TarFile* file)
	: m_file(file), m_size(file->get_size())
{
}

size_t TarReader::remaining()
{
	return m_size - m_position;
}

size_t TarReader::read(char* buf, size_t len)
{
	if (len > remaining())
		len = remaining();

	memcpy(buf, m_file->buffer() + m_position, len);
	m_position += m_position;
	return len;
}

size_t TarReader::seek(OffsetType type, size_t address)
{
	switch (type)
	{
		case OffsetType::BEGINNING:
			m_position = address;
			break;
		case OffsetType::CURRENT:
			m_position += address;
			break;
		case OffsetType::ENDING:
			m_position = m_size - address;
			break;
	}
	return m_position;
}

/*	virtual ~TarReader();
	virtual size_t read(char* buf, size_t len);
	virtual void seek(OffsetType type, size_t address);
};*/

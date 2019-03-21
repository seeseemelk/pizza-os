#ifndef READER_HPP_
#define READER_HPP_

#include <cstddef>

class Reader
{
public:
	enum OffsetType
	{
		CURRENT,
		BEGINNING,
		ENDING
	};

	virtual ~Reader();
	virtual size_t read(char* buf, size_t len);
	virtual void seek(OffsetType type, size_t address);
};

#endif

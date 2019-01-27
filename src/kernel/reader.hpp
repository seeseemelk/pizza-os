#ifndef READER_HPP_
#define READER_HPP_

#include <cstddef>

class Reader
{
public:
	virtual ~Reader();
	virtual size_t read(char* buf, size_t len);
};

#endif

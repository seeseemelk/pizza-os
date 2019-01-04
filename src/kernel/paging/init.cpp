#include "paging.hpp"
#include <cmath>

using namespace Paging;

extern "C" PageDirectory _system_page_directory;
PageDirectory& Paging::directory = _system_page_directory;

void Paging::init()
{

}

size_t Paging::dir_index(size_t virt)
{
	return ceildiv(virt, MB(4));
}

size_t Paging::tbl_index(size_t virt)
{
	return ceildiv(virt % MB(4), KB(4));
}

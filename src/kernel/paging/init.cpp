#include "paging.hpp"
#include <cmath>

using namespace Paging;

extern "C" PageDirectory _system_page_directory;
PageDirectory& Paging::directory = _system_page_directory;

void Paging::init()
{

}

bool PageDirectory::has_table(size_t virt)
{
	size_t dir_i = ceildiv(virt, MB(4));
	PageDirEntry& table = directory.entries[dir_i];
	return table.present;
}

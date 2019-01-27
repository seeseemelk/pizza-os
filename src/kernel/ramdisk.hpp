#ifndef RAMDISK_HPP_
#define RAMDISK_HPP_

#include "result.hpp"
#include <cstddef>

namespace RamDisk
{
	struct TarFile
	{
		// v7 header fields
		char filename[100];
		char filemode[8];
		char uid[8];
		char gid[8];
		char size[12];
		char modtime[12];
		char checksum[8];
		char type_flag;
		char link_file[100];

		// UStar header fields
		char ustar_indicator[6];
		char ustar_version[2];
		char owner_name[32];
		char owner_group[32];
		char dev_major[8];
		char dev_minor[8];
		char filename_prefix[155];

		size_t get_size() const;
	} __attribute__((packed));

	enum TypeFlag
	{
		NORMAL = '0',
		HARD_LINK = '1',
		SYM_LINK = '2',
		CHAR_SPECIAL = '3',
		BLOCK_SPECIAL = '4',
		DIRECTORY = '5',
		FIFO = '6',
		CONT_FILE = '7',
		GLOB_EXT_HEADER = 'g',
		EXT_HEADER = 'x'
	};

	void init();
	void init_ramdisk(const void* ramdisk, size_t length);
	bool has_file(const char* filename);
	Result<const TarFile*> get_file(const char* filename);
	//ResultState copy_file(TarFile* file, void* dest);
}

#endif

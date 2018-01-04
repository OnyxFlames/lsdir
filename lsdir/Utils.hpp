#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "ArgumentHandler.hpp"

enum magnitude : uintmax_t
{
	byte = 1024,
	kilobyte = byte * 1000,
	megabyte = kilobyte * 1000,
	gigabyte = megabyte * 1000,
	terabyte = gigabyte * 1000
};

const std::vector<std::string> list_drives();

const std::string to_shortsuffix(uintmax_t num, unsigned short divisor = byte);
const std::string to_longsuffix(uintmax_t num, unsigned short divisor = byte);

const double to_smallestmagnitude(uintmax_t num, unsigned short divisor = byte);

void list_dir(const std::string dir, FlagStruct& fs);
void list_dir_r(const std::string dir, FlagStruct& fs, size_t depth = 0);
void show_drive(const std::string drive, FlagStruct& fs);
void show_size(const std::string file, FlagStruct& fs);
void diff_files(const std::string first, const std::string second);
void search_dir(const std::string term, const std::string dir);
void regex_dir(const std::string pattern, const std::string dir);
void resize_file(const std::string file, uintmax_t size);
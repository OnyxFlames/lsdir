#pragma once

#include <cstdint>
#include <string>
#include <vector>

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
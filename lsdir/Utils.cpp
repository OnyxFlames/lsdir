#include "Utils.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

const std::vector<std::string> list_drives()
{
	std::vector<std::string> drives;
	//Windows drives
	for (char d = 'A'; d != 'Z'; d++)
	{
		std::string str = "";
		str = d;
		str += ":";
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	// Linux drives
	for (char d = 'a'; d != 'z'; d++)
	{
		std::string str = "sd";
		str += d;
		str += "1";
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	for (char d = 'a'; d != 'z'; d++)
	{
		std::string str = "hd";
		str += d;
		str += "1";
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	return drives;
}

const std::string to_shortsuffix(uintmax_t num)
{
	uint16_t magnitude = 0;
	uintmax_t size = num;
	uintmax_t counter = size;

	//TODO: figure out if files are 1000's or 1024's
	const unsigned short divisor = 1024;

	if (size > divisor)
	{
		magnitude++;
		while ((size /= divisor) >= divisor)
		{
			magnitude++;
		}
	}
	switch (magnitude)
	{
		case 0: return "B"; break;
		case 1: return "KB"; break;
		case 2: return "MB"; break;
		case 3: return "GB"; break;
		case 4: return "TB"; break;
		case 5: return "PB"; break;
		default: return "Unknown\n"; break;
	}
}

const std::string to_longsuffix(uintmax_t num)
{
	uint16_t magnitude = 0;
	uintmax_t size = num;
	uintmax_t counter = size;

	//TODO: figure out if files are 1000's or 1024's
	const unsigned short divisor = 1024;

	if (size > divisor)
	{
		magnitude++;
		while ((size /= divisor) >= divisor)
		{
			magnitude++;
		}
	}
		switch (magnitude)
		{
			case 0: return "byte(s)"; break;
			case 1: return "kilobyte(s)"; break;
			case 2: return "megabyte(s)"; break;
			case 3: return "gigabyte(s)"; break;
			case 4: return "terabyte(s)"; break;
			case 5: return "petabyte(s)"; break;
			default: return "Unknown"; break;
		}
}

const uintmax_t to_smallestmagnitude(uintmax_t num)
{
	uint16_t magnitude = 0;
	uintmax_t size = num;
	uintmax_t counter = size;

	//TODO: figure out if files are 1000's or 1024's
	const unsigned short divisor = 1024;

	if (size > divisor)
	{
		magnitude++;
		while ((size /= divisor) >= divisor)
		{
			magnitude++;
		}
	}
	return size;
}

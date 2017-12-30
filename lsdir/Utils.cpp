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
		//str += ":";
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	for (char d = 'a'; d != 'z'; d++)
	{
		std::string str = "hd";
		str += d;
		//str += ":";
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	return drives;
}

const std::string to_shortsuffix(uintmax_t num)
{
	return std::string();
}

const std::string to_longsuffix(uintmax_t num)
{
	return std::string();
}

const uintmax_t to_smallestmagnitude(uintmax_t num)
{
	return uintmax_t();
}

#include "Utils.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#define DEBUG(x) std::printf("Debug: %s\n", x);

#include <regex>
#include <fstream>
#include <iostream>

#define LSDIR_COLOR

#ifdef LSDIR_COLOR
const char* fg_reset = "\033[0m";
const char* fg_red = "\033[31m";
const char* fg_green = "\033[32m";
const char* fg_yellow = "\033[33m";
const char* fg_blue = "\033[34m";
const char* fg_magenta = "\033[35m";
const char* fg_cyan = "\033[36m";
const char* fg_white = "\033[37m";
const char* fg_brightred = "\033[91m";
const char* fg_brightgreen = "\033[92m";
const char* fg_brightyellow = "\033[93m";
const char* fg_brightblue = "\033[94m";
const char* fg_brightmagenta = "\033[95m";
const char* fg_brightcyan = "\033[96m";
#else
const char* fg_reset = "";
const char* fg_red = "";
const char* fg_green = "";
const char* fg_yellow = "";
const char* fg_blue = "";
const char* fg_magenta = "";
const char* fg_cyan = "";
const char* fg_white = "";
const char* fg_brightred = "";
const char* fg_brightgreen = "";
const char* fg_brightyellow = "";
const char* fg_brightblue = "";
const char* fg_brightmagenta = "";
const char* fg_brightcyan = "";
#endif

const std::vector<std::string> list_drives()
{
	std::vector<std::string> drives;
	//Windows drives
	for (char d = 'A'; d <= 'Z'; d++)
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
	for (char d = 'a'; d <= 'z'; d++)
	{
		std::string str = "/dev/sd";
		str += d;
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
		str = "/dev/hd";
		str += d;
		if (fs::exists(str))
		{
			drives.push_back(str);
		}
	}
	return drives;
}

const std::string to_shortsuffix(uintmax_t num, unsigned short divisor)
{
	uint16_t magnitude = 0;
	uintmax_t size = num;
	uintmax_t counter = size;

	//TODO: figure out if files are 1000's or 1024's
	//const unsigned short divisor = 1024;

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

const std::string to_longsuffix(uintmax_t num, unsigned short divisor)
{
	uint16_t magnitude = 0;
	uintmax_t size = num;
	uintmax_t counter = size;

	//TODO: figure out if files are 1000's or 1024's
	//const unsigned short divisor = 1024;

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

const double to_smallestmagnitude(uintmax_t num, unsigned short divisor)
{
	uint16_t magnitude = 0;
	double size = static_cast<double>(num);
	uintmax_t counter = static_cast<uintmax_t>(size);

	//TODO: figure out if files are 1000's or 1024's
	//const unsigned short divisor = 1024;

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

void list_dir(const std::string dir, FlagStruct& fs)
{
	for (auto &d : fs::directory_iterator(dir))
	{
		if (d.path().filename().string()[0] != '.' || fs.verbose)	// hidden file
		{
			std::cout << fg_brightcyan << d.path().filename() << fg_reset << '\t';
		}
	}
}

void list_dir_r(const std::string dir, FlagStruct& fs, size_t depth)
{
	for (auto& d : fs::directory_iterator(dir))
	{
		if (d.path().filename().string()[0] != '.' || fs.verbose)	// hidden file
		{
			std::cout << fg_brightblue << std::string(depth, '-') << fg_reset;
			if (depth > 0)
				std::cout << fg_white << '|';
			if (d.path().string()[0] == '.')
				std::cout << fg_brightcyan << d.path().filename() << '\n';
			else
				std::cout << fg_brightcyan << d.path().filename() << fg_reset << '\n';
			if (fs::is_directory(dir))
			{
				list_dir_r(d.path().string(), fs, depth + 1);
			}
		}
	}
}

void show_drive(const std::string drive, FlagStruct& fs)
{
	if (fs::exists(drive))
	{
		fs::space_info si;
		si = fs::space(drive);
		std::cout << "Drive '" << fs::path(drive) << "'\t\n"
			<< fg_cyan << "\tAvailable:\t" << fg_brightcyan << to_smallestmagnitude(si.available) << ' ' << to_longsuffix(si.available) << fg_reset << "\n"
			<< fg_cyan << "\tCapacity:\t" << fg_brightcyan << to_smallestmagnitude(si.capacity) << ' ' << to_longsuffix(si.capacity) << fg_reset << "\n"
			<< fg_cyan << "\tFree:\t\t" << fg_brightcyan << to_smallestmagnitude(si.free) << ' ' << to_longsuffix(si.free) << fg_reset << "\n";
		if (si.free != si.available)
			std::cout << fg_red << "\tUnavailable:\t" << fg_brightred << (si.free - si.available) << " byte(s)\n";
	}
	else
	{
		std::cout << "Not a drive.";
		if (list_drives().size() > 0)
			std::cout << " Found drives: \n\t";
		else
			std::cout << "\n";
		for (const auto d : list_drives())
			std::cout << d << "\t";
	}
}

void show_size(const std::string file, FlagStruct& fs)
{
	if (!fs::exists(file)
		|| fs::is_directory(file))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << "Not a file.\n";
		std::exit(1);
	}
	std::cout << fg_brightcyan << to_smallestmagnitude(fs::file_size(file)) << ' ' << to_longsuffix(fs::file_size(file)) << '\n';
	if (fs.verbose)
		std::cout << "(" << fs::file_size(file) << " bytes)\n";
}

void diff_files(const std::string first, const std::string second)
{
	if (!fs::exists(first))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << first << " doesn't exist or is a directory.\n" << fg_reset;
		return;
	}
	if (!fs::exists(second))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << second << " doesn't exist or is a directory.\n" << fg_reset;
		return;
	}

	std::ifstream first_file(first), second_file(second);
	if (!first_file.is_open() || !second_file.is_open())
	{
		std::cerr << fg_red << "Error: " << fg_brightred << second << "" << fg_reset;
	}
	int f, s;
	size_t addr = 0;
	size_t max_diff = 100;
	std::string diff = "";
	std::vector<size_t> diff_addr;
	std::vector<std::string> diffs;
	while (max_diff > 0)
	{
		f = first_file.get();
		s = second_file.get();
		if (f != s)
		{
			if(isalnum(s))
				diff += s;
		}
		else if (diff != "")
		{
			diffs.push_back(diff);
			diff_addr.push_back(addr);
			diff = "";
			max_diff--;
		}
		addr++;
		if (first_file.eof() && second_file.eof())
			break;
	}
	for (size_t d = 0; d < diffs.size(); d++)
	{
		std::cout << fg_green << "0x" << std::hex << diff_addr[d] << std::dec << fg_brightgreen << " '" << diffs[d] << "'\n";
	}
}

void search_dir(const std::string term, const std::string dir)
{
	if (!fs::exists(dir) || !fs::is_directory(dir))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << dir << " doesn't exist or is a file!\n";
	}
	else
	{
		size_t found_count = 0;
		bool limit_reached = false;
		std::vector<std::string> results;
		for (const auto& res : fs::recursive_directory_iterator(dir))
		{
			if (res.path().filename().string().find(term) != std::string::npos)
			{
				found_count++;
				if (results.size() < 50)
					results.push_back(res.path().filename().string());
				else
					limit_reached = true;
			}
		}
		std::cout << fg_green << "Searching for: " << fg_brightgreen << "'" << term << "' in '" << dir
			<< "'\n" << fg_green << "Found " << "(" << found_count << "):\n";
		for (const auto& ret : results)
		{
			std::cout << '\t' << fg_brightcyan << ret << "\n";
		}
		if (limit_reached)
			std::cout << fg_green << "Result limit reached.\n" << fg_reset;
	}
}

void regex_dir(const std::string pattern, const std::string dir)
{
	if (!fs::exists(dir) || !fs::is_directory(dir))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << dir << " doesn't exist or is a file!\n";
	}
	else
	{
		
		size_t found_count = 0;
		bool limit_reached = false;
		std::vector<std::string> results;
		for (const auto& res : fs::recursive_directory_iterator(dir))
		{
			try
			{
				auto r_pattern = std::regex(pattern);
				if (std::regex_search(res.path().filename().string().c_str(), r_pattern))
				{
					found_count++;
					if (results.size() < 50)
						results.push_back(res.path().filename().string());
					else
						limit_reached = true;
				}
			}
			catch (std::regex_error e)
			{
				std::cerr << fg_red << "Error: " << fg_brightred << " Invalid regex pattern.\n" << fg_reset;
				return;
			}
		}
		std::cout << fg_green << "Using pattern: " << fg_brightgreen << "'" << pattern << "' in '" << dir
			<< "'\n" << fg_green << "Found " << "(" << found_count << "):\n";
		for (const auto& ret : results)
		{
			std::cout << '\t' << fg_brightcyan << ret << "\n";
		}
		if (limit_reached)
			std::cout << fg_green << "Result limit reached.\n" << fg_reset;
	}
}

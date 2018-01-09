#include "Utils.hpp"

#include "ColorTable.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#define DEBUG(x) do { std::printf(fg_brightyellow"Debug: %s\n", x); printf(fg_reset); } while(false);
#define DEBUGN(x) do { std::printf(fg_brightyellow"Debug: %20jd\n", x); printf(fg_reset); } while(false);
#define DEBUGF(x) do { std::printf(fg_brightyellow"Debug: %f\n", x); printf(fg_reset); } while(false);

#include <regex>
#include <fstream>
#include <iostream>
#include <cctype>
#include <iomanip>

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

uintmax_t to_byte_value(const std::string magnitude_string)
{
	char mag_char = magnitude_string.at(magnitude_string.size() - 1);
	// If the last character is a digit then the magnitude is represented as a value in bytes,
	// so we don't need to do anything to it.
	if (!std::isalpha(mag_char))
	{
		return std::stoll(magnitude_string);
	}
	else if (std::isalpha(mag_char))
	{
		switch (std::toupper(mag_char))
		{
		case 'B':
			return std::stoll(magnitude_string);
			break;
		case 'K':
			return std::stoll(magnitude_string) * kilobyte;
			break;
		case 'M':
			return std::stoll(magnitude_string) * megabyte;
			break;
		case 'G':
			return std::stoll(magnitude_string) * gigabyte;
			break;
		case 'T':
			return std::stoll(magnitude_string) * terabyte;
			break;
		default:
			std::cerr << fg_red << "Error: " << fg_brightred << " the suffix of byte value isn't recognized.\n";
			std::exit(1);
			return 0;
			break;
		}
	}
	return -1;
}

const std::string get_drive_avail_color(float _val)
{
	if (_val > 100 || _val < 0)
		return "INVALID DRIVE STATUS";
	else if (_val > 25)
		return fg_brightgreen;
	else if (_val > 10)
		return fg_yellow;
	else
		return fg_brightred;
}

const std::string get_drive_used_color(float _val)
{
	if (_val > 100 || _val < 0)
		return "INVALID DRIVE STATUS";
	else if (_val > 90)
		return fg_brightred;
	else if (_val > 75)
		return fg_yellow;
	else
		return fg_brightgreen;
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
		float percent_avail = static_cast<float>((float)si.available / (float)si.capacity) * 100;
		float percent_used = (100 - percent_avail);
		std::cout << "Drive '" << fs::path(drive) << "'\n"
			<< fg_cyan << "\tAvailable:\t" << fg_brightcyan << to_smallestmagnitude(si.available) << ' ' << to_longsuffix(si.available) << fg_reset << "\n"
			<< fg_cyan << "\tCapacity:\t" << fg_brightcyan << to_smallestmagnitude(si.capacity) << ' ' << to_longsuffix(si.capacity) << fg_reset << "\n";

		std::cout << std::setprecision(4) << fg_cyan << "\tAvailable:\t" << get_drive_avail_color(percent_avail) << percent_avail << "%" << fg_cyan
			<< fg_cyan << "\n\tUsed:\t\t" << get_drive_used_color(percent_used) << percent_used << "%\n" << fg_reset << std::setprecision(6);

		if (si.free != si.available)
			std::cout << fg_cyan << "\tFree:\t\t" << fg_brightcyan << to_smallestmagnitude(si.free) << ' ' << to_longsuffix(si.free) << fg_reset << "\n"
			<< fg_red << "\tUnavailable:\t" << fg_brightred << (si.free - si.available) << " byte(s)\n";
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
	if (!fs::exists(file))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << "Not a file.\n";
		std::exit(1);
	}
	else if (fs::is_directory(file))
	{
		uintmax_t dir_size = 0;
		for (const auto& entry : fs::recursive_directory_iterator(file))
			if (!fs::is_directory(entry.path()))
				dir_size += fs::file_size(entry.path());
		std::cout << fg_cyan << "Directory: " << fg_brightcyan << to_smallestmagnitude(dir_size) << ' ' << to_longsuffix(dir_size) << '\n';
		if (fs.verbose)
			std::cout << "(" << dir_size << " bytes)\n";
	}
	else
	{
		std::cout << fg_cyan << "File: " << fg_brightcyan << to_smallestmagnitude(fs::file_size(file)) << ' ' << to_longsuffix(fs::file_size(file)) << '\n';
		if (fs.verbose)
			std::cout << "(" << fs::file_size(file) << " bytes)\n";
	}
}

void diff_files(const std::string first, const std::string second, FlagStruct& fs)
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
	uintmax_t max_diff = fs.res_limit;
	std::cout << fg_cyan << "Address | " << fs::path(first).filename() << " | " << fs::path(second).filename() << fg_reset << "\n";
	while (max_diff > 0 && (!first_file.eof()) && (!second_file.eof()))
	{
		addr++;
		f = first_file.get();
		s = second_file.get();
		if (f != s)
		{
			std::cout << std::setw(8) << std::hex << fg_cyan << addr << ":\t";
			//std::cout << fg_brightcyan << "0x"<< (int)f << " | " << "0x" << (int)s;

			if ((std::isprint(f) || std::isprint(s)) && (!isspace(f) && !isspace(s)))
			{
				//std::cout << " | " << fg_brightcyan << "'" << (char)f << "' | '" << (char)s << "'\n";
				std::cout << fg_brightcyan << "0x" << (int)f << " ('" << char(f) << "') | " << "0x" << (int)s << " ('" << char(s) << "')\n";
			}
			else
				std::cout << fg_brightcyan << "0x" << (int)f << " | " << "0x" << (int)s << "\n";
			max_diff--;
		}
	}
}

void search_dir(const std::string term, const std::string dir, FlagStruct& fs)
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
				if (results.size() < fs.res_limit || fs.res_limit == 0)
					results.push_back(res.path().filename().string());
				else
					limit_reached = true;
			}
		}
		std::cout << fg_green << "Searching for: " << fg_brightgreen << "'" << term << "' in '" << fs::canonical(dir);
		if(fs.res_limit == 0)
			std::cout << "'\n" << fg_green << "Found (" << found_count << ") | Showing All\n";
		else
			std::cout << "'\n" << fg_green << "Found (" << found_count << ") | Showing (" << fs.res_limit << ")\n";
		for (const auto& ret : results)
		{
			std::cout << '\t' << fg_brightcyan << ret << "\n";
		}
		if (limit_reached)
			std::cout << fg_green << "Result limit reached.\n" << fg_reset;
	}
}

void regex_dir(const std::string pattern, const std::string dir, FlagStruct& fs)
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
					if (results.size() < fs.res_limit || fs.res_limit == 0)
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
		std::cout << fg_green << "Using pattern: " << fg_brightgreen << "'" << pattern << "' in '/" << fs::canonical(dir).filename();
		if (fs.res_limit == 0)
			std::cout << "'\n" << fg_green << "Found (" << found_count << ") | Showing All\n";
		else
			std::cout << "'\n" << fg_green << "Found (" << found_count << ") | Showing (" << fs.res_limit << ")\n";
		for (const auto& ret : results)
		{
			std::cout << '\t' << fg_brightcyan << ret << "\n";
		}
		if (limit_reached)
			std::cout << fg_green << "Result limit reached.\n" << fg_reset;
	}
}

void resize_file(const std::string file, uintmax_t size)
{
	if (!fs::exists(file) || fs::is_directory(file))
	{
		std::cerr << fg_red << "Error: " << fg_brightred << " file doesn't exist or is a directory.\n";
		std::exit(1);
	}
	else
	{
		try
		{
			uintmax_t original = fs::file_size(file);
			fs::resize_file(file, size);
			std::cout << fg_cyan << "Resized '" << file << "' to " << fg_brightcyan << size << fg_cyan << " bytes.";
			if (original > size)
			{
				//std::cout << fg_brightred << " (-" << (original - size) << " bytes)" << fg_reset;
				std::cout << fg_brightred << " (-" << to_smallestmagnitude(original - size) << " " << to_longsuffix(original - size) << ")" << fg_reset;
			}
			else if (original < size)
			{
				//std::cout << fg_brightgreen << " (+" << (size - original) << " bytes)" << fg_reset;
				std::cout << fg_brightgreen << " (+" << to_smallestmagnitude(size - original) << " " << to_longsuffix(size - original) << ")" << fg_reset;
			}
		}
		catch(std::exception)
		{
			std::cerr << fg_red << "Error: " << fg_brightred << " invalid size for " << fg_cyan << "'" << file << "'\n";
		}
	}
}

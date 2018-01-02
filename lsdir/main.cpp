#include <iostream>
#include <experimental/filesystem>

#include "Utils.hpp"

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

namespace fs = std::experimental::filesystem;

bool show_hidden = false;
bool verbose = false;

void list_dir(const std::string dir)
{
	if (verbose)
		for (auto &d : fs::directory_iterator(dir))
		{
			if (d.path().filename().string()[0] != '.' || show_hidden)	// hidden file
			{
				std::cout << fg_brightcyan << d.path().filename() << fg_reset << '\t';
				if (fs::is_directory(d))
					std::cout << "/\n";
				else
					std::cout << "*\n";
			}
		}
	else
		for (auto &d : fs::directory_iterator(dir))
		{
			if (d.path().filename().string()[0] != '.' || show_hidden)	// hidden file
			{
				std::cout << fg_brightcyan << d.path().filename() << fg_reset << '\n';
			}
		}
}

void list_dir_r(const std::string dir, size_t depth = 0)
{
	for (auto& d : fs::directory_iterator(dir))
	{
		if (d.path().filename().string()[0] != '.' || show_hidden)	// hidden file
		{
			std::cout << fg_brightblue << std::string(depth, '-') << fg_reset;
			if (depth > 0)
				std::cout << fg_white << '|';
			if (d.path().string()[0] == '.')
				std::cout << fg_brightcyan << d.path().filename() << '\n';
			else
				std::cout << fg_brightcyan << d.path().filename() << reset << '\n';
			if (fs::is_directory(dir))
			{
				list_dir_r(d.path().string(), depth + 1);
			}
		}
	}
}

void show_drive(const std::string drive)
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

int main(int argc, char* argv[])
{
	// default to list
	if (argc < 2)
	{
		list_dir(".");
	}
	else if(std::string(argv[1]) == "-h"
		|| std::string(argv[1]) == "--help")
	{
		std::cout << "Usage: " << argv[0] << " <directory> or\n\t" <<
			argv[0] << " <flag> <file/directory> [extra-flag]\n"
			"Available flags: \n"
			" -l/--list - lists files in directory\n"
			" -r/--recursive - lists files in directory as well as their children directories\n"
			" -d/--drive - lists drive info\n"
			" -s/--sizeof - returns size of given file";
	}
	// list
	else if (std::string(argv[1]) == "-l"
		|| std::string(argv[1]) == "--list")
	{
		if (argc > 3 
			&& (std::string(argv[3]) == "-v"
			|| std::string(argv[3]) == "--verbose"))
			verbose = true;
		if (argc > 3 && std::string(argv[3]) == "-a")
			show_hidden = true;

		if (argc < 3)
			list_dir(".");
		else
			list_dir(argv[2]);
	}
	// default to list and assume the next arg is a directory
	else if (argc < 3)
	{
		if (fs::exists(argv[1]) && fs::is_directory(argv[1]))
		{
			list_dir(argv[1]);
		}
	}
	// recursive list
	else if (std::string(argv[1]) == "-r"
		|| std::string(argv[1]) == "--recursive")
	{
		if (argc > 3 && std::string(argv[3]) == "-a")
			show_hidden = true;

		if (argc < 3)
			list_dir_r(".");
		else
			list_dir_r(argv[2]);
	}
	// list drives and usage
	else if (std::string(argv[1]) == "-d"
		|| std::string(argv[1]) == "--drive")
	{
		if (argc > 2 && fs::exists(argv[2]))
		{
			show_drive(argv[2]);
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
	// show size of given file
	else if (std::string(argv[1]) == "-s"
		|| std::string(argv[1]) == "--sizeof")
	{
		if (argc < 3 
			|| !fs::exists(argv[2]) 
			|| fs::is_directory(argv[2]))
		{
			std::cerr << "Not a file.\n";
			return 1;
		}
		// verbose
		if (argc > 3
			&& (std::string(argv[3]) == "-v"
			|| std::string(argv[3]) == "--verbose"))
			verbose = true;

		std::cout << to_smallestmagnitude(fs::file_size(argv[2])) << ' ' << to_longsuffix(fs::file_size(argv[2])) << '\n';
		if (verbose)
			std::cout << "("<< fs::file_size(argv[2]) << " bytes)\n";
	}
	else
	{
		std::cerr << "Invalid args!\n";
		return 1;
	}
	return 0;
}
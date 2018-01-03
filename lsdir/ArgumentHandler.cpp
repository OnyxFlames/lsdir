#include "ArgumentHandler.hpp"

#include "Utils.hpp"

#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

ArgumentHandler::ArgumentHandler(int _argc, char * _argv[])
{
	for (int i = 0; i < _argc; i++)
		arguments.push_back(_argv[i]);
}

FlagStruct & ArgumentHandler::eval()
{
	for (size_t i = 0; i < arguments.size(); i++)
	{
		std::string c = arguments[i];

		if (c == "-h" || c == "--help")
			print_help();

		if (c == "-l" || c == "--list")
		{
			flags.list_dir = true;
			flags.paths[0] = arguments[i + 1];
		}
		if (c == "-r" || c == "--recursive")
		{
			flags.list_dir_r = true;
			flags.paths[0] = arguments[i + 1];
		}
		if (c == "-d" || c == "--drive")
		{
			flags.show_drive = true;
			if ((i + 1) >= arguments.size())
				flags.paths[0] = list_drives()[0];
			else
				flags.paths[0] = arguments[i + 1];
				
		}
		if (c == "-v" || c == "--verbose")
		{
			flags.verbose = true;
		}
		if (c == "-s" || c == "--sizeof")
		{
			if ((i + 1) >= arguments.size())
			{
				std::cerr << c << " required a valid file\n";
			}
			else
			{
				flags.show_size = true;
				flags.paths[0] = arguments[i + 1];
			}
		}
	}
	return flags;
}

void ArgumentHandler::exec()
{
	if (flags.list_dir)
		list_dir(flags.paths[0], flags);
	if (flags.list_dir_r)
		list_dir_r(flags.paths[0], flags);
	if (flags.show_drive)
		show_drive(flags.paths[0], flags);
	if (flags.show_size)
		show_size(flags.paths[0], flags);
	if (flags.diff_file)
		std::cout << "--diff not currently supported.\n";
	if (flags.search)
		std::cout << "--search not currently supported.\n";
	if (flags.regex_search)
		std::cout << "--regex not currently supported.\n";
}

void ArgumentHandler::print_help()
{
	std::cout << "Usage: " << arguments[0] << " <directory> or\n\t" <<
		arguments[0] << " <flag> <file/directory> [extra-flag]\n"
		"Available flags: \n"
		" -l/--list - lists files in directory\n"
		" -r/--recursive - lists files in directory as well as their children directories\n"
		" -d/--drive - lists drive info\n"
		" -s/--sizeof - returns size of given file";
	std::exit(1);
}

ArgumentHandler::~ArgumentHandler()
{
}

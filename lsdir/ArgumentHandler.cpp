#include "ArgumentHandler.hpp"

#include "ColorTable.hpp"
#include "Utils.hpp"

#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

ArgumentHandler::ArgumentHandler(int _argc, char * _argv[])
{
	for (int i = 0; i < _argc; i++)
		arguments.push_back(_argv[i]);
}

FlagStruct& ArgumentHandler::eval()
{
	for (size_t i = 0; i < arguments.size(); i++)
	{
		std::string c = arguments[i];

		if (c == "-h" || c == "--help")
			print_help();

		if (c == "-l" || c == "--list")
		{
			flags.list_dir = true;
			if ((i + 1) >= arguments.size())
				flags.paths[0] = ".";
			else
				flags.paths[0] = arguments[++i];
		}
		if (c == "-r" || c == "--recursive")
		{
			flags.list_dir_r = true;
			if ((i + 1) >= arguments.size())
				flags.paths[0] = ".";
			else
				flags.paths[0] = arguments[++i];
		}
		if (c == "-d" || c == "--drive")
		{
			flags.show_drive = true;
			if ((i + 1) >= arguments.size())
				flags.paths[0] = list_drives()[0];
			else
				flags.paths[0] = arguments[++i];
				
		}
		if (c == "-v" || c == "--verbose")
		{
			flags.verbose = true;
		}
		if (c == "-s" || c == "--sizeof")
		{
			if ((i + 1) >= arguments.size())
			{
				std::cerr << c << " expects file\n";
			}
			else
			{
				flags.show_size = true;
				flags.paths[0] = arguments[++i];
			}
		}
		if (c == "--diff")
		{
			if ((i + 2) >= arguments.size())
			{
				std::cerr << c << " expects 2 files\n";
			}
			else
			{
				flags.diff_file = true;
				flags.paths[0] = arguments[++i];
				flags.paths[1] = arguments[++i];
			}
		}
		if (c == "--search")
		{
			if ((i + 1) >= arguments.size())
			{
				std::cerr << c << " expects a search term\n";
			}
			else
			{
				flags.search = true;
				flags.paths[0] = arguments[++i];
				if ((i + 1) >= arguments.size())
				{
					flags.paths[1] = ".";
				}
				else
					flags.paths[1] = arguments[++i];
			}
		}
		if (c == "--regex")
		{
			if ((i + 1) >= arguments.size())
			{
				std::cerr << c << " expects a search pattern\n";
			}
			else
			{
				flags.regex_search = true;
				flags.paths[0] = arguments[++i];
				if ((i + 1) >= arguments.size())
				{
					flags.paths[1] = ".";
				}
				else
					flags.paths[1] = arguments[++i];
			}
		}
		if (c == "--resize")
		{
			if ((i + 2) >= arguments.size())
			{
				std::cerr << fg_red << "Error: " << fg_brightred << c << " expects <file>, then <value> in bytes.\n";
			}
			else
			{
				flags.resize_file = true;
				flags.paths[0] = arguments[++i];
				flags.size = to_byte_value(arguments[++i]);
			}
		}
		if (c == "--resultlimit")
		{
			if ((i + 1) >= arguments.size())
			{
				std::cerr << fg_red << "Error: " << fg_brightred << c << " expected integer for result limit.\n";
			}
			else
			{
				//flags.newresult_limit = true;
				flags.res_limit = std::stoll(arguments[++i]);
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
		diff_files(flags.paths[0], flags.paths[1], flags);
	if (flags.search)
		search_dir(flags.paths[0], flags.paths[1], flags);
	if (flags.regex_search)
		regex_dir(flags.paths[0], flags.paths[1], flags);
	if (flags.resize_file)
		resize_file(flags.paths[0], flags.size);
}

void ArgumentHandler::print_help()
{
	std::cout << arguments[0] << " - Filesystem toolset" << "\nUsage: " << arguments[0] << " <directory> or\n\t" <<
		arguments[0] << " <flag> <file/directory> [extra-flag]\n"
		"Available flags: \n"
		" -l/--list - lists files in directory\n"
		" -r/--recursive - lists files in directory as well as their children directories\n"
		" -d/--drive - lists drive info\n"
		" -s/--sizeof - returns size of given file\n"
		" --diff - compares two files\n"
		" --search - uses <term> to search in <dir>\n"
		" --regex - uses <pattern> to search in <dir\n"
		" --resize - resizes <file> to size in <bytes>\n"
		" --resultlimit - limits results to <value> when applicable\n";
	std::exit(1);
}

ArgumentHandler::~ArgumentHandler()
{
}

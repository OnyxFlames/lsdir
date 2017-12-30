#include <iostream>
#include <experimental/filesystem>

//#define LSDIR_COLOR

#ifdef LSDIR_COLOR
	const char* reset = "\033[0m";
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
	const char* reset = "\033[0m";
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

void list_dir_r(const std::string dir, size_t depth = 0)
{
	for (auto& d : fs::directory_iterator(dir))
	{
		if (d.path().filename().string()[0] != '.' || show_hidden)	// hidden file
		{
			std::cout << fg_cyan << std::string(depth, '-') << reset;
			if (depth > 0)
				std::cout << fg_cyan << '|';
			std::cout << fg_cyan << d.path().filename() << reset << '\n';
			if (fs::is_directory(dir))
			{
				list_dir_r(d.path().string(), depth + 1);
			}
		}
	}
}

void list_dir(const std::string dir)
{
	if (verbose)
		for (auto &d : fs::directory_iterator(dir))
		{
			std::cout << d.path().filename() << '\t';
			if (fs::is_directory(d))
				std::cout << "/\n";
			else
				std::cout << "*\n";
		}
	else
		for (auto &d : fs::directory_iterator(dir))
			std::cout << d.path().filename() << '\n';
}

int main(int argc, char* argv[])
{
	// default to list
	if (argc < 2)
	{
		list_dir("");
	}
	// list
	else if (std::string(argv[1]) == "-l")
	{
		if (argc > 3 
			&& (std::string(argv[3]) == "-v"
			|| std::string(argv[3]) == "--verbose"))
			verbose = true;

		if (argc < 3)
			list_dir("");
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
		else
		{
			std::cerr << "Invalid args!\n";
			return 1;
		}
	}
	// recursive list
	else if (std::string(argv[1]) == "-r")
	{
		if (argc > 3 && std::string(argv[3]) == "-a")
			show_hidden = true;

		if (argc < 3)
			list_dir_r("");
		else
			list_dir_r(argv[2]);
	}
	// list drives and usage
	else if (std::string(argv[1]) == "-d")
	{
		// todo: add util that converts to largest magnitude
		if (argc > 2 && fs::exists(argv[2]))
		{
			fs::space_info si;
			si = fs::space(argv[2]);
			std::cout << "Drive '" << fs::path(argv[2]) << "'\t\n"
				<< "\tAvailable:\t" << si.available << "\n"
				<< "\tCapacity:\t" << si.capacity << "\n"
				<< "\tFree:\t\t" << si.free << "\n";
			if (si.free != si.available)
				std::cout << "\tUnavailable:\t" << (si.free - si.available) << "\n";
		}
		else
			std::cout << "Not a drive.\n";
	}
	// show size of given file
	else if (std::string(argv[1]) == "--sizeof")
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

		uint16_t magnitude = 0;
		uintmax_t size = fs::file_size(fs::path(argv[2]));
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
		if (verbose)
		{
			switch (magnitude)
			{
				case 0: std::cout << size << " bytes\t"; break;
				case 1: std::cout << size << " kilobytes\t"; break;
				case 2: std::cout << size << " megabytes\t"; break;
				case 3: std::cout << size << " gigabytes\t"; break;
				case 4: std::cout << size << " terabytes\t"; break;
				case 5: std::cout << size << " petabytes\t"; break;
				default: std::cout << "Unknown\n"; break;
			}
			if (magnitude > 0)
				std::cout << "(" << counter << " bytes)\n";
			else
				std::cout << '\n';

		}
		else
		{
			switch (magnitude)
			{
			case 0: std::cout << size << " bytes\n"; break;
			case 1: std::cout << size << " kilobytes\n"; break;
			case 2: std::cout << size << " megabytes\n"; break;
			case 3: std::cout << size << " gigabytes\n"; break;
			case 4: std::cout << size << " terabytes\n"; break;
			case 5: std::cout << size << " petabytes\n"; break;
			default: std::cout << "Unknown\n"; break;
			}
		}
	}
	else
	{
		std::cerr << "Invalid args!\n";
		return 1;
	}
	return 0;
}
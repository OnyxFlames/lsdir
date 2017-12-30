#include <iostream>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool show_hidden = false;
bool verbose = false;

void list_dir_r(const std::string dir, size_t depth = 0)
{
	for (auto& d : fs::directory_iterator(dir))
	{
		if (d.path().filename().string()[0] != '.' || show_hidden)	// hidden file
		{
			std::cout << "\033[91m" << std::string(depth, '-') << "\033[0m";
			if (depth > 0)
				std::cout << '|';
			std::cout << "\033[36m" << d.path().filename() << "\033[0m" << '\n';
			if (fs::is_directory(dir))
			{
				list_dir_r(d.path().string(), depth + 1);
			}
		}
	}
}

void list_dir(const std::string dir)
{
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
		if (argc < 3)
			list_dir("");
		else
			list_dir(argv[2]);
	}
	// default to list and assume the next arg is a directory
	else if (argc < 3)
	{
		if (fs::exists(argv[2]) && fs::is_directory(argv[2]))
		{
			list_dir(argv[2]);
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
		std::cout << "Drive listing unavailable.\n";
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
		if (argc > 3 && std::string(argv[2]) == "-v")
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
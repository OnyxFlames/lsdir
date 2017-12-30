#include <iostream>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool show_hidden = true;

void list_dir_new(const std::string dir,
	size_t recur_depth = 0,
	bool recursive = false,
	bool show_size = false)
{
	for (auto &d : fs::directory_iterator(dir))
	{
		if (recursive)
		{
			std::cout << std::string(recur_depth, '-');
			if (recur_depth > 0)
				std::cout << '|';
		}
		std::cout << d.path().filename() << '\n';
		if (fs::is_directory(d.path()))
		{
			if(recursive)
				list_dir_new(d.path().string(), recur_depth + 1, recursive, show_size);
		}
	}
}

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

int main()
{
	list_dir_r("../");
	//getchar();
	return 0;
}
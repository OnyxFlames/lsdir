#include <iostream>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void list_dir_r(const std::string dir, size_t recur_depth = 0)
{
	for (auto &d : fs::directory_iterator(dir))
	{
		std::cout << std::string(recur_depth, '-');
		if (recur_depth > 0)
			std::cout << '|';
		std::cout << d.path().filename() << '\n';
		if (fs::is_directory(d.path()))
		{
			list_dir_r(d.path().string(), recur_depth + 1);
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
	list_dir("../");
	getchar();
	return 0;
}
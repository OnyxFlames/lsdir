#include <iostream>
#include <experimental/filesystem>

#include "Utils.hpp"
#include "ArgumentHandler.hpp"
#include "ColorTable.hpp"

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	ArgumentHandler ah(argc, argv);
	ah.eval();
	ah.exec();
	return 0;
}
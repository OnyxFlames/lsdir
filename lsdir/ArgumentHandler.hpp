#pragma once

#include <array>
#include <string>
#include <vector>

struct FlagStruct
{
	bool print_help =	false;
	bool list_dir =		false;
	bool list_dir_r =	false;
	bool show_drive =	false;
	bool show_size =	false;
	bool diff_file =	false;
	bool search =		false;
	bool regex_search = false;
	bool resize_file =  false;
	//bool newresult_limit = false;

	std::array<std::string, 2> paths;

	uintmax_t size = 0;
	uintmax_t res_limit = 50;

	bool verbose = false;
};
class ArgumentHandler
{
private:
	FlagStruct flags;
	std::vector<std::string> arguments;
public:
	ArgumentHandler(int _argc, char* _argv[]);
	FlagStruct& eval();
	void exec();
	void print_help();
	~ArgumentHandler();
};


#pragma once

#define LSDIR_COLOR

#ifdef LSDIR_COLOR
#define fg_reset "\033[0m"
#define fg_red "\033[31m"
#define fg_green "\033[32m"
#define fg_yellow "\033[33m"
#define fg_blue "\033[34m"
#define fg_magenta "\033[35m"
#define fg_cyan "\033[36m"
#define fg_white "\033[37m"
#define fg_brightred "\033[91m"
#define fg_brightgreen "\033[92m"
#define fg_brightyellow "\033[93m"
#define fg_brightblue "\033[94m"
#define fg_brightmagenta "\033[95m"
#define fg_brightcyan "\033[96m"
#else
#define fg_reset ""
#define fg_red ""
#define fg_green ""
#define fg_yellow ""
#define fg_blue ""
#define fg_magenta ""
#define fg_cyan ""
#define fg_white ""
#define fg_brightred ""
#define fg_brightgreen ""
#define fg_brightyellow ""
#define fg_brightblue ""
#define fg_brightmagenta ""
#define fg_brightcyan ""
#endif
#pragma once

#include <cstdint>
#include <string>
#include <vector>

const std::vector<std::string> list_drives();

const std::string to_shortsuffix(uintmax_t num, unsigned short divisor = 1024);
const std::string to_longsuffix(uintmax_t num, unsigned short divisor = 1024);

const double to_smallestmagnitude(uintmax_t num, unsigned short divisor = 1024);
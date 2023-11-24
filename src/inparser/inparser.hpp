#ifndef __INPUTPARSER_H
#define __INPUTPARSER_H
#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace InputParser {
void parse(int argc, char *argv[]);

const std::vector<std::string_view> &input_arguments();
bool show_ends();
bool show_line_numbers();

};  // namespace InputParser

#endif

#include "AoC.h"

#include <iostream>

int main() {
	std::cout << "Input file name: ";
	std::string input_file_name;
	std::getline(std::cin, input_file_name);

	int result = aoc2023::dayFour(input_file_name);
	std::cout << "Result is: " << result << std::endl;

	return 0;
}
#include "AoC.h"

#include <iostream>

int main() {
	std::cout << "Input file 1 name: ";
	std::string input_file_name;
	std::getline(std::cin, input_file_name);

	int result = aoc2023::dayOne(input_file_name);
	std::cout << "Day one result is: " << result << std::endl;

	return 0;
}
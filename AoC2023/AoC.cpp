#include "AoC.h"

#include <fstream>
#include <string>
#include <cctype>
#include <limits>
#include <vector>
#include <unordered_map>

int aoc2023::dayOne(std::string day_one_file) {
	std::ifstream input_file(day_one_file);
	std::string current_line;
	long sum{};
	std::unordered_map<std::string, int> numbers{
		{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4},
		{"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}
	};

	while (std::getline(input_file, current_line)) {
		// Iterate through the line and get all numbers
		std::vector<int> string_nums;
		for (size_t i = 0; i < current_line.length(); i++) {
			// Check if digit
			if (std::isdigit(current_line.at(i))) {
				string_nums.push_back(current_line.at(i) - '0');
				continue;
			}
			// Check if spelled out digit in 3, 4, or 5 length words
			int possible_lengths[3]{ 3, 4, 5 };
			for (size_t j = 0; j < 3; j++) {
				if (numbers.count(current_line.substr(i, possible_lengths[j])) > 0) {
					string_nums.push_back(numbers[current_line.substr(i, possible_lengths[j])]);
					break;
				}
			}
		}

		sum += (string_nums.at(0) * 10 + string_nums.back());
	}

	return sum;
}
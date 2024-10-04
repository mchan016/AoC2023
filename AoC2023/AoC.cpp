#include "AoC.h"

#include <fstream>
#include <string>
#include <cctype>
#include <limits>
#include <vector>
#include <unordered_map>

#include <iostream>

int aoc2023::dayTwo(std::string day_two_file) {
	std::ifstream input_file(day_two_file);
	std::string current_line;
	int games_power_sum{};

	while (std::getline(input_file, current_line)) {
		// Get the current game's rounds
		auto separator = current_line.find_first_of(':');
		std::string game_rounds = current_line.substr(separator+1);
		std::unordered_map<char, int> largest_data{
			{'r', std::numeric_limits<int>::min()},
			{'g', std::numeric_limits<int>::min()},
			{'b', std::numeric_limits<int>::min()},
		};

		// Iterate through all rounds trying to find a datapoint that exceeds the limits
		for (size_t i = 0; i < game_rounds.length(); i++) {
			if (std::isdigit(game_rounds[i])) {
				int current_num = 0;
				while (std::isdigit(game_rounds[i])) {
					current_num *= 10;
					current_num += (game_rounds[i++] - '0');
				}
				char color = game_rounds[++i];

				// Check if color is smaller than current smallest data
				if (largest_data[color] == std::numeric_limits<int>::min() || largest_data[color] < current_num) {
					largest_data[color] = current_num;
				}
			}
		}

		// No datapoints exceeded limits, add current round to game sum
		int power{ 1 };
		for (const auto& data_point : largest_data) {
			int number = data_point.second;
			if (number != std::numeric_limits<int>::min()) {
				power *= number;
			}
		}

		games_power_sum += power;
	}


	return games_power_sum;
}

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
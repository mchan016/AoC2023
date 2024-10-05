#include "AoC.h"

#include <cmath>
#include <fstream>
#include <string>
#include <cctype>
#include <limits>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <iostream>


///////////////////////////////////////////////////
//  GENERAL UTILITY FUNCTIONS
///////////////////////////////////////////////////

template <typename T>
std::vector<T> numberLineToVector(std::string nums_line) {
	nums_line += ' ';
	std::vector<T> nums;
	T current_num{std::numeric_limits<T>::min()};
	for (auto character : nums_line) {
		if (std::isdigit(character)) {
			if (current_num == std::numeric_limits<T>::min()) {
				current_num = 0;
			}
			current_num *= 10;
			current_num += character - '0';
		}
		else {
			if (current_num != std::numeric_limits<T>::min()) {
				nums.push_back(current_num);
				current_num = std::numeric_limits<T>::min();
			}
		}
	}

	return nums;
}

///////////////////////////////////////////////////
//  DAY FIVE
///////////////////////////////////////////////////

int aoc2023::dayFive(std::string day_five_file) {
	std::ifstream input_file(day_five_file);
	std::string current_line;
	struct almanac_point {
	private:
		long long _dest;
		long long _src;
		long long _range;

	public:
		almanac_point(long long dest, long long src, long long range) 
			: _dest{dest}, _src{src}, _range{range}
		{}

		bool is_in_range(long long candidate) {
			if (candidate >= _src && candidate < (_src + _range)) {
				return true;
			}

			return false;
		}

		long long get_destination(long long candidate) {
			long long diff = candidate - _src;

			return _dest + diff;
		}
	};

	// Get all inputs
	std::vector<long long> seeds;
	std::string current_conversion{""};
	std::unordered_map<std::string, std::vector<almanac_point*>> data;
	while (std::getline(input_file, current_line)) {
		if (current_line.find("seeds") != std::string::npos) {
			auto seeds_line = current_line.substr(current_line.find(':') + 1);
			seeds = numberLineToVector<long long>(seeds_line);
		}
		else if (current_line.empty()) {
			if (!current_conversion.empty()) {
				current_conversion = "";
			}
		}
		else if (current_line.find("map") != std::string::npos) {
			current_conversion = current_line.substr(0, current_line.find("map") - 1);
		}
		else {
			// Add data points to current almanac conversion
			std::vector<long long> nums = numberLineToVector<long long>(current_line);
			data[current_conversion].push_back(new almanac_point{ nums.at(0), nums.at(1), nums.at(2) });
		}
	}

	std::vector<std::string> pipeline{
		"seed-to-soil",
		"soil-to-fertilizer",
		"fertilizer-to-water",
		"water-to-light",
		"light-to-temperature",
		"temperature-to-humidity",
		"humidity-to-location"
	};
	long long current_closest_location{ std::numeric_limits<long long>::max() };
	for (const auto& seed : seeds) {
		auto source = seed;
		long long destination = std::numeric_limits<long long>::min();
		for (const auto& pipeline_point : pipeline) {
			std::vector<almanac_point*>& translation_data = data.at(pipeline_point);

			for (auto* point : translation_data) {
				if (point->is_in_range(source)) {
					destination = point->get_destination(source);
					break;
				}
			}

			source = (destination == std::numeric_limits<long long>::min()) ? source : destination;
			destination = std::numeric_limits<long long>::min();
		}

		// Check if location is closer than current closest location
		if (source < current_closest_location) {
			current_closest_location = source;
		}
	}
	

	return current_closest_location;
}

///////////////////////////////////////////////////
//  DAY FOUR
///////////////////////////////////////////////////

int aoc2023::dayFour(std::string day_four_file) {
	std::ifstream input_file(day_four_file);
	std::string current_line;

	std::vector<int> scratcher_matches;
	std::vector<int> scratcher_count;
	while (std::getline(input_file, current_line)) {
		// Grab the section with winning numbers
		auto winning_start = current_line.find_first_of(':') + 1;
		auto winning_end = current_line.find_first_of('|');
		std::string winning_nums_line = current_line.substr(winning_start, winning_end - winning_start);
		std::vector<int> winning_nums_vector{ numberLineToVector<int>(winning_nums_line) };
		std::unordered_set<int> winning_nums{ winning_nums_vector.begin(), winning_nums_vector.end() };
		
		// Grabe the section with scratcher numbers
		std::string scratcher_nums_line = current_line.substr(winning_end + 1);
		std::vector<int> scratcher_nums{ numberLineToVector<int>(scratcher_nums_line) };
		int matches{ 0 };
		for (const auto& num : scratcher_nums) {
			if (winning_nums.count(num) > 0) {
				++matches;
			}
		}

		scratcher_matches.push_back(matches);
		scratcher_count.push_back(1);
	}

	// Process each match according to new rules
	int instances_sum{};
	for (size_t i = 0; i < scratcher_count.size(); i++) {
		int current_matches = scratcher_matches.at(i);
		int current_copies = scratcher_count.at(i);
		instances_sum += current_copies;

		for (size_t j = 1; j <= current_matches; j++) {
			scratcher_count[i + j] += current_copies;
		}
	}

	return instances_sum;
}

///////////////////////////////////////////////////
//  DAY THREE
///////////////////////////////////////////////////

int aoc2023::dayThree(std::string day_three_file) {
	std::ifstream input_file(day_three_file);
	std::string current_line;
	std::vector<int> numbers;
	int numbers_position{};
	int current_number{};
	std::vector<std::vector<std::string>> schematic;

	while (std::getline(input_file, current_line)) {
		// Stuff numbers into the numbers vector and keep references into the schematic
		std::vector<std::string> schema_line{};
		for (size_t i = 0; i < current_line.length(); i++) {
			char character = current_line[i];
			if (!std::isdigit(character)) {
				if (current_number != 0) {
					numbers.push_back(current_number);
					current_number = 0;
					++numbers_position;
				}

				schema_line.push_back(std::string{ character });
			}
			else {
				current_number *= 10;
				current_number += character - '0';
				schema_line.push_back(std::to_string(numbers_position));
			}
		}

		schematic.push_back(schema_line);
	}

	// Explore the map and get all numbers that are adjacent to a symbol
	// make sure don't double count
	int gear_ratio_sum{};
	for (int i = 0; i < static_cast<int>(schematic.size()); i++) {
		const auto& line = schematic.at(i);
		for (int j = 0; j < static_cast<int>(line.size()); j++) {
			char character = line.at(j).at(0);
			if (!std::isdigit(character) && character == '*') {
				std::vector<int> nums_adjacent;
				std::unordered_set<int> seen_positions;
				// Check all neighboring numbers
				for (int row_displace = -1; row_displace <= 1; row_displace++) {
					for (int col_displace = -1; col_displace <= 1; col_displace++) {
						int row = i + row_displace;
						int col = j + col_displace;

						// Check if out of bounds or is not a digit
						if (row < 0 || row >= schematic.size() || col < 0 || col >= schematic.at(0).size() || !std::isdigit(schematic[row][col].at(0))) {
							continue;
						}

						// Otherwise add to sum and account for number
						int position = std::stoi(schematic[row][col]);
						if (seen_positions.count(position) > 0) {
							continue;
						}
						seen_positions.insert(position);
						nums_adjacent.push_back(numbers[position]);
					}
				}

				// Only add to gear ratio if it's a gear
				if (nums_adjacent.size() == 2) {
					gear_ratio_sum += nums_adjacent.at(0) * nums_adjacent.at(1);
				}
			}
		}
	}

	return gear_ratio_sum;
}

///////////////////////////////////////////////////
//  DAY TWO
///////////////////////////////////////////////////

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

///////////////////////////////////////////////////
//  DAY ONE
///////////////////////////////////////////////////

int aoc2023::dayOne(std::string day_one_file) {
	std::ifstream input_file(day_one_file);
	std::string current_line;
	long long sum{};
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
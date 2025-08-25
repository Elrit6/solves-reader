#pragma once

#include <algorithm>
#include <cstdint>
#include <print>
#include <sstream>
#include <vector>

#include "solveData.hpp"

class SolvesHandler {
public:
	static auto getAverages(
		const std::vector<Solve> &solves,
		const std::uint32_t averageSolveCount
	) -> std::vector<Average>;
	template <typename T>
	static auto sort(std::vector<T> &objects) -> void;
	template <typename T>
	static auto display(const std::vector<T> &objects) -> std::string;
};

template <typename T>
auto SolvesHandler::sort(std::vector<T> &objects) -> void {
	std::sort(objects.begin(), objects.end());
}

template <typename T>
auto SolvesHandler::display(const std::vector<T> &objects) -> std::string {
	std::ostringstream buffer;
	std::uint32_t index = 1;
	for (const T &object : objects) {
		buffer << object.display(index) << "\n";
		index++;
	}
	return buffer.str();
}

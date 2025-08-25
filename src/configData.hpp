#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <set>
#include <string>

struct ConfigData {
	struct DateFilter {
		std::optional<std::chrono::system_clock::time_point> after;
		std::optional<std::chrono::system_clock::time_point> before;
	};
	std::set<std::string> sessions;
	std::uint32_t solves;
	std::string outputFilePath;
	DateFilter dateFilter;
};

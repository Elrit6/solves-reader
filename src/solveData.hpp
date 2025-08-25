#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

class Solve {
public:
	std::string scramble;
	std::chrono::milliseconds time;
	std::chrono::system_clock::time_point date;
	bool plus2;
	bool dnf;
	auto display(const std::uint32_t index) const -> std::string;
	auto operator<(const Solve &otherSolve) const -> bool;
};

class Average {
public:
	std::vector<Solve> solves;
	std::uint32_t solveCount;
	std::chrono::milliseconds time;
	std::chrono::system_clock::time_point date;
	bool dnf;
	bool mean;
	auto setSolves(const std::vector<Solve> &newSolves) -> void;
	auto display(const std::uint32_t index) const -> std::string; 
	auto operator<(const Average &otherAverage) const -> bool;
};


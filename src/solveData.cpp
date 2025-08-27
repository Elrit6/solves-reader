#include "solveData.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <sstream>

using namespace std::chrono_literals;

namespace {
	namespace Format {
		auto timeToText(
			std::chrono::milliseconds ms,
			const bool plus2,
			const bool dnf
		) -> std::string
		{
			if (dnf)
				return "DNF";

			const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(ms);
			ms -= minutes;
			const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ms);
			ms -= seconds;
				
			std::stringstream buffer;
			if (minutes.count() > 0) {
				buffer
					<< minutes.count() << ":"
					<< std::setw(2) << std::setfill('0') << seconds.count();
			} else {
				buffer << seconds.count();
			}
			buffer << "." << std::setw(2) << std::setfill('0') << ms.count() / 10; // truncate to 2 digits

			if (plus2)
				buffer << "+";

			return buffer.str();
		}

		auto dateToText(const std::chrono::system_clock::time_point date) -> std::string {
			return std::format("[{:%Y-%m-%d %H:%M}]", date);
		}
	}
}

auto Solve::display(const std::uint32_t index) const -> std::string {
	return std::format(
		"{}. {} @ {} ({})",
		index,
		Format::timeToText(time, plus2, dnf),
		Format::dateToText(date),
		scramble
	);
}

auto Solve::operator<(const Solve &otherSolve) const -> bool {
	if (dnf != otherSolve.dnf)
		return !dnf;
	return time < otherSolve.time;
}

auto Average::setSolves(const std::vector<Solve> &newSolves) -> void {
	solves = newSolves;
	solveCount = solves.size();
	mean = solveCount < 4u;

	auto sortedSolves = solves;
	std::sort(sortedSolves.begin(), sortedSolves.end());
	
	constexpr double trimValue = 0.05;
	const std::uint32_t
		trimCount = mean ? 0 : std::ceil(solveCount * trimValue),
		allowedDnfCount = mean ? 0 : trimCount + 1;

	auto timeSum = 0ms;
	bool foundDnfCount = false;
	std::uint32_t countingSolves = 0u, dnfCount = 0u;
	for (std::uint32_t index = 0u; index < solveCount; index++) {
		const Solve &solve = sortedSolves.at(index);
		
		if (!mean && (index < trimCount) || (index >= solveCount - trimCount))
			continue;
		
		if (solve.dnf) {
			if (!foundDnfCount) {
				dnfCount = solveCount - index;
				foundDnfCount = true;
			}
		} else {
			timeSum += solve.time;
			countingSolves++;
		}
	}
	
	time = std::chrono::milliseconds(
		timeSum.count() / static_cast<int32_t>(countingSolves)
	);
	dnf = dnfCount >= allowedDnfCount;

	const Solve &latestSolve = *std::max_element(solves.begin(), solves.end(), 
		[](const Solve &solve1, const Solve &solve2) {
			return solve1.date < solve2.date;
		});
	date = latestSolve.date;
}

auto Average::display(const std::uint32_t index) const -> std::string {
	std::ostringstream buffer;
	buffer << std::format(
		"{}. {} {}{} @ {}",
		index,
		Format::timeToText(time, false, dnf), // since avg doesn't have plus2 false is passed
		mean ? "mo" : "ao",
		solves.size(),
		Format::dateToText(date)
	);

	std::uint32_t solveIndex = 1u;
	for (const Solve &solve : solves) {
		buffer << "\n" << "    " << solve.display(solveIndex);
		solveIndex++;
	}
	
	return buffer.str();
}

auto Average::operator<(const Average &otherAverage) const -> bool {
	if (dnf != otherAverage.dnf)
		return !dnf;
	return time < otherAverage.time;	
}

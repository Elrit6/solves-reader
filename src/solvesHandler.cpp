#include "solvesHandler.hpp"

#include <format>
#include <print>
#include <sstream>
#include <stdexcept>

auto SolvesHandler::getAverages(
	const std::vector<Solve> &solves,
	std::uint32_t averageSolveCount 
) -> std::vector<Average>
{
	if (averageSolveCount > solves.size())
		throw std::runtime_error(
			std::format(
				"Not enough solves ({}) to calculate the average ({}).",
				solves.size(),
				averageSolveCount
			)
		);

	const std::uint32_t maxAverages = solves.size() >= averageSolveCount
		? solves.size() - averageSolveCount + 1
		: 0u;

	std::vector<Average> averages;
	averages.reserve(maxAverages);
	for (std::uint32_t averageIndex = 0u; averageIndex < maxAverages; averageIndex++) {
		std::vector<Solve> averageSolves;
		averageSolves.reserve(averageSolveCount);
		for (std::uint32_t i = averageIndex; i < averageIndex + averageSolveCount; i++)
			averageSolves.push_back(solves.at(i));

		Average average;
		average.setSolves(averageSolves);
		averages.push_back(average);
	}

	return averages;
}


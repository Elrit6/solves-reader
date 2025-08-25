#include <fstream>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "configData.hpp"
#include "configReader.hpp"
#include "solveData.hpp"
#include "solvesHandler.hpp"
#include "solvesReader.hpp"

auto saveToFile(const std::string &output, const std::string &filePath) -> void {
	std::ofstream file(filePath);
	if (!file.is_open())
		throw std::runtime_error(
			std::format("Failed to open the output file ({}).", filePath)
		);
	file << output;
	file.close();

	std::println("Results saved to '{}'.", filePath);
}

auto main(int argc, char **argv) -> int {
	bool isError = false;
	try {
		if (argc < 2)
			throw std::runtime_error("Not enough arguments, the csTimer file path is required.");
		const auto configData = ConfigReader::read();

		const std::string_view csTimerFilePath = argv[1];
		std::vector<Solve> solves = SolvesReader::read(
			csTimerFilePath,
			configData.sessions,
			configData.dateFilter
		);

		std::string output = "";
		const bool isAverage = configData.solves > 1;
		if (isAverage) {
			std::vector<Average> averages = SolvesHandler::getAverages(
				solves, configData.solves
			);
			SolvesHandler::sort(averages);
			output = SolvesHandler::display(averages);
		} else {
			SolvesHandler::sort(solves);
			output = SolvesHandler::display(solves);
		}

		saveToFile(output, configData.outputFilePath);

	} catch (const std::exception &error) {
		std::println("ERROR: {}", error.what());
		isError = true;
	}

	std::cin.get();
	return isError;
}

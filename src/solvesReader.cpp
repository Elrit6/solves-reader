#include "solvesReader.hpp"

#include <chrono>
#include <format>
#include <fstream>
#include <print>
#include <stdexcept>

#include <json.hpp>

using namespace std::chrono_literals;

auto SolvesReader::read(
	const std::string_view filePath,
	const std::set<std::string> &selectedSessions,
	const ConfigData::DateFilter &dateFilter
) -> std::vector<Solve>
{
	std::ifstream file{std::string(filePath)};
	if (!file.is_open())
		throw std::runtime_error(std::format("Failed to open the csTimer file ({}).", filePath));

	std::vector<Solve> solves;
	try {
		const nlohmann::json jsonData = parseCsTimerFile(file);
		solves = getSolvesFromJson(jsonData, selectedSessions, dateFilter);
	} catch (const nlohmann::json::parse_error &error) {
		throw std::runtime_error(std::format("Failed to parse the csTimer file: {}", error.what()));
	} catch (const nlohmann::json::type_error &error) {
		throw std::runtime_error(std::format("Invalid type in the csTimer file: {}", error.what()));
	}

	file.close();
	return solves;
}

auto SolvesReader::parseCsTimerFile(std::ifstream &file) -> nlohmann::json {
	nlohmann::json jsonData = nlohmann::json::parse(file);
	const std::string rawSessionData = jsonData.at("properties").at("sessionData").get<std::string>();
	jsonData.at("properties").at("sessionData") = nlohmann::json::parse(rawSessionData);
	return jsonData; // sessionData is a string not an object, parsed here ^
}

auto SolvesReader::getSolvesFromSession(
	const nlohmann::json &sessionSolves,
	std::vector<Solve> &solves,
	const ConfigData::DateFilter &dateFilter
) -> void
{
	for (const auto &solveData : sessionSolves) {
		const auto date = std::chrono::system_clock::time_point(
			std::chrono::seconds(solveData.at(3).get<std::uint64_t>())
		);

		if (
			(dateFilter.after && date < dateFilter.after) ||
			(dateFilter.before && date > dateFilter.before)
		)
			continue;

		const auto scramble = solveData.at(1).get<std::string>();
		const auto time = std::chrono::milliseconds(
			solveData.at(0).at(1).get<std::int32_t>()
		);
		const auto penalty = std::chrono::milliseconds(
			solveData.at(0).at(0).get<std::int32_t>()
		);
		const bool plus2 = (penalty >= 2000ms);
		const bool dnf = (penalty == -1ms);

		solves.push_back(Solve {
			.scramble = scramble,
			.time = plus2 
				? time + penalty
				: time,
			.date = date,
			.plus2 = plus2,
			.dnf = dnf
		});
	}
}

auto SolvesReader::getSolvesFromJson(
	const nlohmann::json &jsonData,
	const std::set<std::string> &selectedSessions,
	const ConfigData::DateFilter dateFilter
) -> std::vector<Solve> 
{
	std::vector<Solve> solves = {};
	const nlohmann::json &sessionData = jsonData.at("properties").at("sessionData");
	const std::uint32_t sessionCount = jsonData.size() - 1u; // last one is properties

	for (std::uint32_t sessionIndex = 1u; sessionIndex < sessionCount; sessionIndex++) {
		const nlohmann::json &sessionNameObject = sessionData.at(std::to_string(sessionIndex)).at("name");
		const std::string sessionName = sessionNameObject.is_number_unsigned()
			? std::to_string(sessionNameObject.get<std::int32_t>())
			: sessionNameObject.get<std::string>();

		if (!selectedSessions.contains(sessionName))
			continue;

		const nlohmann::json sessionSolves = jsonData.at(std::format("session{}", sessionIndex));
		getSolvesFromSession(sessionSolves, solves, dateFilter);
	}

	return solves;	
};

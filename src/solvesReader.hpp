#pragma once

#include <chrono>
#include <set>
#include <string>
#include <string_view>

#include <json.hpp>

#include "configData.hpp"
#include "solveData.hpp"

class SolvesReader {
public:
	static auto read(
		const std::string_view filePath,
		const std::set<std::string> &selectedSessions,
		const ConfigData::DateFilter &dateFilter
	) -> std::vector<Solve>;
private:
	static auto parseCsTimerFile(std::ifstream &file) -> nlohmann::json;
	static auto getSolvesFromSession(
		const nlohmann::json &sessionSolves,
		std::vector<Solve> &solves,
		const ConfigData::DateFilter &dateFilter
	) -> void;
	static auto getSolvesFromJson(
		const nlohmann::json &jsonData,
		const std::set<std::string> &selectedSessions,
		const ConfigData::DateFilter dateFilter
	) -> std::vector<Solve>;
};

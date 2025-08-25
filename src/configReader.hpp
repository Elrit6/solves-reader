#pragma once

#include <chrono>
#include <map>
#include <optional>
#include <set>
#include <string>

#include <json.hpp>

#include "configData.hpp"

class ConfigReader {
public:
	static auto read() -> ConfigData;
private:
	inline static const auto requiredProperties = std::map<std::string, nlohmann::json::value_t> {
		{"sessions", nlohmann::json::value_t::array},
		{"solves", nlohmann::json::value_t::number_unsigned},
		{"outputFile", nlohmann::json::value_t::string},
		{"after", nlohmann::json::value_t::string},
		{"before", nlohmann::json::value_t::string}
	};
	inline static const auto requiredArrayTypes = std::map<std::string, nlohmann::json::value_t> {
		{"sessions", nlohmann::json::value_t::string},
	};
	static auto checkArrayType(
		const nlohmann::json &jsonArray,
		const nlohmann::json::value_t
	) -> std::int32_t;
	static auto verifyConfig(const nlohmann::json &jsonData) -> void;
	static auto parseDate(
		const std::string &rawDate
	) -> std::optional<std::chrono::system_clock::time_point>;
};

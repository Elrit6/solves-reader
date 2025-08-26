#include "configReader.hpp"

#include <format>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <print>
auto ConfigReader::read() -> ConfigData {
	constexpr char path[] = "config.json";
	std::ifstream file(path);
	if (!file)
		throw std::runtime_error(std::format("Failed to open the configuration file ({}).", path));

	ConfigData data;
	try {
		nlohmann::json jsonData = nlohmann::json::parse(file);
		verifyConfig(jsonData);

		data.sessions = jsonData.at("sessions").get<std::set<std::string>>();
		data.solves = jsonData.at("solves").get<std::uint32_t>();
		data.outputFilePath = jsonData.at("outputFile").get<std::string>();
		data.dateFilter.after = parseDate(jsonData.at("after").get<std::string>());
		data.dateFilter.before = parseDate(jsonData.at("before").get<std::string>());

	} catch (const nlohmann::json::parse_error &error) {
		throw std::runtime_error(std::format("Failed to parse the configuration file: {}", error.what()));
	} catch (const nlohmann::json::type_error &error) {
		throw std::runtime_error(std::format("Invalid type in the configuration file: {}", error.what()));
	}
	
	file.close();
	return data;
}

auto ConfigReader::checkArrayType(
	const nlohmann::json &array,
	const nlohmann::json::value_t expectedType
) -> std::int32_t
{
	std::int32_t itemIndex = -1;
	for (const nlohmann::json &item : array) {
		itemIndex++;
		if (item.type() != expectedType)
			return itemIndex;
	}
	return -1;
}

auto ConfigReader::verifyConfig(const nlohmann::json &jsonData) -> void {
	for (const auto &[key, requiredType] : requiredProperties) {
		if (!jsonData.contains(key))
			throw std::runtime_error(std::format("'{}' is missing from the configuration file.", key));
		if (jsonData.at(key).type() != requiredType)
			throw std::runtime_error(std::format("'{}' is an incorrect type in the configuration file.", key));
	}
	for (const auto &[key, requiredType] : requiredArrayTypes) {
		const std::int32_t wrongTypeIndex = checkArrayType(jsonData.at(key), requiredType);
		if (wrongTypeIndex > -1)
			throw std::runtime_error(std::format("'{}' contains an incorrect type at index {}.", key, wrongTypeIndex));
	}
}

auto ConfigReader::parseDate(
	const std::string &rawDate
) -> std::optional<std::chrono::system_clock::time_point>
{
	if (rawDate.empty())
		return std::nullopt;

	std::istringstream buffer{rawDate};
	std::chrono::year_month_day yearMonthDay;
	buffer >> std::chrono::parse("%F", yearMonthDay);
	if (buffer.fail())
		throw std::runtime_error(std::format("Invalid date: '{}'.", rawDate));

	return std::chrono::system_clock::time_point{
		std::chrono::sys_days{yearMonthDay}
	};
}

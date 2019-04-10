#pragma once
#include <map>
#include <set>
#include <filesystem>
#include <tuple>
#include <iostream>

using std::experimental::filesystem::path;


struct SearchResult
{
	const std::map<std::string, std::string> strToReplace{ { "\n", "\\n" },{ "\r", "\\r" },{ "\t", "\\t" } };
	using position = size_t;
	using prefix = std::string;
	using suffix = std::string;
	using ResultInfo = std::tuple<position, prefix, suffix>;

	void addResult(path p_path, ResultInfo p_result);
	void printAll();

private:
	std::map<path, std::set<ResultInfo>> resultContainer;

	void replaceSpecialCharacters(ResultInfo& p_result);
	std::string replaceAll(std::string p_str, const std::string& p_from, const std::string& p_to);
};
#include "SearchResult.h"

void SearchResult::addResult(path p_path, ResultInfo p_result)
{
	auto& resIter = resultContainer.find(p_path);
	replaceSpecialCharacters(p_result);
	if (resIter != resultContainer.end())
	{
		resIter->second.insert(std::move(p_result));
	}
	else
	{
		resultContainer.emplace(std::move(p_path), std::set<ResultInfo>{ p_result });
	}
}
void SearchResult::printAll()
{
	for (auto res : resultContainer)
	{
		for (auto resInfo : res.second)
			std::cout << res.first.u8string()
			<< "(" << std::get<0>(resInfo)
			<< "): " << std::get<1>(resInfo)
			<< "..." << std::get<2>(resInfo)
			<< std::endl;
	}
}

void SearchResult::replaceSpecialCharacters(ResultInfo& p_result)
{
	prefix& l_pref = std::get<1>(p_result);
	suffix& l_suff = std::get<2>(p_result);

	for (const auto& str : strToReplace)
	{
		l_pref = replaceAll(l_pref, str.first, str.second);
		l_suff = replaceAll(l_suff, str.first, str.second);
	}
}

std::string SearchResult::replaceAll(std::string p_str, const std::string& p_from, const std::string& p_to)
{
	size_t start_pos = 0;
	while ((start_pos = p_str.find(p_from, start_pos)) != std::string::npos) {
		p_str.replace(start_pos, p_from.length(), p_to);
		start_pos += p_to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return p_str;
}
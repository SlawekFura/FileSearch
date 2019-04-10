#pragma once
#include "SearchResult.h"
#include <fstream>

#define DATA_BUFFER_SIZE (1<<5)
#define MAX_PREFIX_SIZE 3
#define MAX_SUFFIX_SIZE 3


using std::experimental::filesystem::path;

struct Parser
{
	Parser(std::ifstream& p_in, std::streamoff p_fileSize, std::string p_wordToSearch, path p_path, SearchResult& p_searchResult)
		: m_fileStream(p_in), m_fileSize(p_fileSize), m_wordToSearch(std::move(p_wordToSearch)), m_path(p_path), m_searchResult(p_searchResult)
	{
		m_preambule = MAX_PREFIX_SIZE + m_wordToSearch.size() + MAX_SUFFIX_SIZE - 1; 
		m_newDataToReadSize = DATA_BUFFER_SIZE - m_preambule - 1;
	}

	void parse();
		
private:
	void processBuffer(long long i);

	int8_t calculatePrefixOffset(long long, SearchResult::position);
	int8_t calculateSuffixOffset(long long, SearchResult::position, const char *, bool);

	SearchResult& m_searchResult;
	std::ifstream& m_fileStream;
	std::string m_wordToSearch;
	path m_path;
	std::streamoff m_fileSize;
	const char* m_buffer;

	size_t m_preambule;
	char m_data[DATA_BUFFER_SIZE];
	size_t m_newDataToReadSize;
};
#pragma once
#include <string>
#include <iostream>
#include "SearchResult.h"
#include <fstream>
#include <filesystem>

#define DATA_BUFFER (1<<4)
#define MAX_PREFIX_SIZE 3
#define MAX_SUFFIX_SIZE 3


using std::experimental::filesystem::path;

struct Parser
{
	Parser(std::ifstream& p_in, std::streamoff p_fileSize, std::string p_wordToSearch, path p_path, SearchResult& p_searchResult)
		: m_fileStream(p_in), m_fileSize(p_fileSize), m_wordToSearch(p_wordToSearch), m_path(p_path), m_searchResult(p_searchResult)
	{
		m_preambule = MAX_PREFIX_SIZE + m_wordToSearch.size() + MAX_SUFFIX_SIZE;
		m_finalBufferSize = DATA_BUFFER;
		m_dataBuffer = DATA_BUFFER - m_preambule;
		m_newDataToReadSize = m_finalBufferSize - m_preambule;
	}

	void parse();
		
private:
	void processBuffer(long long i);

	int8_t calculatePrefixOffset(long long, SearchResult::position);
	int8_t calculateSuffixOffset(long long, SearchResult::position, const char *);

	SearchResult& m_searchResult;
	std::ifstream& m_fileStream;
	std::string m_wordToSearch;
	path m_path;
	std::streamoff m_fileSize;
	const char* m_buffer;

	size_t m_preambule;
	size_t m_finalBufferSize;
	char m_data[DATA_BUFFER];

	size_t m_dataBuffer;
	size_t m_newDataToReadSize;
};
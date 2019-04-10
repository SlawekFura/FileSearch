#pragma once
#include "SearchResult.h"
#include <fstream>
#include <mutex>

#define DATA_BUFFER_SIZE (1<<19)
#define MAX_PREFIX_SIZE 3
#define MAX_SUFFIX_SIZE 3
#define DUMMY_CHAR 27


using std::experimental::filesystem::path;

struct Parser
{
	Parser(std::ifstream& p_in, std::string p_wordToSearch, path p_path, SearchResult& p_searchResult, std::mutex& p_resultFillingMutex)
		: m_fileStream(p_in), m_wordToSearch(std::move(p_wordToSearch)), m_path(p_path), m_searchResult(p_searchResult), m_resultFillingMutex(p_resultFillingMutex)
	{
		m_fileStream.seekg(0, std::ios::end);
		m_fileSize = m_fileStream.tellg();
		m_fileStream.seekg(0, std::ios::beg);

		m_preambule = MAX_PREFIX_SIZE + m_wordToSearch.size() + MAX_SUFFIX_SIZE - 1; 
		m_newDataToReadSize = DATA_BUFFER_SIZE - m_preambule - 1;
	}

	void parse();
		
private:
	void fillDataPreambuleWithDummyChars();

	void processBuffer(size_t);

	int8_t calculatePrefixOffset(size_t, SearchResult::position);
	int8_t calculateSuffixOffset(size_t, SearchResult::position, const char *, bool);

	SearchResult& m_searchResult;
	std::ifstream& m_fileStream;
	std::string m_wordToSearch;
	path m_path;
	std::streamoff m_fileSize;
	const char* m_buffer;
	std::mutex& m_resultFillingMutex;

	size_t m_preambule;
	char m_data[DATA_BUFFER_SIZE];
	size_t m_newDataToReadSize;
};
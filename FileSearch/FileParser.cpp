#pragma once
#include <string>
#include <iostream>
#include "FileParser.h"
#include <fstream>
#include <filesystem>

static int a = 0;

//some function for testing purposes
void printer(const char* p_word)
{
	while (*p_word++ != '\0')
	{
		char charToPrint = *p_word;
		switch (charToPrint)
		{
		case '\n':
			std::cout << "\\n";
			break;
		case '\t':
			std::cout << "\\t";
			break;
		default:
			std::cout << *p_word;
		}
	}
}

using std::experimental::filesystem::path;

void Parser::parse()
{
	for (long long buffer = 0; buffer < m_fileSize; buffer += m_newDataToReadSize)
	{
		m_fileStream.read(&m_data[m_preambule - 1], m_newDataToReadSize);

		m_data[m_fileStream.gcount() + m_preambule - 1] = '\0';

		processBuffer(buffer);
	}
	//testing purposes
	//std::cout << "num: " << a << std::endl;
}

//Main assumption:
//buffer consists {[data from previous iteration], [new read data], [unprocessed data to be copied on the begin]} - for getting preffix and suffix
void Parser::processBuffer(long long p_bufferPackage)
{
	const char * pos = m_data + MAX_PREFIX_SIZE;
	while ((pos = strstr(pos, m_wordToSearch.c_str())) != NULL)
	{
		SearchResult::position finalPosition = p_bufferPackage + (pos - &m_data[0]) - (m_preambule - 1);

		bool isWordOccurrenceExceededSearchingBoundries = pos > m_data + m_finalBufferSize - m_wordToSearch.size() - MAX_PREFIX_SIZE - 1;
		if (isWordOccurrenceExceededSearchingBoundries)
		{
			++pos;
			continue;
		}

		int8_t preffixOffset = calculatePrefixOffset(p_bufferPackage, finalPosition);
		if (preffixOffset == -1)
		{
			std::cout << "Error with prefix Offset calculation!" << std::endl;
			continue;
		}

		int8_t suffixOffset = calculateSuffixOffset(p_bufferPackage, finalPosition, pos);
		if (suffixOffset == -1)
		{
			std::cout << "Error with suffix Offset calculation!" << std::endl;
			continue;
		}

		m_searchResult.addResult(m_path, std::make_tuple<SearchResult::position, SearchResult::prefix, SearchResult::suffix>
			(std::move(finalPosition),
				std::string(pos - preffixOffset, pos),
				std::string(pos + m_wordToSearch.size(), pos + m_wordToSearch.size() + suffixOffset)));
		++pos;
		a++;
	}
	strncpy_s(&m_data[0], m_finalBufferSize, &m_data[m_finalBufferSize - m_preambule], m_preambule - 1);
}

int8_t Parser::calculatePrefixOffset(long long p_bufferPackage, SearchResult::position p_finalPosition)
{
	bool isFirstDataPackageProcessed = p_bufferPackage == 0;
	if (isFirstDataPackageProcessed)
	{
		if (p_finalPosition < MAX_PREFIX_SIZE)
			return p_finalPosition;
	}
	return MAX_PREFIX_SIZE;
}

int8_t Parser::calculateSuffixOffset(long long p_bufferPackage, SearchResult::position p_finalPosposition, const char * p_pos)
{
	bool isLastDataPackageProcessed = m_fileStream.gcount() < m_newDataToReadSize;
	if (isLastDataPackageProcessed)
	{
		bool isWholeSuffixInFileBoundaries = (p_finalPosposition + m_wordToSearch.size() + MAX_SUFFIX_SIZE) < m_fileSize;
		if (isWholeSuffixInFileBoundaries)
			return  MAX_SUFFIX_SIZE;
		else
			return m_fileSize - p_finalPosposition - m_wordToSearch.size();
	}
	else
	{
		bool isPositionInBoundaries =
			p_bufferPackage + m_newDataToReadSize <= p_finalPosposition + m_wordToSearch.size() + MAX_SUFFIX_SIZE &&
			p_bufferPackage + m_newDataToReadSize >= p_finalPosposition + m_wordToSearch.size();

		if (isPositionInBoundaries)
		{
			return m_preambule + m_newDataToReadSize - (p_pos - &m_data[0]) - m_wordToSearch.size() - 1;
		}
		else if (p_finalPosposition + m_wordToSearch.size() + MAX_SUFFIX_SIZE < p_bufferPackage + m_newDataToReadSize)
		{
			return MAX_SUFFIX_SIZE;
		}
		else
		{
			return -1;
		}
	}
}

#pragma once
#include <iostream>

#define MAX_STRING_TO_SEARCH_SIZE 128
#define STRING_TO_SEARCH_POSITION 2
#define PATH_TO_SEARCH_POSITION 1
#define PARAM_NUM 2

bool validateParameters(int p_argc, char * p_argv[])
{
	if (p_argc != (PARAM_NUM + 1))
	{
		std::cout << "Wrong input parameters number - provided:" << p_argc - 1 << ", should be: 2" << std::endl;
		return false;
	}

	auto size = strlen(p_argv[STRING_TO_SEARCH_POSITION]);
	if (size > MAX_STRING_TO_SEARCH_SIZE)
	{
		std::cout << "Size of word to search exceeded! \tis: "<< size <<"\t\tshould be less than: "<< MAX_STRING_TO_SEARCH_SIZE << std::endl;
		return false;
	}
	return true;
}
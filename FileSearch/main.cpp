#include "ThreadPool.hpp"
#include <chrono>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <WinBase.h>
#include <handleapi.h>
#include <minwinbase.h>
#include <memory>
#include <comdef.h>
#include "ParametersValidator.h"
#include "Task.hpp"

using std::experimental::filesystem::recursive_directory_iterator;
using std::experimental::filesystem::is_directory;



int main(int argc, char * argv[])
{
	if (!validateParameters(argc, argv))
	{
		system("pause");
		return 0;
	}
	
	path pathToSearch(argv[PATH_TO_SEARCH_POSITION]);
	std::string wordToSearch(argv[STRING_TO_SEARCH_POSITION]);

	auto startTime = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> end;

	unsigned int numOfAvailableThreads = std::thread::hardware_concurrency();

	SearchResult result;

	std::mutex printMutex;
	auto threadSafePrint = [&](std::string p_string)
	{
		std::unique_lock<std::mutex>l_lock(printMutex);
		std::cout << p_string << std::endl;
	};
	TaskFactory l_taskFactory(threadSafePrint, wordToSearch, result);
	//narrowed scope for earlier l_threadPool destroy
	{
		ThreadPool l_threadPool(numOfAvailableThreads, startTime);

		if (is_directory(pathToSearch))
		{
			for (auto& dirEntry : recursive_directory_iterator(pathToSearch))
			{
				if (is_directory(dirEntry.status()))
					continue;
				l_threadPool.enqueue(l_taskFactory.createTask(dirEntry));
			}
		}
		else if(is_regular_file(pathToSearch))
		{
			l_threadPool.enqueue(l_taskFactory.createTask(pathToSearch));
		}
		else
		{
			std::cout << pathToSearch.u8string() << " is neither file or directory!" << std::endl;
		}
	}	
	result.printAll();

	system("pause");
}
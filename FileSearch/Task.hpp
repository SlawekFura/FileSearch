#pragma
#include "FileParser.h"


using std::experimental::filesystem::path;

class TaskFactory
{
	using Task = std::function<void()>;

	SearchResult& m_result;
	std::function<void(std::string)> m_sharedPrinter;
	std::string m_wordToSearch;
	std::mutex m_resultFillingMutex;

public:
	TaskFactory(std::function<void(std::string)> p_printer, std::string p_word, SearchResult& p_result)
		: m_sharedPrinter(std::move(p_printer)), m_wordToSearch(std::move(p_word)), m_result(p_result)
	{

	}

	Task createTask(const path& p_path)
	{
		return [=]() {
			std::ifstream in(p_path, std::ios::in | std::ios::binary);
			if (in)
			{
				Parser parser(in, m_wordToSearch, p_path, m_result, m_resultFillingMutex);
				parser.parse();
			}
			else
			{
				std::string errorMessage = "Can't open " + p_path.u8string() + "!";
				m_sharedPrinter(std::move(errorMessage));
			}
		};
	}
};
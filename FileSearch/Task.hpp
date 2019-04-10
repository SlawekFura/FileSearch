#pragma
#include "FileParser.h"


using std::experimental::filesystem::path;

class TaskFactory
{
	using Task = std::function<void()>;

	SearchResult& m_result;
	std::function<void(std::string)> m_sharedPrinter;
	std::string m_wordToSearch;

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
				in.seekg(0, std::ios::end);
				std::streamoff fileSize = in.tellg();
				in.seekg(0, std::ios::beg);

				size_t preambule = MAX_PREFIX_SIZE + m_wordToSearch.size() + MAX_SUFFIX_SIZE - 1;
				constexpr size_t finalBufferSize = DATA_BUFFER_SIZE;
				size_t dataBuffer = DATA_BUFFER_SIZE - preambule;
				size_t newDataToReadSize = finalBufferSize - preambule;

				char data[finalBufferSize];
				for (size_t i = 0; i < preambule; ++i) { data[i] = 27; };

				Parser parser(in, fileSize, m_wordToSearch, p_path, m_result);
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
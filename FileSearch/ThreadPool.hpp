#include <iostream>
#include <fstream>
#include <filesystem>
#include <stack>
#include <thread>
#include <functional>
#include <condition_variable>
#include <queue>



class ThreadPool
{
public:
	using Task = std::function<void()>;

	explicit ThreadPool(std::size_t numThreads, std::chrono::time_point<std::chrono::system_clock> &_startTime)
		: startTime(_startTime)
	{
		start(numThreads);
	}

	~ThreadPool()
	{
		stop();
	}

	void enqueue(Task task)
	{
		{
			std::unique_lock<std::mutex> lock{ m_eventMutex };
			m_task.emplace(task);
		}
		m_eventVar.notify_one();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> startTime;

	std::vector<std::thread> m_threads;
	std::mutex m_eventMutex;
	std::condition_variable m_eventVar;
	bool m_stopping = false;
	std::queue<Task> m_task;

	void start(std::size_t p_numThreads)
	{
		for (std::size_t i = 0; i < p_numThreads; ++i)
		{
			m_threads.emplace_back([=]()
			{
				while (true)
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock{ m_eventMutex };

						m_eventVar.wait(lock, [=] { return m_stopping || !m_task.empty(); });

						if (m_stopping && m_task.empty())
						{
							break;
						}

						task = std::move(m_task.front());
						m_task.pop();
					}
					task();
				}
			});
		}
	}

	void stop()
	{
		{
			std::unique_lock<std::mutex> lock{ m_eventMutex };
			m_stopping = true;
		}

		m_eventVar.notify_all();
		for (auto& thread : m_threads)
		{
			thread.join();
		}
		//performance testing purpose
		//std::cout << "Execution time: "<<
		//	static_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - startTime).count() << std::endl;
	}
};
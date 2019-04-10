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

	explicit ThreadPool(std::size_t p_numThreads, std::chrono::time_point<std::chrono::system_clock> p_startTime)
		: m_startTime(p_startTime)
	{
		start(p_numThreads);
	}

	~ThreadPool();
	void enqueue(Task task);
private:
	std::chrono::time_point<std::chrono::system_clock> m_startTime;

	std::vector<std::thread> m_threads;
	std::mutex m_eventMutex;
	std::condition_variable m_eventVar;
	bool m_stopping = false;
	std::queue<Task> m_task;

	void start(std::size_t p_numThreads);
	void stop();
};
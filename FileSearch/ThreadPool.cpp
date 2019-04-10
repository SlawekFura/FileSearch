#include "ThreadPool.hpp"

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::enqueue(Task task)
{
	{
		std::unique_lock<std::mutex> lock{ m_eventMutex };
		m_task.emplace(task);
	}
	m_eventVar.notify_one();
}

void ThreadPool::start(std::size_t p_numThreads)
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

void ThreadPool::stop()
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
	//	static_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - m_startTime).count() << std::endl;
}
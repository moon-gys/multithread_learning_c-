#include <assert.h>
#include <iostream>
#include <sstream>

#include "thread_pool.h"

namespace sola {

    std::unique_ptr<LoggerInterface> active_logger = nullptr;

    static const char black[] = { 0x1b, '[', '1', ';', '3', '0', 'm', 0 };
    static const char red[] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
    static const char yellow[] = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
    static const char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
    static const char normal[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
   
    Logger::Logger(LogLevel level) : m_level(level) {}
    void Logger::Debug(const std::string& msg, const std::string& file, std::size_t line)
    {
        if (m_level >= LogLevel::kDebug) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cout << "[" << black << "DEBUG" << normal << "][sola::Logger][" << file << ":" << line << "] " << msg << std::endl;
        }
    }

    void Logger::Info(const std::string& msg, const std::string& file, std::size_t line) 
    {
        if (m_level >= LogLevel::kInfo) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cout << "[" << blue << "INFO " << normal << "][sola::Logger][" << file << ":" << line << "] " << msg << std::endl;
        }
    }

    void Logger::Warn(const std::string& msg, const std::string& file, std::size_t line) 
    {
        if (m_level >= LogLevel::kWarn) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cout << "[" << yellow << "WARN " << normal << "][sola::Logger][" << file << ":" << line << "] " << msg << std::endl;
        }
    }

    void Logger::Error(const std::string& msg, const std::string& file, std::size_t line)
    {
        if (m_level >= LogLevel::kError) {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cerr << "[" << red << "ERROR" << normal << "][sola::Logger][" << file << ":" << line << "] " << msg << std::endl;
        }
    }

    void  Debug(const std::string& msg, const std::string& file, std::size_t line) {
        if (active_logger)
            active_logger->Debug(msg, file, line);
    }

    void Info(const std::string& msg, const std::string& file, std::size_t line)
    {
        if (active_logger)
            active_logger->Info(msg, file, line);
    }

    void Warn(const std::string& msg, const std::string& file, std::size_t line) 
    {
        if (active_logger)
            active_logger->Warn(msg, file, line);
    }

    void Error(const std::string& msg, const std::string& file, std::size_t line) 
    {
        if (active_logger)
            active_logger->Error(msg, file, line);
    }

    static std::string get_tid()
    {
        std::stringstream tmp;
        tmp << std::this_thread::get_id();
        return tmp.str();
    }

    ThreadPool::ThreadPool(int init_size): m_init_threads_size(init_size), m_mutex(), m_cond(), m_is_started(false)
    {
        Start();
    }

    ThreadPool::~ThreadPool()
    {
        if (m_is_started)
        {
            Stop();
        }
    }

    void ThreadPool::Start()
    {
        assert(m_threads.empty());
        m_is_started = true;
        m_threads.reserve(m_init_threads_size);
        for (int i = 0; i < m_init_threads_size; ++i)
        {
            m_threads.push_back(new std::thread(std::bind(&ThreadPool::ThreadLoop, this)));
        }

    }

    void ThreadPool::Stop()
    {
        __SOLA_LOG(debug, "thread_pool::stop() stop.");
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_is_started = false;
            m_cond.notify_all();
            __SOLA_LOG(debug, "thread_pool::stop() notifyAll().");
        }

        for (ThreadsPool::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
        {
            (*it)->join();
            delete* it;
        }
        m_threads.clear();
    }


    void ThreadPool::ThreadLoop()
    {
        __SOLA_LOG(debug, "thread_pool::threadLoop() tid : " + get_tid() + " start.");
        while (m_is_started)
        {
            TaskCallFunction task = Take();
            if (task)
            {
                task();
            }
        }
        __SOLA_LOG(debug, "thread_pool::threadLoop() tid : " + get_tid() + " exit.");
    }

    void ThreadPool::AddTask(const TaskCallFunction& task)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        /*while(m_tasks.isFull())
          {//when m_tasks have maxsize
            cond2.notify_one();
          }
        */
        m_tasks.push_back(task);
        m_cond.notify_one();
    }

    ThreadPool::TaskCallFunction ThreadPool::Take()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        //always use a while-loop, due to spurious wakeup
        while (m_tasks.empty() && m_is_started)
        {
            __SOLA_LOG(debug, "thread_pool::take() tid : " + get_tid() + " wait.");
            m_cond.wait(lock);
        }

        __SOLA_LOG(debug, "thread_pool::take() tid : " + get_tid() + " wakeup.");

        TaskCallFunction task;
        TasksPool::size_type size = m_tasks.size();
        if (!m_tasks.empty() && m_is_started)
        {
            task = m_tasks.front();
            m_tasks.pop_front();
            assert(size - 1 == m_tasks.size());
            /*if (TaskQueueSize_ > 0)
            {
              cond2.notify_one();
            }*/
        }

        return task;

    }
}
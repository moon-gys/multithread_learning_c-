#ifndef _thread_pool_H_
#define _thread_pool_H_

#include <vector>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

/*!
convenience macro to log with file and line information
*/
#ifdef __SOLA_LOGGING_ENABLED
#define __SOLA_LOG(level, msg) sola::level(msg, __FILE__, __LINE__);
#else
#define __SOLA_LOG(level, msg)
#endif /* __SOLA_LOGGING_ENABLED */

namespace sola {

	class LoggerInterface {
	public:
		LoggerInterface() {};
		virtual ~LoggerInterface() {};

		LoggerInterface(const LoggerInterface&) {};
		LoggerInterface& operator=(const LoggerInterface&) {};

		/*!
		* debug logging
		*
		* \param msg  --- message to be logged
		* \param file --- file from which the message is coming
		* \param line --- line in the file of the message
		*/
		virtual void Debug(const std::string& msg, const std::string& file, std::size_t line) = 0;

		/*!
		* info logging
		*
		* \param msg message to be logged
		* \param file --- file from which the message is coming
		* \param line --- line in the file of the message
		*/
		virtual void Info(const std::string& msg, const std::string& file, std::size_t line) = 0;

		/*!
		* warn logging
		*
		* \param msg message to be logged
		* \param file file from which the message is coming
		* \param line line in the file of the message
		*/
		virtual void Warn(const std::string& msg, const std::string& file, std::size_t line) = 0;

		/*!
		* error logging
		*
		* \param msg message to be logged
		* \param file file from which the message is coming
		* \param line line in the file of the message
		*/
		virtual void Error(const std::string& msg, const std::string& file, std::size_t line) = 0;
	};

	/*!
	 *  default logger class provided by the library
	 */
	class Logger : public LoggerInterface {
	public:

		/*!
		* log level
		*/
		enum class LogLevel {
			kError = 0,
			kWarn = 1,
			kInfo = 2,
			kDebug = 3
		};
	public:

		Logger(LogLevel level = LogLevel::kInfo);
		virtual ~Logger() {};

		Logger(const Logger&) {};
		Logger& operator=(const Logger&) {};

	public:
		/*!
		* debug logging
		*
		* \param msg  --- message to be logged
		* \param file --- file from which the message is coming
		* \param line --- line in the file of the message
		*/
		void Debug(const std::string& msg, const std::string& file, std::size_t line);

		/*  !
		* info logging
		*
		* \param msg message to be logged
		* \param file file from which the message is coming
		* \param line line in the file of the message
		*/
		void Info(const std::string& msg, const std::string& file, std::size_t line);

		/*!
		* warn logging
		*
		* \param msg message to be logged
		* \param file file from which the message is coming
		* \param line line in the file of the message
		*/
		void Warn(const std::string& msg, const std::string& file, std::size_t line);

		/*!
		* error logging
		*
		* \param msg message to be logged
		* \param file file from which the message is coming
		* \param line line in the file of the message
		*/
		void Error(const std::string& msg, const std::string& file, std::size_t line);

	private:

		/* !
		* current log level in use
		*/
		LogLevel m_level;

		/*!
		* mutex used to serialize logs in multithreaded environment
		*/
		std::mutex m_mutex;
	};

	/* !
	* variable containing the current logger
	* by default, not set (no logs)
	*/
	extern std::unique_ptr<LoggerInterface> active_logger;

	/*!
	* debug logging
	* convenience function used internally to call the logger
	*
	* \param msg  --- message to be logged
	* \param file --- file from which the message is coming
	* \param line --- line in the file of the message
	*/
	void Debug(const std::string& msg, const std::string& file, std::size_t line);

	/*!
	* info logging
	* convenience function used internally to call the logger
	*
	* \param msg message to be logged
	* \param file file from which the message is coming
	* \param line line in the file of the message
	*/
	void Info(const std::string& msg, const std::string& file, std::size_t line);

	/*!
	* warn logging
	* convenience function used internally to call the logger
	*
	* \param msg message to be logged
	* \param file file from which the message is coming
	* \param line line in the file of the message
	*/
	void Warn(const std::string& msg, const std::string& file, std::size_t line);

	/*!
	* error logging
	* convenience function used internally to call the logger
	*
	* \param msg message to be logged
	* \param file file from which the message is coming
	* \param line line in the file of the message
	*/
	void Error(const std::string& msg, const std::string& file, std::size_t line);


	class ThreadPool {
	public:
		typedef std::function<void()> TaskCallFunction;

		ThreadPool(int init_size = 3);
		~ThreadPool();

		void Stop();
		void AddTask(const TaskCallFunction&);

	private:
		ThreadPool(const ThreadPool&);
		const ThreadPool& operator=(const ThreadPool&);

		bool IsStarted() { return m_is_started; }
		void Start();

		void ThreadLoop();
		TaskCallFunction Take();

		typedef std::vector<std::thread*> ThreadsPool;
		typedef std::deque<TaskCallFunction> TasksPool;

		int m_init_threads_size;

		ThreadsPool m_threads;
		TasksPool m_tasks;

		std::mutex m_mutex;
		std::condition_variable m_cond;
		bool m_is_started;
	};

}
#endif /* _thread_pool_H_ */

#include <open/Log.h>
#include <stdarg.h>
#include <string>
#include <mutex>

namespace open
{
	struct Lock
	{
		Lock(std::mutex& m)
		:mutex(m)
		{
			mutex.lock();
		}

		~Lock()
		{
			mutex.unlock();
		}

		std::mutex& mutex;
	};

	Log::Log()
		:m_logStorage(nullptr), m_accessMask(5), m_normalMask(1)
	{
	}

	void Log::init(ILogStorage* logStorage)
	{
		m_logStorage = logStorage;
	}

	void Log::logInternal(const char* log)
	{
		if (strlen(log)<1)
			return;

		std::string processLog = log;
		if (processLog.at(processLog.size() - 1) != '\n')
		{
			processLog.append("\n");
		}
		m_logStorage->store(processLog.c_str());
	}

	void Log::log_level_args(INT32 level, const char* format, ...)
	{
		if (!validLog(level))
			return;

		va_list list;
		va_start(list, format);

		logInternal(format, list);

		va_end(list);
	}

	void Log::log_args(const char* format, ...)
	{
		if (!validLog(m_normalMask))
			return;

		va_list list;
		va_start(list, format);

		logInternal(format, list);

		va_end(list);
	}

	Log::~Log()
	{
		delete m_logStorage;
	}

	class ConsoleLogStorage : public ILogStorage
	{
	public:
		ConsoleLogStorage()
		{
			m_mutx = new std::mutex();
		}

		~ConsoleLogStorage()
		{
			//delete m_mutx;
		}

		void store(const char* log)
		{
			Lock lock(*m_mutx);
			printf("%s", log);
		}

	private:
		std::mutex* m_mutx;
	};

	void LogInit::init()
	{
		if (s_init)
			return;
		s_traceLog = new Log();
		s_infoLog = s_traceLog;

		s_traceLog->init(new ConsoleLogStorage);
	}

	Log* LogInit::s_traceLog = nullptr;

	Log* LogInit::s_infoLog = nullptr;

	bool LogInit::s_init = false;

	int LogInit::s_errorMask = 0;

	int LogInit::s_warnMask = 5;

}
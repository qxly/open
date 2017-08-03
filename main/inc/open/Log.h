#ifndef WYIN_LOG_H
#define WYIN_LOG_H

#include <open/openDef.h>

namespace open
{
	class COMMON_API ILogStorage
	{
	public:
		virtual void store(const char* log) = 0;

		virtual ~ILogStorage(){}
	};

	class  COMMON_API Log
	{
	public:
		Log();

		void init(ILogStorage* logStorage);

		void setAccessMask(int mask) { m_accessMask = mask; }

		void setNormalAccessMask(int mask) { m_normalMask = mask; }

		inline void log(const char* logInfo, int mask)
		{
			if (!validLog(mask))
				return;

			logInternal(logInfo);
		}

		inline void log(const char* logInfo)
		{
			log(logInfo, m_normalMask);
		}

		void log_level_args(INT32 level, const char* format, ...);

		void log_args(const char* format, ...);

		inline bool validLog(int mask) { return mask < m_accessMask; }

		~Log();

	private:
		void logInternal(const char* logInfo);

		inline void logInternal(const char* format, va_list list)
		{
			const int length = 4096;
			char buffer[length];

#ifdef _WINDOWS
			int count = _vsnprintf(buffer, length - 1, format, list);
#else
			int count = vsnprintf(buffer, length - 1, format, list);
#endif
			if (count == -1 || count >= length)
			{
				count = sizeof(buffer)-1;
			}
			logInternal(buffer);
		}

	private:
		ILogStorage* m_logStorage;

		int m_accessMask;
		int m_normalMask;
	};

	class COMMON_API LogInit
	{
	public:
		static void init(Log* traceLog, Log* infoLog)
		{
			if (s_init)
				return;

			s_init = true;
			s_traceLog = traceLog;
			s_infoLog = infoLog;
		}


		static void init();

		static void destroy()
		{
			if (s_traceLog != s_infoLog)
			{
				if (s_traceLog)
				{
					delete s_traceLog;
					s_traceLog = nullptr;
				}
				if (s_infoLog)
				{
					delete s_infoLog;
					s_infoLog = nullptr;
				}
			}
			else
			{
				if (s_traceLog)
				{
					delete s_traceLog;
					s_traceLog = nullptr;
				}
			}
			s_init = false;
		}

		///跟踪型日志的输出log
		static Log* s_traceLog;

		///信息型日志的输出log
		static Log* s_infoLog;

		///必须要初始化
		static bool s_init;

		///结合s_traceLog和s_errorMask可动态设置错误型日志的开关
		static int s_errorMask;

		///结合s_traceLog和s_errorMask可动态设置警告型日志的开关
		static int s_warnMask;
	};
}


///是否开启错误型日志,1开启，0关闭
#define HAVE_LOG_ERROE 1

///是否开启警告型日志,1开启，0关闭
#define HAVE_LOG_WARN 1

///是否开启信息型日志,1开启，0关闭
#define HAVE_LOG_INFO 1

#if HAVE_LOG_ERROE
///记录错误，并抛出异常
#define LOG_ERROR(file,line,info)\
{\
if (open::LogInit::s_traceLog){\
char __error__[1024]; \
_vsnprintf(__error__, 1024 "%s %d line,error: %s", file, line, info); \
open::LogInit::s_traceLog->log(__error__, LogInit::s_errorMask); \
throw Exception(__error__);}\
}


#define LOG_ERROR_ARGS(foramt,...)\
{\
if (open::LogInit::s_traceLog){\
open::LogInit::s_traceLog->log_level_args(open::LogInit::s_errorMask, foramt, __VA_ARGS__);} \
}


#else
///无意义
#define LOG_ERROR(file,line,info);
#define LOG_ERROR_ARGS(foramt,...);
#endif

#if HAVE_LOG_WARN
///记录警告信息和跟踪型信息结合使用
#define LOG_WARN(file,line,info)\
{\if (open::LogInit::s_traceLog  && open::LogInit::s_traceLog->validLog(LogInit::s_warnMask)){\
char __warn__[1024]; \
_vsnprintf(__warn__, 1024, "%s %d line,warn: %s", file, line, info); \
open::LogInit::s_traceLog->log(__warn__, open::LogInit::s_warnMask);}\
}


#define LOG_WARN_ARGS(foramt,...)\
{if (open::LogInit::s_traceLog && open::LogInit::s_traceLog->validLog(LogInit::s_warnMask)){\
open::LogInit::s_traceLog->log_level_args(LogInit::s_warnMask, foramt, __VA_ARGS__);}\
}


#else
///无意义
#define LOG_WARN(file,line,info);
#define LOG_WARN_ARGS(level,foramt,...);
#endif

#if HAVE_LOG_INFO
///记录一般信息
#define LOG_INFO(info)\
if (open::LogInit::s_infoLog){\
open::LogInit::s_infoLog->log(info);\
}
///使用默认级别1输出info
#define LOG_INFO_ARGS(foramt,...)\
if (open::LogInit::s_infoLog && open::LogInit::s_infoLog->validLog(1)){\
open::LogInit::s_infoLog->log_level_args(1, foramt, __VA_ARGS__);\
}
///指定输出基本，记录一般信息，支持不定参数
#define LOG_INFO_LEVEL(level,foramt,...)\
if (open::LogInit::s_infoLog && open::LogInit::s_infoLog->validLog(level)){\
open::LogInit::s_infoLog->log_level_args(level, foramt, __VA_ARGS__);\
}


#else
///无意义
#define LOG_INFO(info);
///无意义
#define LOG_INFO_ARGS(foramt,...);

#endif

#ifdef _DEBUG
#define LOG_DEBUG(foramt,...)\
if (open::LogInit::s_infoLog && open::LogInit::s_infoLog->validLog(-1)){\
open::LogInit::s_infoLog->log_level_args(-1, foramt, __VA_ARGS__);\
}
#else
#define LOG_DEBUG(foramt,...)
#endif

#endif
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

		///��������־�����log
		static Log* s_traceLog;

		///��Ϣ����־�����log
		static Log* s_infoLog;

		///����Ҫ��ʼ��
		static bool s_init;

		///���s_traceLog��s_errorMask�ɶ�̬���ô�������־�Ŀ���
		static int s_errorMask;

		///���s_traceLog��s_errorMask�ɶ�̬���þ�������־�Ŀ���
		static int s_warnMask;
	};
}


///�Ƿ�����������־,1������0�ر�
#define HAVE_LOG_ERROE 1

///�Ƿ�����������־,1������0�ر�
#define HAVE_LOG_WARN 1

///�Ƿ�����Ϣ����־,1������0�ر�
#define HAVE_LOG_INFO 1

#if HAVE_LOG_ERROE
///��¼���󣬲��׳��쳣
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
///������
#define LOG_ERROR(file,line,info);
#define LOG_ERROR_ARGS(foramt,...);
#endif

#if HAVE_LOG_WARN
///��¼������Ϣ�͸�������Ϣ���ʹ��
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
///������
#define LOG_WARN(file,line,info);
#define LOG_WARN_ARGS(level,foramt,...);
#endif

#if HAVE_LOG_INFO
///��¼һ����Ϣ
#define LOG_INFO(info)\
if (open::LogInit::s_infoLog){\
open::LogInit::s_infoLog->log(info);\
}
///ʹ��Ĭ�ϼ���1���info
#define LOG_INFO_ARGS(foramt,...)\
if (open::LogInit::s_infoLog && open::LogInit::s_infoLog->validLog(1)){\
open::LogInit::s_infoLog->log_level_args(1, foramt, __VA_ARGS__);\
}
///ָ�������������¼һ����Ϣ��֧�ֲ�������
#define LOG_INFO_LEVEL(level,foramt,...)\
if (open::LogInit::s_infoLog && open::LogInit::s_infoLog->validLog(level)){\
open::LogInit::s_infoLog->log_level_args(level, foramt, __VA_ARGS__);\
}


#else
///������
#define LOG_INFO(info);
///������
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
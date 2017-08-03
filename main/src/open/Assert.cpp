#ifdef _DEBUG

#include <open/openDef.h>

namespace open
{
	void assertInternal(const char* exp, const char *error, const char *file, unsigned int line)
	{
		char buffer[1024];
		if (error)
			sprintf(buffer, "ASSERT_ :  %s \n in file:  %s \n row:  %d \n msg: %s\n", exp, file, line, error);
		else
			sprintf(buffer, "ASSERT_ :  %s \n in file:  %s \n row:  %d \n", exp, file, line);

#if defined _WINDOWS || defined _WIN32 || defined _WIN64

		int next = ::MessageBox(nullptr, buffer, "ASSERT_ error", MB_ABORTRETRYIGNORE);

		if (next == 3)///÷’÷π
		{
			exit(0);
		}

		else if (next == 4)///÷ÿ ‘
		{
			DebugBreak();
		}
		else//∫ˆ¬‘
			return;
#else

#endif
	}

	void assertInternal(const char* exp, const char *file, unsigned int line, const char *format, ...)
	{
		va_list list;
		va_start(list, format);

		const int length = 4096;
		char buffer[length];
		int count = 0;

#if defined _WINDOWS || defined _WIN32 || defined _WIN64
		count = _vsnprintf(buffer, length - 1, format, list);
#else
		count = vsnprintf(buffer, length - 1, format, list);
#endif
		va_end(list);

		assertInternal(exp, buffer, file, line);
	}
}

#endif
#ifndef WYIN_DEF_H
#define WYIN_DEF_H 1

#pragma warning (disable:4996)

#include <stdio.h>
#include <stddef.h>
#include<stdlib.h>
#include <stdarg.h>
#include<string.h>
#include <memory.h>
#include <math.h>
#include <time.h>

#if defined _WINDOWS || defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#endif

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 8 bit integer
typedef char int8;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 8 bit unsigned integer
typedef unsigned char uint8;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 16 bit integer
typedef short int16;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 16 bit unsigned integer
typedef unsigned short uint16;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 32 bit integer
typedef int int32;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 32 bit unsigned integer
typedef unsigned int uint32;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 64 bit integer
typedef long long int64;

//////////////////////////////////////////////////////////////////////////////
/// \brief
/// 64 bit unsigned integer
typedef unsigned long long uint64;

typedef uint32 Handle;

#define DOUBLE_PRECISION 0

#if DOUBLE_PRECISION
#define Real double
#else
#define Real float
#endif

#if defined _WINDOWS || defined _WIN32 || defined _WIN64
#ifdef COMMON_EXPORTS
#    define COMMON_API __declspec(dllexport)
#else
#    define COMMON_API __declspec(dllimport)
#endif
#else
#define COMMON_API
#endif

#if _DEBUG

namespace open
{
	extern COMMON_API void assertInternal(const char* exp, const char *error, const char *file, unsigned int line);

	extern COMMON_API void assertInternal(const char* exp, const char *file, unsigned int line, const char *format, ...);
}

#define ASSERT_(exp)\
if ((!(exp))) open::assertInternal(#exp, nullptr, __FILE__, __LINE__);

#define ASSERT_ERROR(exp,error)\
if ((!(exp))) open::assertInternal(#exp, error, __FILE__, __LINE__);

#define ASSERT_ARGS(exp,format,...)\
if ((!(exp))) open::assertInternal(#exp, __FILE__, __LINE__, format, __VA_ARGS__);

#else
#define ASSERT_(exp)
#define ASSERT_ERROR(exp,error)
#define ASSERT_ARGS(exp,format,...)

#endif

#endif
/********************************************************************
* Copyright (c) zoomway 2010-2015
* FileName:   Execption.h
* Version:    v3.0
* Author:     wyin
* Datetime:   2015/1/7   15:40
* Purpose:
* Changelist: none
********************************************************************/

#ifndef GISCORE_COMMON_EXCEPTION_H_
#define GISCORE_COMMON_EXCEPTION_H_

#include <gis_core/common/GISCoreDef.h>
#include <gis_core/common/String.h>

#ifdef _WINDOWS
#pragma warning (disable:4290)
#endif

namespace GISCore
{
	class GISCORE_API Exception
	{
	public:
		Exception(const char* str, INT32 errorCode = 0);

		~Exception();

		const char* what() const;

		string track();

	private:
		string m_what;
		INT32 m_errorCode;
	};
}

#endif
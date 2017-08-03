#ifndef BSON_HEX_H
#define BSON_HEX_H

#include "../bsonassert.h"
#include "builder.h"

namespace bson
{

	//can't use hex namespace because it conflicts with hex iostream function
	inline int fromHex(char c)
	{
		if ('0' <= c && c <= '9')
			return c - '0';
		if ('a' <= c && c <= 'f')
			return c - 'a' + 10;
		if ('A' <= c && c <= 'F')
			return c - 'A' + 10;
		assert(false);
		return 0xff;
	}

	inline char fromHex(const char *c)
	{
		return (char)((fromHex(c[0]) << 4) | fromHex(c[1]));
	}

	inline string toHex(const void* inRaw, int len)
	{
		static const char hexchars[] = "0123456789ABCDEF";

		StringBuilder out;
		const char* in = reinterpret_cast<const char*>(inRaw);
		for (int i = 0; i < len; ++i)
		{
			char c = in[i];
			char hi = hexchars[(c & 0xF0) >> 4];
			char lo = hexchars[(c & 0x0F)];

			out << hi << lo;
		}

		return out.str();
	}

	inline string toHexLower(const void* inRaw, int len)
	{
		static const char hexchars[] = "0123456789abcdef";

		StringBuilder out;
		const char* in = reinterpret_cast<const char*>(inRaw);
		for (int i = 0; i < len; ++i)
		{
			char c = in[i];
			char hi = hexchars[(c & 0xF0) >> 4];
			char lo = hexchars[(c & 0x0F)];

			out << hi << lo;
		}

		return out.str();
	}
}

#endif
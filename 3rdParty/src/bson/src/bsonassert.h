#ifndef BSON_ASSERT_H
#define BSON_ASSERT_H

#include <cstdlib>
#include <memory>
#include <iostream>
#include <sstream>
#include <boost/utility.hpp>

#include "stringdata.h"

namespace bson
{
	using std::string;
	using std::stringstream;

	class assertion : public std::exception
	{
	public:
		assertion(unsigned u, const string& s)
			: id(u), msg(s)
		{
			stringstream ss;
			ss << "BsonAssertion id: " << u << " " << s;
			full = ss.str();
		}

		virtual ~assertion() throw() {}

		virtual const char* what() const throw() { return full.c_str(); }

		unsigned id;
		string msg;
		string full;
	};

#if !defined(assert)
	inline void assert(bool expr)
	{
		if (!expr)
		{
			throw bson::assertion(0, "assertion failure in bson library");
		}
	}
#endif
#if !defined(uassert)
	inline void uasserted(unsigned msgid, std::string s)
	{
		throw bson::assertion(msgid, s);
	}

	inline void uassert(unsigned msgid, std::string msg, bool expr)
	{
		if (!expr)
			uasserted(msgid, msg);
	}

	inline void msgasserted(int msgid, const char *msg)
	{
		throw bson::assertion(msgid, msg);
	}

	inline void msgasserted(int msgid, const std::string &msg) { msgasserted(msgid, msg.c_str()); }

	inline void massert(unsigned msgid, std::string msg, bool expr)
	{
		if (!expr)
		{
			std::cout << "assertion failure in bson library: " << msgid << ' ' << msg << std::endl;
			throw bson::assertion(msgid, msg);
		}
	}
#endif
}

#endif
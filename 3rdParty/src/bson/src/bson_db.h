#ifndef BSON_DB_H
#define BSON_DB_H

#include "util/optime.h"
#include "util/time_support.h"

#ifndef log
#define log(...) std::cerr
#endif

namespace bson
{
	inline BSONObjBuilder& BSONObjBuilder::appendTimestamp(
		const StringData& fieldName, unsigned long long time, unsigned int inc)
	{
		OpTime t((unsigned)(time / 1000), inc);
		appendTimestamp(fieldName, t.asDate());
		return *this;
	}

	inline OpTime BSONElement::_opTime() const
	{
		if (type() == bson::Date || type() == Timestamp)
			return OpTime(*reinterpret_cast<const unsigned long long*>(value()));
		return OpTime();
	}

	inline string BSONElement::_asCode() const
	{
		switch (type())
		{
		case bson::String:
		case Code:
			return string(valuestr(), valuestrsize() - 1);
		case CodeWScope:
			return string(codeWScopeCode(), *(int*)(valuestr()) - 1);
		default:
			log() << "can't convert type: " << (int)(type()) << " to code"
				<< endl;
		}
		uassert(10062, "not code", 0);
		return "";
	}

	inline BSONObjBuilder& BSONObjBuilderValueStream::operator<<(
		DateNowLabeler& id)
	{
		_builder->appendDate(_fieldName, jsTime());
		_fieldName = 0;
		return *_builder;
	}

	inline BSONObjBuilder& BSONObjBuilderValueStream::operator<<(
		MinKeyLabeler& id)
	{
		_builder->appendMinKey(_fieldName);
		_fieldName = 0;
		return *_builder;
	}

	inline BSONObjBuilder& BSONObjBuilderValueStream::operator<<(
		MaxKeyLabeler& id)
	{
		_builder->appendMaxKey(_fieldName);
		_fieldName = 0;
		return *_builder;
	}

}

#endif
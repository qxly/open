#ifndef BSON_JSON_H
#define BSON_JSON_H

#include "../bson.h"

namespace bson
{
	BSONObj fromjson(const string &str);

	/** len will be size of JSON object in text chars. */
	BSONObj fromjson(const char *str, int* len = NULL);

} // namespace mongo

#endif
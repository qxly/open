#ifndef BSON_STRINGDATA_H
#define BSON_STRINGDATA_H

#include <string>
#include <cstring>

namespace bson
{
	using std::string;

	/* A StringData object wraps a 'const string&' or a 'const char*' without
	* copying its contents. The most common usage is as a function argument
	* that takes any of the two forms of strings above. Fundamentally, this
	* class tries go around the fact that string literals in C++ are char[N]'s.
	* Note that the object StringData wraps around must be alive while the
	* StringData is.
	*/

	class StringData
	{
	public:
		/** Construct a StringData, for the case where the length of
		* string is not known. 'c' must be a pointer to a null-terminated
		* string.
		*/
		StringData(const char* c)
			: _data(c), _size((unsigned)strlen(c)) {}

		/** Construct a StringData explicitly, for the case where the length of
		* the string is already known. 'c' must be a pointer to a null-
		* terminated string, and strlenOfc must be the length that
		* std::strlen(c) would return, a.k.a the index of the terminator in c.
		*/
		StringData(const char* c, unsigned len)
			: _data(c), _size(len) {}

		/** Construct a StringData, for the case of a std::string. */
		StringData(const string& s)
			: _data(s.c_str()), _size((unsigned)s.size()) {}

		// Construct a StringData explicitly, for the case of a literal whose
		// size is known at compile time.
		struct LiteralTag {};
		template<size_t N>
		StringData(const char(&val)[N], LiteralTag)
			: _data(&val[0]), _size(N - 1) {}

		// accessors
		const char* const data() const { return _data; }
		const unsigned size() const { return _size; }

	private:
		const char* const _data;  // is always null terminated
		const unsigned    _size;  // 'size' does not include the null terminator
	};

} // namespace bson

#endif
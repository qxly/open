#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <open/openDef.h>

namespace open
{
	class COMMON_API Config
	{
		struct ConfigNode;
		struct ConfigSection;
		struct ConfigProperty;
		struct ConfigComment;
		struct ConfigDocoment;

	public:

		Config();

		~Config();

		void getStringValue(const char* sectionName, const char* propName, char(&value)[128], const char* defaultValue = "") const;

		void getDoubleValue(const char* sectionName, const char* propName, double& propValue, double defaultValue = 0) const;

		void getIntValue(const char* sectionName, const char* propName, int32& propValue, int32 defaultValue = 0) const;

		void getBoolValue(const char* sectionName, const char* propName, bool& propValue, bool defaultValue = false) const;

		bool setProperty(const char* sectionName, const char* propName, const char* describeContent);

		bool loadConfig(const char* fileName);

	private:

		bool getValue(const char* sectionName, const char* propName, char(&value)[128]) const;

		char* loadFile(const char* fileName);

	private:
		ConfigDocoment* _document;
	};
}

#endif
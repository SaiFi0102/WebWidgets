#ifndef WW_CACHE_CONFIGURATION_H
#define WW_CACHE_CONFIGURATION_H

#include "DboDatabase/ConfigurationsDatabase.h"

namespace WW
{
	class ConfigurationCache
	{
	protected:
		typedef ConfigurationsDatabase::BoolMap BoolMap;
		typedef ConfigurationsDatabase::DoubleMap DoubleMap;
		typedef ConfigurationsDatabase::EnumMap EnumMap;
		typedef ConfigurationsDatabase::FloatMap FloatMap;
		typedef ConfigurationsDatabase::IntMap IntMap;
		typedef ConfigurationsDatabase::LongIntMap LongIntMap;
		typedef ConfigurationsDatabase::StringMap StringMap;

	public:
		ConfigurationCache(ConfigurationsDatabase *database);

		Ddo::ptr<Ddo::ConfigurationBool> getBoolPtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationDouble> getDoublePtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationEnum> getEnumPtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationFloat> getFloatPtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationInt> getIntPtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationLongInt> getLongIntPtr(const std::string &name, long long moduleId) const;
		Ddo::ptr<Ddo::ConfigurationString> getStringPtr(const std::string &name, long long moduleId) const;

		bool getBool(const std::string &name, long long moduleId, bool defaultValue) const;
		double getDouble(const std::string &name, long long moduleId, double defaultValue) const;
		int getEnum(const std::string &name, long long moduleId, int defaultValue) const;
		float getFloat(const std::string &name, long long moduleId, float defaultValue) const;
		int getInt(const std::string &name, long long moduleId, int defaultValue) const;
		long long getLongInt(const std::string &name, long long moduleId, long long defaultValue) const;
		std::string getStr(const std::string &name, long long moduleId, std::string defaultValue = "") const;

		ConfigurationsDatabase *database() { return _database; }

	protected:
		BoolMap _boolMap;
		DoubleMap _doubleMap;
		EnumMap _enumMap;
		FloatMap _floatMap;
		IntMap _intMap;
		LongIntMap _longIntMap;
		StringMap _stringMap;

		ConfigurationsDatabase *_database;
	};
}

#endif

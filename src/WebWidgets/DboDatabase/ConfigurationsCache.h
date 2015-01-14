#ifndef CONFIGURATIONS_CACHE_H
#define CONFIGURATIONS_CACHE_H

#include "DboDatabase/ConfigurationsDatabase.h"

class ConfigurationsCache
{
	protected:
		typedef ConfigurationsDatabase::BoolMaps BoolMaps;
		typedef ConfigurationsDatabase::DoubleMaps DoubleMaps;
		typedef ConfigurationsDatabase::EnumMaps EnumMaps;
		typedef ConfigurationsDatabase::FloatMaps FloatMaps;
		typedef ConfigurationsDatabase::IntMaps IntMaps;
		typedef ConfigurationsDatabase::LongIntMaps LongIntMaps;
		typedef ConfigurationsDatabase::StringMaps StringMaps;

	public:
		ConfigurationsCache(ConfigurationsDatabase *Database);

		boost::shared_ptr<ConfigurationBoolData> GetBoolPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationDoubleData> GetDoublePtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationEnumData> GetEnumPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationFloatData> GetFloatPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationIntData> GetIntPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationLongIntData> GetLongIntPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<ConfigurationStringData> GetStringPtr(const std::string &Name, long long ModuleId) const;

		bool GetBool(const std::string &Name, long long ModuleId, bool Default) const;
		double GetDouble(const std::string &Name, long long ModuleId, double Default) const;
		int GetEnum(const std::string &Name, long long ModuleId, int Default) const;
		float GetFloat(const std::string &Name, long long ModuleId, float Default) const;
		int GetInt(const std::string &Name, long long ModuleId, int Default) const;
		long long GetLongInt(const std::string &Name, long long ModuleId, long long Default) const;
		std::string GetStr(const std::string &Name, long long ModuleId, std::string Default = "") const;

		ConfigurationsDatabase *Database() { return _Database; }

	protected:
		BoolMaps BoolMap;
		DoubleMaps DoubleMap;
		EnumMaps EnumMap;
		FloatMaps FloatMap;
		IntMaps IntMap;
		LongIntMaps LongIntMap;
		StringMaps StringMap;

		ConfigurationsDatabase *_Database;
};

#endif

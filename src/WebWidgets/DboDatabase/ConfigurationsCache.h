#ifndef CONFIGURATIONS_CACHE_H
#define CONFIGURATIONS_CACHE_H

#include "Dbo/Configuration.h"
#include <boost/unordered_map.hpp>

class ConfigurationsDatabase;
class Application;

class ConfigurationsCache
{
	protected:
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationBool> > BoolMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationDouble> > DoubleMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationEnum> > EnumMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationFloat> > FloatMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationInt> > IntMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationLongInt> > LongIntMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationString> > StringMaps;

	public:
		ConfigurationsCache(ConfigurationsDatabase *Database, Application *App);

		Wt::Dbo::ptr<ConfigurationBool> GetBoolPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationDouble> GetDoublePtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationEnum> GetEnumPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationFloat> GetFloatPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationInt> GetIntPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationLongInt> GetLongIntPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationString> GetStringPtr(const std::string &Name, long long ModuleId) const;

		bool GetBool(const std::string &Name, long long ModuleId, bool Default) const;
		double GetDouble(const std::string &Name, long long ModuleId, double Default) const;
		int GetEnum(const std::string &Name, long long ModuleId, int Default) const;
		float GetFloat(const std::string &Name, long long ModuleId, float Default) const;
		int GetInt(const std::string &Name, long long ModuleId, int Default) const;
		long long GetLongInt(const std::string &Name, long long ModuleId, long long Default) const;
		std::string GetStr(const std::string &Name, long long ModuleId, std::string Default = "") const;

		ConfigurationsDatabase *Database();

	protected:
		BoolMaps BoolMap;
		DoubleMaps DoubleMap;
		EnumMaps EnumMap;
		FloatMaps FloatMap;
		IntMaps IntMap;
		LongIntMaps LongIntMap;
		StringMaps StringMap;

		ConfigurationsDatabase *_Database;
		Application *_App;
};

#endif

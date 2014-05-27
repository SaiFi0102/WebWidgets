#ifndef CONFIGURATIONS_DATABASE_H
#define CONFIGURATIONS_DATABASE_H

#include "Dbo/Configuration.h"
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>

class WServer;

class ConfigurationsDatabase
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
		ConfigurationsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		ConfigurationsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll();

		Wt::Dbo::ptr<ConfigurationBool> GetBoolPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationDouble> GetDoublePtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationEnum> GetEnumPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationFloat> GetFloatPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationInt> GetIntPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationLongInt> GetLongIntPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationString> GetStringPtr(const std::string &Name, long long ModuleId) const;

		bool GetBool(const std::string &Name, long long ModuleId, bool Default) const;
		double GetDouble(const std::string &Name, long long ModuleId, double Default = 0) const;
		int GetEnum(const std::string &Name, long long ModuleId, int Default) const;
		float GetFloat(const std::string &Name, long long ModuleId, float Default = 0) const;
		int GetInt(const std::string &Name, long long ModuleId, int Default = 0) const;
		long long GetLongInt(const std::string &Name, long long ModuleId, long long Default = 0) const;
		std::string GetStr(const std::string &Name, long long ModuleId, std::string Default = "") const;

		long long GetLoadDurationinMS() const;
		std::size_t CountConfigurations() const;

	protected:
		void MapClasses();

		BoolMaps BoolMap;
		DoubleMaps DoubleMap;
		EnumMaps EnumMap;
		FloatMaps FloatMap;
		IntMaps IntMap;
		LongIntMaps LongIntMap;
		StringMaps StringMap;

		Wt::Dbo::Session DboSession;
		WServer &_Server;
		boost::posix_time::time_duration LoadDuration;
		std::size_t Count;
		mutable boost::shared_mutex mutex;
};

#endif
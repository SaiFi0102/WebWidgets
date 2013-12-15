#ifndef CONFIGURATIONS_DATABASE_H
#define CONFIGURATIONS_DATABASE_H

#include "Dbo/Configuration.h"
#include "Application/WServer.h"
#include <Wt/Dbo/Query>
#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>

class ConfigurationsDatabase
{
	protected:
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationBool> > BoolMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationDouble> > DoubleMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationEnum> > EnumMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationFloat> > FloatMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationInt> > IntMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, Wt::Dbo::ptr<ConfigurationString> > StringMaps;

	public:
		ConfigurationsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		ConfigurationsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll();

		ConfigurationBool GetBoolDbo(const std::string &Name, long long ModuleId) const;
		ConfigurationDouble GetDoubleDbo(const std::string &Name, long long ModuleId) const;
		ConfigurationEnum GetEnumDbo(const std::string &Name, long long ModuleId) const;
		ConfigurationFloat GetFloatDbo(const std::string &Name, long long ModuleId) const;
		ConfigurationInt GetIntDbo(const std::string &Name, long long ModuleId) const;
		ConfigurationString GetStringDbo(const std::string &Name, long long ModuleId) const;

		bool GetBool(const std::string &Name, long long ModuleId, bool Default) const;
		double GetDouble(const std::string &Name, long long ModuleId, double Default = 0) const;
		int GetEnum(const std::string &Name, long long ModuleId, int Default) const;
		float GetFloat(const std::string &Name, long long ModuleId, float Default = 0) const;
		int GetInt(const std::string &Name, long long ModuleId, int Default = 0) const;
		std::string GetStr(const std::string &Name, long long ModuleId, std::string Default = "") const;

		long long GetLoadDurationinMS() const;
		int CountConfigurations() const;

	protected:
		virtual void MapClasses();
		Wt::Dbo::ptr<ConfigurationBool> GetBoolPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationDouble> GetDoublePtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationEnum> GetEnumPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationFloat> GetFloatPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationInt> GetIntPtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<ConfigurationString> GetStringPtr(const std::string &Name, long long ModuleId) const;

		BoolMaps BoolMap;
		DoubleMaps DoubleMap;
		EnumMaps EnumMap;
		FloatMaps FloatMap;
		IntMaps IntMap;
		StringMaps StringMap;

		Wt::Dbo::Session DboSession;
		WServer &_Server;
		boost::posix_time::time_duration LoadDuration;
		int Count;
		mutable boost::shared_mutex mutex;
};

#endif
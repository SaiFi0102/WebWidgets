#ifndef CONFIGURATIONS_DATABASE_H
#define CONFIGURATIONS_DATABASE_H

#include "Dbo/Configuration.h"
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>

class WServer;

class ConfigurationsDatabase
{
	protected:
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationBoolData> > BoolMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationDoubleData> > DoubleMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationEnumData> > EnumMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationFloatData> > FloatMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationIntData> > IntMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationLongIntData> > LongIntMaps;
		typedef boost::unordered_map< std::pair<long long, std::string>, boost::shared_ptr<ConfigurationStringData> > StringMaps;

	public:
		ConfigurationsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		ConfigurationsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void Load() { FetchAll(); }
		void Reload() { FetchAll(); }

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

		long long GetLoadDurationinMS() const;
		std::size_t CountConfigurations() const;

	protected:
		void MapClasses();
		void FetchAll();

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

	private:
		friend class ReadLock;
		friend class ConfigurationsCache;
};

#endif
#ifndef CONFIGURATIONS_DATABASE_H
#define CONFIGURATIONS_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Configuration.h"

class ConfigurationsDatabase : public AbstractDboDatabase
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
		ConfigurationsDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const ConfigurationBoolData> GetBoolPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationDoubleData> GetDoublePtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationEnumData> GetEnumPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationFloatData> GetFloatPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationIntData> GetIntPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationLongIntData> GetLongIntPtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const ConfigurationStringData> GetStringPtr(const std::string &Name, long long ModuleId) const;

		bool GetBool(const std::string &Name, long long ModuleId, bool Default) const;
		double GetDouble(const std::string &Name, long long ModuleId, double Default) const;
		int GetEnum(const std::string &Name, long long ModuleId, int Default) const;
		float GetFloat(const std::string &Name, long long ModuleId, float Default) const;
		int GetInt(const std::string &Name, long long ModuleId, int Default) const;
		long long GetLongInt(const std::string &Name, long long ModuleId, long long Default) const;
		std::string GetStr(const std::string &Name, long long ModuleId, std::string Default = "") const;

		long long GetLoadDurationinMS() const;
		std::size_t CountConfigurations() const;

		virtual std::string Name() const { return "ConfigurationsDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession) { FetchAll(DboSession); }
		void FetchAll(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		BoolMaps BoolMap;
		DoubleMaps DoubleMap;
		EnumMaps EnumMap;
		FloatMaps FloatMap;
		IntMaps IntMap;
		LongIntMaps LongIntMap;
		StringMaps StringMap;

		boost::posix_time::time_duration LoadDuration;
		std::size_t Count;

	private:
		friend class ConfigurationsCache;
};

#endif
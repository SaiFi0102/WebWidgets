#ifndef WW_DBODATABASE_CONFIGURATION_H
#define WW_DBODATABASE_CONFIGURATION_H

#include <unordered_map>
#include "Objects/PairHash.h"
#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class ConfigurationsDatabase : public AbstractDboDatabase
	{
	protected:
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationBool> > BoolMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationDouble> > DoubleMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationEnum> > EnumMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationFloat> > FloatMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationInt> > IntMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationLongInt> > LongIntMap;
		typedef std::unordered_map< std::pair<long long, std::string>, Ddo::ptr<Ddo::ConfigurationString> > StringMap;

	public:
		ConfigurationsDatabase(DboDatabaseManager *Manager);

		Ddo::cPtr<Ddo::ConfigurationBool> getBoolPtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationDouble> getDoublePtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationEnum> getEnumPtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationFloat> getFloatPtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationInt> getIntPtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationLongInt> getLongIntPtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::ConfigurationString> getStringPtr(const std::string &name, long long moduleId) const;

		bool getBool(const std::string &name, long long moduleId, bool defaultValue) const;
		double getDouble(const std::string &name, long long moduleId, double defaultValue) const;
		int getEnum(const std::string &name, long long moduleId, int defaultValue) const;
		float getFloat(const std::string &name, long long moduleId, float defaultValue) const;
		int getInt(const std::string &name, long long moduleId, int defaultValue) const;
		long long getLongInt(const std::string &name, long long moduleId, long long defaultValue) const;
		std::string getStr(const std::string &name, long long moduleId, std::string defaultValue = "") const;

		long long getLoadDurationinMS() const;
		std::size_t configurationsCount() const;

		virtual std::string name() const override { return "ConfigurationsDatabase"; }

	protected:
		virtual void load(Wt::Dbo::Session &DboSession) override;
		virtual void reload(Wt::Dbo::Session &DboSession) override { fetchAll(DboSession); }
		void fetchAll(Wt::Dbo::Session &DboSession);

		virtual bool isVital() const override { return true; }

		BoolMap _boolMap;
		DoubleMap _doubleMap;
		EnumMap _enumMap;
		FloatMap _floatMap;
		IntMap _intMap;
		LongIntMap _longIntMap;
		StringMap _stringMap;

		boost::posix_time::time_duration _loadDuration;
		std::size_t _count;

	private:
		friend class ConfigurationCache;
	};
}

#endif
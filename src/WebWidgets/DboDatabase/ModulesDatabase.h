#ifndef MODULES_DATABASE_H
#define MODULES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Module.h"
#include <boost/unordered_map.hpp>

class ModulesDatabase : public AbstractDboDatabase
{
	public:
		enum BuiltInModules
		{
			Wt = 1,
			Server = 2,
			Logging = 3,
			Configurations = 4,
			Localization = 5,
			Authentication = 6,
			Styles = 7,
			Navigation = 8
		};

		ModulesDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const ModuleData> GetPtr(long long Id) const;
		std::size_t CountModules() const;
		long long GetLoadDurationinMS() const;

		virtual std::string Name() const { return "ModulesDatabase"; }

	protected:
		typedef boost::unordered_map< long long, boost::shared_ptr<ModuleData> > ModuleMaps;

		void FetchAll(Wt::Dbo::Session &DboSession);
		virtual void Load(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		ModuleMaps ModuleMap;
		boost::posix_time::time_duration LoadDuration;
};

#endif
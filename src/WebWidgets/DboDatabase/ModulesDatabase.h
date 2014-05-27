#ifndef MODULES_DATABASE_H
#define MODULES_DATABASE_H

#include "Dbo/Module.h"
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>

class WServer;

class ModulesDatabase
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

		ModulesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		ModulesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll();

		Wt::Dbo::ptr<Module> GetPtr(long long Id) const;
		std::size_t CountModules() const;
		long long GetLoadDurationinMS() const;

	protected:
		typedef boost::unordered_map< long long, Wt::Dbo::ptr<Module> > ModuleMaps;

		void MapClasses();

		ModuleMaps ModuleMap;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
		mutable boost::shared_mutex mutex;
};

#endif
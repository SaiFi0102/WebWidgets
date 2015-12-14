#ifndef WW_DBODATABASE_MODULE_H
#define WW_DBODATABASE_MODULE_H

#include <unordered_map>
#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class ModuleDatabase : public AbstractDboDatabase
	{
	public:
		enum BuiltInModules
		{
			Wt = 1,
			Server = 2,
			Logging = 3,
			Configuration = 4,
			Localization = 5,
			Authentication = 6,
			Styling = 7,
			Navigation = 8
		};

		ModuleDatabase(DboDatabaseManager *manager);

		Ddo::cPtr<Ddo::Module> getPtr(long long id) const;
		std::size_t modulesCount() const;
		long long getLoadDurationinMS() const;

		virtual std::string name() const override { return "ModulesDatabase"; }

	protected:
		typedef std::unordered_map< long long, Ddo::ptr<Ddo::Module> > ModuleMap;

		void fetchAll(Wt::Dbo::Session &dboSession);
		virtual void load(Wt::Dbo::Session &dboSession) override;

		virtual bool isVital() const override { return true; }

		ModuleMap _moduleMap;
		boost::posix_time::time_duration _loadDuration;
	};
}

#endif
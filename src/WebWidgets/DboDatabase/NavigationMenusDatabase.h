#ifndef NAVIGATIONMENU_DATABASE_H
#define NAVIGATIONMENU_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/NavigationMenu.h"

class NavigationMenusDatabase : public AbstractDboDatabase
{
	public:
		NavigationMenusDatabase(DboDatabaseManager *Manager);
		virtual std::string Name() const { return "NavigationMenusDatabase"; }

	protected:
		void FetchAll(Wt::Dbo::Session &DboSession);
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }
}

#endif

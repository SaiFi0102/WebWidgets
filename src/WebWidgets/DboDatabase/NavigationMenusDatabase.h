#ifndef NAVIGATIONMENU_DATABASE_H
#define NAVIGATIONMENU_DATABASE_H

#include <unordered_map>
#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/NavigationMenu.h"

class PagesMenu;

class NavigationMenusDatabase : public AbstractDboDatabase
{
	protected:
		typedef std::unordered_map<long long, NavigationMenuItemData::ChildrenItemVector> NavigationMenuItemMaps;
		typedef std::list<std::shared_ptr<NavigationMenuItemData>> NavigationMenuItemLists;

	public:
		NavigationMenusDatabase(DboDatabaseManager *Manager);
		virtual std::string Name() const { return "NavigationMenusDatabase"; }

		void PopulateMenuItems(PagesMenu *menu) const;

	protected:
		void FetchAll(Wt::Dbo::Session &DboSession);
		void FetchChildren(Wt::Dbo::Session &DboSession, NavigationMenuItemData::ChildrenItemVector &destination, NavigationMenuItemLists &source, long long ParentId);
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		void PopulateChildrenItems(PagesMenu *menu, const NavigationMenuItemData::ChildrenItemVector &childrenVec) const;

		NavigationMenuItemMaps MenuItemMap;
		boost::posix_time::time_duration LoadDuration;
};

#endif

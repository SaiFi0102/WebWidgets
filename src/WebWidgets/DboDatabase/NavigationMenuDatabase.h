#ifndef WW_DBODATABASE_NAVIGATIONMENU_H
#define WW_DBODATABASE_NAVIGATIONMENU_H

#include <unordered_map>
#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class DboMenu;

	class NavigationMenuDatabase : public AbstractDboDatabase
	{
	private:
		typedef std::unordered_map<long long, Ddo::NavigationMenuItem::ChildrenItemVector> NavigationMenuItemMap;
		typedef std::list<Ddo::ptr<Ddo::NavigationMenuItem>> NavigationMenuItemList;
		typedef boost::tuple<long long, long long> MenuItemOnStyleTuple;
		typedef boost::tuple<std::string, long long> MenuItemOnLanguageTuple;
		typedef boost::tuple<std::string, long long> MenuItemOnAccessHostTuple;

		struct SourceData
		{
			NavigationMenuItemList itemList;
			std::list<Dbo::ptr<Dbo::MenuItemOnPage>> itemOnPageList;
			std::list<Dbo::ptr<Dbo::MenuItemOnPageAccessPath>> itemOnPageAccessPathList;
			std::list<MenuItemOnAccessHostTuple> itemOnAccessHostNameList;
			std::list<MenuItemOnLanguageTuple> itemOnLanguageList;
			std::list<MenuItemOnStyleTuple> itemOnStyleList;
		};

	public:
		NavigationMenuDatabase(DboDatabaseManager *manager);
		virtual std::string name() const override { return "NavigationMenusDatabase"; }

		void populateMenuItems(DboMenu *menu) const;

	protected:
		void fetchAll(Wt::Dbo::Session &dboSession);
		void fetchChildren(Wt::Dbo::Session &dboSession, Ddo::NavigationMenuItem::ChildrenItemVector &destination, SourceData &source, long long parentId);
		virtual void load(Wt::Dbo::Session &dboSession) override;
		virtual void reload(Wt::Dbo::Session &dboSession) override;

		virtual bool isVital() const override { return true; }

		void populateChildrenItems(DboMenu *menu, const Ddo::NavigationMenuItem::ChildrenItemVector &childrenVec) const;

		NavigationMenuItemMap _menuItemMap;
		boost::posix_time::time_duration _loadDuration;
	};
}

#endif

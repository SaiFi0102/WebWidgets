#include "DboDatabase/NavigationMenuDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Widgets/DboMenu.h"

namespace WW
{

NavigationMenuDatabase::NavigationMenuDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

void NavigationMenuDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	fetchAll(dboSession);
}

void NavigationMenuDatabase::reload(Wt::Dbo::Session &dboSession)
{
	fetchAll(dboSession);
}

void NavigationMenuDatabase::fetchChildren(Wt::Dbo::Session &dboSession, Ddo::NavigationMenuItem::ChildrenItemVector &destination, SourceData &source, long long parentId)
{
	for(auto &itemPtr : source.itemList)
	{
		if(itemPtr->parentItemId != parentId)
			continue;

		//Show on pages
		for(const auto &ptr : source.itemOnPageList)
		{
			if(ptr.id().menuItemPtr.id() == itemPtr->id)
				itemPtr->showOnPageIds[ptr.id().pagePtr.id()] = ptr->recursive;
		}

		//Show on page access paths 
		for(const auto &ptr : source.itemOnPageAccessPathList)
		{
			if(ptr.id().menuItemPtr.id() == itemPtr->id)
				itemPtr->showOnAccessPathIds[ptr.id().accessPathPtr.id()] = ptr->recursive;
		}

		//Show on access hostnames
		for(const auto &tuple : source.itemOnAccessHostNameList)
		{
			if(tuple.get<1>() == itemPtr->id)
				itemPtr->showOnAccessHostNames.insert(tuple.get<0>());
		}

		//Show on languages
		for(const auto &tuple : source.itemOnLanguageList)
		{
			if(tuple.get<1>() == itemPtr->id)
				itemPtr->showOnLanguageCodes.insert(tuple.get<0>());
		}

		//Show on styles
		for(const auto &tuple : source.itemOnStyleList)
		{
			if(tuple.get<1>() == itemPtr->id)
				itemPtr->showOnStyleIds.insert(tuple.get<0>());
		}

		fetchChildren(dboSession, itemPtr->childrenItems, source, itemPtr->id);
		destination.push_back(itemPtr);
	}

	std::sort(destination.begin(), destination.end(), [](const Ddo::ptr<Ddo::NavigationMenuItem> &left, const Ddo::ptr<Ddo::NavigationMenuItem> &right) {
		if(left->order < right->order)
			return true;
		else if(left->order = right->order)
			return left->id < right->id;
		else
			return false;
	});
}

void NavigationMenuDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	NavigationMenuItemMap menuitemmap;

	//Fetch
	Wt::Dbo::Transaction transaction(dboSession);

	//Menus
	Dbo::NavigationMenuCollection navigationMenuCollection = dboSession.find<Dbo::NavigationMenu>();
	for(const auto &menuPtr : navigationMenuCollection)
	{
		menuitemmap[menuPtr.id()];
	}

	SourceData source;

	//MenuItems
	Dbo::NavigationMenuItemCollection navigationMenuItemCollection = dboSession.find<Dbo::NavigationMenuItem>();
	for(const auto &ptr : navigationMenuItemCollection)
		source.itemList.push_back(Ddo::ptr<Ddo::NavigationMenuItem>(new Ddo::NavigationMenuItem(ptr)));

	//Show on pages
	Dbo::MenuItemOnPageCollection menuItemOnPageCollection = dboSession.find<Dbo::MenuItemOnPage>();
	for(const auto &ptr : menuItemOnPageCollection)
		source.itemOnPageList.push_back(ptr);

	//Show on page access paths 
	Dbo::MenuItemOnPageAccessPathCollection menuItemOnPageAccessPathCollection = dboSession.find<Dbo::MenuItemOnPageAccessPath>();
	for(const auto &ptr : menuItemOnPageAccessPathCollection)
		source.itemOnPageAccessPathList.push_back(ptr);

	//Show on access hostnames
	Wt::Dbo::collection<MenuItemOnAccessHostTuple> menuItemOnAccessHostCollection = dboSession.query<MenuItemOnAccessHostTuple>("SELECT accesshostname_hostName, navigationmenuitem_id FROM accesshost_show_menuitem");
	for(const auto &ptr : menuItemOnAccessHostCollection)
		source.itemOnAccessHostNameList.push_back(ptr);

	//Show on languages
	Wt::Dbo::collection<MenuItemOnLanguageTuple> menuItemOnLanguageCollection
		= dboSession.query<MenuItemOnLanguageTuple>("SELECT language_code, navigationmenuitem_id FROM language_show_menuitem");
	for(const auto &tuple : menuItemOnLanguageCollection)
		source.itemOnLanguageList.push_back(tuple);

	//Show on styles
	Wt::Dbo::collection<MenuItemOnStyleTuple> menuItemOnStyleCollection = 
		dboSession.query<MenuItemOnStyleTuple>("SELECT style_id, navigationmenuitem_id FROM style_show_menuitem");
	for(const auto &tuple : menuItemOnStyleCollection)
		source.itemOnStyleList.push_back(tuple);

	//FetchAll
	for(auto &res : menuitemmap)
		fetchChildren(dboSession, res.second, source, -1);

	//Apply
	transaction.commit();
	_menuItemMap.swap(menuitemmap);

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = ptEnd - ptStart;

	Wt::log("info") << name() << ": " << _menuItemMap.size() << " Menu entries and " << source.itemList.size()
		<< " Menu Item entries successfully loaded in " << _loadDuration.total_milliseconds() << " ms";
}

void NavigationMenuDatabase::populateChildrenItems(DboMenu *menu, const Ddo::NavigationMenuItem::ChildrenItemVector &childrenVec) const
{
	for(const Ddo::ptr<Ddo::NavigationMenuItem> &ptr : childrenVec)
	{
		DboMenuItem *item = new DboMenuItem(ptr);
		menu->addItem(item);

		if(!ptr->childrenItems.empty())
		{
			DboMenu *childMenu = new DboMenu(menu->menuId(), true);
			populateChildrenItems(childMenu, ptr->childrenItems);
			item->setMenu(childMenu);
		}
	}
}

void NavigationMenuDatabase::populateMenuItems(DboMenu *menu) const
{
	ReadLock lock(manager());

	NavigationMenuItemMap::const_iterator fitr = _menuItemMap.find(menu->menuId());
	if(fitr == _menuItemMap.end())
		return;

	populateChildrenItems(menu, fitr->second);
}

}

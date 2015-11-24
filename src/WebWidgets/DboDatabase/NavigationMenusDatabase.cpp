#include "DboDatabase/NavigationMenusDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Widgets/PagesMenu.h"

NavigationMenusDatabase::NavigationMenusDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager)
{ }

void NavigationMenusDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}

void NavigationMenusDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
}

void NavigationMenusDatabase::FetchChildren(Wt::Dbo::Session &DboSession, NavigationMenuItemData::ChildrenItemVector &destination, NavigationMenuItemLists &source, long long ParentId)
{
	for(const auto &Ptr : source)
	{
		if(Ptr->ParentItemId != ParentId)
			continue;

// 		//Show on pages
// 		MenuItemOnPageCollections MenuItemOnPageCollection = ItemPtr->MenuItemOnPageCollection;
// 		std::list<Wt::Dbo::ptr<MenuItemOnPage>> ResultList1(MenuItemOnPageCollection.begin(), MenuItemOnPageCollection.end());
// 		for(const auto &Ptr : ResultList1)
// 		{
// 			itemData->ShowOnPageIds[Ptr.id().PagePtr.id()] = Ptr->Recursive;
// 		}
// 
// 		//Show on page access paths 
// 		MenuItemOnPageAccessPathCollections MenuItemOnPageAccessPathCollection = ItemPtr->MenuItemOnPageAccessPathCollection;
// 		std::list<Wt::Dbo::ptr<MenuItemOnPageAccessPath>> ResultList2(MenuItemOnPageAccessPathCollection.begin(), MenuItemOnPageAccessPathCollection.end());
// 		for(const auto &Ptr : ResultList2)
// 		{
// 			itemData->ShowOnAccessPathIds[Ptr.id().AccessPathPtr.id()] = Ptr->Recursive;
// 		}
// 
// 		//Show on access hostnames
// 		AccessHostNameCollections AccessHostNameCollection = ItemPtr->ShowOnAccessHosts;
// 		std::list<Wt::Dbo::ptr<AccessHostName>> ResultList3(AccessHostNameCollection.begin(), AccessHostNameCollection.end());
// 		for(const auto &Ptr : ResultList3)
// 		{
// 			itemData->ShowOnAccessHostNames.insert(Ptr.id());
// 		}
// 
// 		//Show on languages
// 		LanguageCollections LanguageCollection = ItemPtr->ShowOnLanguages;
// 		std::list<Wt::Dbo::ptr<Language>> ResultList4(LanguageCollection.begin(), LanguageCollection.end());
// 		for(const auto &Ptr : ResultList4)
// 		{
// 			itemData->ShowOnLanguageCodes.insert(Ptr.id());
// 		}
// 
// 		//Show on page access paths
// 		StyleCollections StyleCollection = ItemPtr->ShowOnStyles;
// 		std::list<Wt::Dbo::ptr<Style>> ResultList5(StyleCollection.begin(), StyleCollection.end());
// 		for(const auto &Ptr : ResultList5)
// 		{
// 			itemData->ShowOnStyleIds.insert(Ptr.id());
// 		}

		FetchChildren(DboSession, Ptr->ChildrenItems, source, Ptr->Id);
		destination.push_back(Ptr);
	}

	std::sort(destination.begin(), destination.end(), [](const std::shared_ptr<NavigationMenuItemData> &Left, const std::shared_ptr<NavigationMenuItemData> &Right) {
		if(Left->Order < Right->Order)
			return true;
		else if(Left->Order = Right->Order)
			return Left->Id < Right->Id;
		else
			return false;
	});
}

void NavigationMenusDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	NavigationMenuItemMaps menuitemmap;

	//Fetch
	Wt::Dbo::Transaction transaction(DboSession);

	//Menus
	NavigationMenuCollections NavigationMenuCollection = DboSession.find<NavigationMenu>();
	for(const auto &MenuPtr : NavigationMenuCollection)
	{
		menuitemmap[MenuPtr.id()];
	}

	//MenuItems
	NavigationMenuItemCollections NavigationMenuItemCollection = DboSession.find<NavigationMenuItem>();
	NavigationMenuItemLists ItemList;
	for(const auto &Ptr : NavigationMenuItemCollection)
	{
		ItemList.push_back(std::shared_ptr<NavigationMenuItemData>(new NavigationMenuItemData(Ptr)));
	}

// 	//Show on pages
// 	MenuItemOnPageCollections MenuItemOnPageCollection = DboSession.find<MenuItemOnPage>();
// 	std::list<Wt::Dbo::ptr<MenuItemOnPage>> ItemOnPageList(MenuItemOnPageCollection.begin(), MenuItemOnPageCollection.end());
// 
// 	//Show on page access paths 
// 	MenuItemOnPageAccessPathCollections MenuItemOnPageAccessPathCollection = DboSession.find<MenuItemOnPageAccessPath>();
// 	std::list<Wt::Dbo::ptr<MenuItemOnPageAccessPath>> ItemOnPageAccessPathList(MenuItemOnPageAccessPathCollection.begin(), MenuItemOnPageAccessPathCollection.end());
// 
// 	//Show on access hostnames
// 	AccessHostNameCollections AccessHostNameCollection = DboSession.find<AccessHostName>();
// 	std::list<Wt::Dbo::ptr<AccessHostName>> ItemOnAccessHostNameList(AccessHostNameCollection.begin(), AccessHostNameCollection.end());
// 
// 	//Show on languages
// 	LanguageCollections LanguageCollection = DboSession.find<Language>();
// 	std::list<Wt::Dbo::ptr<Language>> ItemOnLanguageList(LanguageCollection.begin(), LanguageCollection.end());
// 
// 	//Show on page access paths
// 	StyleCollections StyleCollection = DboSession.find<Style>();
// 	std::list<Wt::Dbo::ptr<Style>> ItemOnStyleList(StyleCollection.begin(), StyleCollection.end());

	for(auto &Res : menuitemmap)
	{
		FetchChildren(DboSession, Res.second, ItemList, -1);
	}

	//Apply
	transaction.commit();
	MenuItemMap.swap(menuitemmap);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << MenuItemMap.size() << " Menu entries and " << ItemList.size()
		<< " Menu Item entries successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
}

void NavigationMenusDatabase::PopulateChildrenItems(PagesMenu *menu, const NavigationMenuItemData::ChildrenItemVector &childrenVec) const
{
	for(NavigationMenuItemData::ChildrenItemVector::const_iterator itr = childrenVec.begin();
		itr != childrenVec.end();
		++itr)
	{
		PagesMenuItem *item = new PagesMenuItem(*itr);
		menu->addItem(item);

		if(!(*itr)->ChildrenItems.empty())
		{
			PagesMenu *childMenu = new PagesMenu(menu->MenuId(), true);
			PopulateChildrenItems(childMenu, (*itr)->ChildrenItems);
			item->setMenu(childMenu);
		}

// 		//Access hostnames rule
// 		if((*itr)->BlacklistBits & NavigationMenuItem::BlacklistAccessHosts)
// 		{
// 			if((*itr)->ShowOnAccessHostNames.find(AccessHost) != (*itr)->ShowOnAccessHostNames.end())
// 				continue;
// 		}
// 		else
// 		{
// 			if((*itr)->ShowOnAccessHostNames.find(AccessHost) == (*itr)->ShowOnAccessHostNames.end())
// 				continue;
// 		}
// 
// 		//Page Access path rule
// 		if((*itr)->BlacklistBits & NavigationMenuItem::BlacklistAccessPaths)
// 		{
// 			if((*itr)->ShowOnAccessPathIds.find(PageAccessPathId) != (*itr)->ShowOnAccessPathIds.end())
// 				continue;
// 		}
// 		else
// 		{
// 			if((*itr)->ShowOnAccessHostNames.find(AccessHost) == (*itr)->ShowOnAccessHostNames.end())
// 				continue;
// 		}
	}
}

void NavigationMenusDatabase::PopulateMenuItems(PagesMenu *menu) const
{
	ReadLock lock(Manager());

	NavigationMenuItemMaps::const_iterator fitr = MenuItemMap.find(menu->MenuId());
	if(fitr == MenuItemMap.end())
		return;

	PopulateChildrenItems(menu, fitr->second);
}

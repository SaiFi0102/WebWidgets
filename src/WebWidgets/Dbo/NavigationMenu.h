#ifndef NAVIGATIONMENU_DBO_H
#define NAVIGATIONMENU_DBO_H

#include "Dbo/DboTraits.h"

class NavigationMenu
{
	public:
		NavigationMenuItemCollections MenuItemsCollection;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::hasMany(a, MenuItemsCollection, Wt::Dbo::ManyToOne, "NavigationMenu");
		}

		static const char *TableName()
		{
			return "navigationmenu";
		}
};

class NavigationMenuItem
{
	public:
		enum AnchorTarget
		{
			TargetSelf = 0,
			TargetThisWindow = 1,
			TargetNewWindow = 2
		};
		enum BlacklistBitField
		{
			BlacklistAccessHosts	= 0x01,
			BlacklistAccessPaths	= 0x02,
			BlacklistPages			= 0x04,
			BlacklistLanguages		= 0x08,
			BlacklistStyles			= 0x10
		};

		Wt::Dbo::ptr<NavigationMenu> MenuPtr;
		int Order;
		Wt::Dbo::ptr<SingularKey> LabelKey;
		Wt::Dbo::ptr<PageAccessPath> AccessPathPtr;
		std::string WildcardData;
		AnchorTarget Target;
		bool Preload;

		NavigationMenuItemCollections ChildrenItems;
		Wt::Dbo::ptr<NavigationMenuItem> ParentItem;

		MenuItemOnPageCollections MenuItemOnPageCollection;
		MenuItemOnPageAccessPathCollections MenuItemOnPageAccessPathCollection;

		AccessHostNameCollections ShowOnAccessHosts;
		LanguageCollections ShowOnLanguages;
		StyleCollections ShowOnStyles;
		short BlacklistBits;

		NavigationMenuItem()
			: Order(0), BlacklistBits(0), Target(TargetSelf), Preload(false)
		{ }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, MenuPtr, "NavigationMenu", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Order, "Order");
			Wt::Dbo::belongsTo(a, LabelKey, "LabelKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, AccessPathPtr, "AccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, WildcardData, "WildcardData");
			Wt::Dbo::field(a, Target, "Target");
			Wt::Dbo::field(a, Preload, "Preload");
			Wt::Dbo::field(a, BlacklistBits, "BlacklistBits");

			Wt::Dbo::hasMany(a, ChildrenItems, Wt::Dbo::ManyToOne, "Parent_Item");
			Wt::Dbo::belongsTo(a, ParentItem, "Parent_Item", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);

			Wt::Dbo::hasMany(a, MenuItemOnPageCollection, Wt::Dbo::ManyToOne, "NavigationMenuItem");
			Wt::Dbo::hasMany(a, MenuItemOnPageAccessPathCollection, Wt::Dbo::ManyToOne, "NavigationMenuItem");
 			
			Wt::Dbo::hasMany(a, ShowOnAccessHosts, Wt::Dbo::ManyToMany, "accesshost_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
 			Wt::Dbo::hasMany(a, ShowOnLanguages, Wt::Dbo::ManyToMany, "language_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
 			Wt::Dbo::hasMany(a, ShowOnStyles, Wt::Dbo::ManyToMany, "style_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		static const char *TableName()
		{
			return "navigationmenuitem";
		}
};

struct MenuItemOnPageKey
{
	Wt::Dbo::ptr<NavigationMenuItem> MenuItemPtr;
	Wt::Dbo::ptr<Page> PagePtr;

	bool operator==(const MenuItemOnPageKey &other) const
	{
		return MenuItemPtr == other.MenuItemPtr && PagePtr == other.PagePtr;
	}

	bool operator<(const MenuItemOnPageKey &other) const
	{
		if(MenuItemPtr < other.MenuItemPtr)
			return true;
		else if(MenuItemPtr == other.MenuItemPtr)
			return PagePtr < other.PagePtr;
		else
			return false;
	}
};
class MenuItemOnPage
{
	public:
		bool Recursive;

		MenuItemOnPage()
			: Recursive(false)
		{ }
		MenuItemOnPage(Wt::Dbo::ptr<NavigationMenuItem> MenuItemPtr, Wt::Dbo::ptr<Page> PagePtr)
			: Recursive(false)
		{
			_id.MenuItemPtr = MenuItemPtr;
			_id.PagePtr = PagePtr;
		}

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _id, "Id");
			Wt::Dbo::field(a, Recursive, "Recursive");
		}
		static const char *TableName()
		{
			return "page_show_menuitem";
		}

	private:
		MenuItemOnPageKey _id;
};
std::ostream &operator<<(std::ostream &o, const MenuItemOnPageKey &c);

struct MenuItemOnPageAccessPathKey
{
	Wt::Dbo::ptr<NavigationMenuItem> MenuItemPtr;
	Wt::Dbo::ptr<PageAccessPath> AccessPathPtr;

	bool operator==(const MenuItemOnPageAccessPathKey &other) const
	{
		return MenuItemPtr == other.MenuItemPtr && AccessPathPtr == other.AccessPathPtr;
	}

	bool operator<(const MenuItemOnPageAccessPathKey &other) const
	{
		if(MenuItemPtr < other.MenuItemPtr)
			return true;
		else if(MenuItemPtr == other.MenuItemPtr)
			return AccessPathPtr < other.AccessPathPtr;
		else
			return false;
	}
};
class MenuItemOnPageAccessPath
{
	public:
		bool Recursive;

		MenuItemOnPageAccessPath()
			: Recursive(false)
		{ }
		MenuItemOnPageAccessPath(Wt::Dbo::ptr<NavigationMenuItem> MenuItemPtr, Wt::Dbo::ptr<PageAccessPath> AccessPathPtr)
			: Recursive(false)
		{
			_id.MenuItemPtr = MenuItemPtr;
			_id.AccessPathPtr = AccessPathPtr;
		}

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _id, "Id");
			Wt::Dbo::field(a, Recursive, "Recursive");
		}
		static const char *TableName()
		{
			return "pageaccesspath_show_menuitem";
		}

	private:
		MenuItemOnPageAccessPathKey _id;
};
std::ostream &operator<<(std::ostream &o, const MenuItemOnPageAccessPathKey &c);

class NavigationMenuItemData
{
	public:
		typedef std::vector<std::shared_ptr<NavigationMenuItemData>> ChildrenItemVector;

		long long Id;
		long long MenuId;
		int Order;
		std::string LabelKey;
		long long LabelModuleId;
		long long AccessPathId;
		std::string WildcardData;
		NavigationMenuItem::AnchorTarget Target;
		bool Preload;
		long long ParentItemId;
		short BlacklistBits;
		std::map<long long, bool> ShowOnPageIds;
		std::map<long long, bool> ShowOnAccessPathIds;
		std::set<std::string> ShowOnAccessHostNames;
		std::set<std::string> ShowOnLanguageCodes;
		std::set<long long> ShowOnStyleIds;
		ChildrenItemVector ChildrenItems;

		NavigationMenuItemData(const Wt::Dbo::ptr<NavigationMenuItem> &Ptr);
};

#include "Dbo/Page.h"
#include "Dbo/AccessPath.h"
#include "Dbo/Language.h"
#include "Dbo/Style.h"

#endif
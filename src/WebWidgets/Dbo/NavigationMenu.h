#ifndef WW_DBO_NAVIGATIONMENU_H
#define WW_DBO_NAVIGATIONMENU_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{

		class NavigationMenu
		{
		public:
			NavigationMenuItemCollection menuItemsCollection;

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::hasMany(a, menuItemsCollection, Wt::Dbo::ManyToOne, "navigationMenu");
			}

			constexpr static const char *tableName()
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
			enum WhitelistFlags
			{
				WhitelistAccessHosts = 0x01,
				WhitelistAccessPaths = 0x02,
				WhitelistPages = 0x04,
				WhitelistLanguages = 0x08,
				WhitelistStyles = 0x10
			};
			enum LoggedInVisibility
			{
				LoggedInNoRestriction = 0,
				LoggedInOnly = 1,
				LoggedOutOnly = 2
			};

			NavigationMenuItem() = default;
			NavigationMenuItem(ptr<NavigationMenu> menuPtr)
				: menuPtr(menuPtr)
			{ }

			ptr<NavigationMenu> menuPtr;
			int order = 0;
			ptr<SingularKey> labelKey;
			ptr<PageAccessPath> accessPathPtr;
			std::string wildcardData;
			AnchorTarget target = TargetSelf;
			bool preload = false;

			NavigationMenuItemCollection childrenItems;
			ptr<NavigationMenuItem> parentItem;

			MenuItemOnPageCollection menuItemOnPageCollection;
			MenuItemOnPageAccessPathCollection menuItemOnPageAccessPathCollection;

			AccessHostNameCollection showOnAccessHosts;
			LanguageCollection showOnLanguages;
			StyleCollection showOnStyles;

			short whitelistFlags = 0x00;
			LoggedInVisibility loggedInVisibility = LoggedInNoRestriction;

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, menuPtr, "navigationMenu", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, order, "order");
				Wt::Dbo::belongsTo(a, labelKey, "labelKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::belongsTo(a, accessPathPtr, "accessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::field(a, wildcardData, "wildcardData");
				Wt::Dbo::field(a, target, "target");
				Wt::Dbo::field(a, preload, "preload");
				Wt::Dbo::field(a, whitelistFlags, "whitelistFlags");
				Wt::Dbo::field(a, loggedInVisibility, "loggedInVisibility");

				Wt::Dbo::hasMany(a, childrenItems, Wt::Dbo::ManyToOne, "parent_item");
				Wt::Dbo::belongsTo(a, parentItem, "parent_item", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);

				Wt::Dbo::hasMany(a, menuItemOnPageCollection, Wt::Dbo::ManyToOne, "navigationMenuItem");
				Wt::Dbo::hasMany(a, menuItemOnPageAccessPathCollection, Wt::Dbo::ManyToOne, "navigationMenuItem");

				Wt::Dbo::hasMany(a, showOnAccessHosts, Wt::Dbo::ManyToMany, "accesshost_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::hasMany(a, showOnLanguages, Wt::Dbo::ManyToMany, "language_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::hasMany(a, showOnStyles, Wt::Dbo::ManyToMany, "style_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "navigationmenuitem";
			}
		};

		struct MenuItemOnPageKey
		{
			ptr<NavigationMenuItem> menuItemPtr;
			ptr<Page> pagePtr;

			bool operator==(const MenuItemOnPageKey &other) const
			{
				return menuItemPtr == other.menuItemPtr && pagePtr == other.pagePtr;
			}

			bool operator<(const MenuItemOnPageKey &other) const
			{
				if(menuItemPtr < other.menuItemPtr)
					return true;
				else if(menuItemPtr == other.menuItemPtr)
					return pagePtr < other.pagePtr;
				else
					return false;
			}
		};
		class MenuItemOnPage
		{
		public:
			bool recursive = false;

			MenuItemOnPage() = default;
			MenuItemOnPage(ptr<NavigationMenuItem> menuItemPtr, ptr<Page> pagePtr)
			{
				_id.menuItemPtr = menuItemPtr;
				_id.pagePtr = pagePtr;
			}

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _id, "id");
				Wt::Dbo::field(a, recursive, "recursive");
			}
			constexpr static const char *tableName()
			{
				return "page_show_menuitem";
			}

		private:
			MenuItemOnPageKey _id;
		};

		struct MenuItemOnPageAccessPathKey
		{
			ptr<NavigationMenuItem> menuItemPtr;
			ptr<PageAccessPath> accessPathPtr;

			bool operator==(const MenuItemOnPageAccessPathKey &other) const
			{
				return menuItemPtr == other.menuItemPtr && accessPathPtr == other.accessPathPtr;
			}

			bool operator<(const MenuItemOnPageAccessPathKey &other) const
			{
				if(menuItemPtr < other.menuItemPtr)
					return true;
				else if(menuItemPtr == other.menuItemPtr)
					return accessPathPtr < other.accessPathPtr;
				else
					return false;
			}
		};
		class MenuItemOnPageAccessPath
		{
		public:
			bool recursive = false;

			MenuItemOnPageAccessPath() = default;
			MenuItemOnPageAccessPath(ptr<NavigationMenuItem> menuItemPtr, ptr<PageAccessPath> accessPathPtr)
			{
				_id.menuItemPtr = menuItemPtr;
				_id.accessPathPtr = accessPathPtr;
			}

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _id, "id");
				Wt::Dbo::field(a, recursive, "recursive");
			}
			constexpr static const char *tableName()
			{
				return "pageaccesspath_show_menuitem";
			}

		private:
			MenuItemOnPageAccessPathKey _id;
		};

		std::ostream &operator<<(std::ostream &o, const WW::Dbo::MenuItemOnPageAccessPathKey &c);
		std::ostream &operator<<(std::ostream &o, const WW::Dbo::MenuItemOnPageKey &c);

	}

	namespace Ddo
	{

		class NavigationMenuItem
		{
		public:
			typedef std::vector<Ddo::ptr<Ddo::NavigationMenuItem>> ChildrenItemVector;

			long long id;
			long long menuId;
			int order;
			std::string labelKey;
			long long labelModuleId;
			long long accessPathId;
			std::string wildcardData;
			Dbo::NavigationMenuItem::AnchorTarget target;
			bool preload;
			long long parentItemId;
			short whitelistFlags;
			Dbo::NavigationMenuItem::LoggedInVisibility loggedInVisibility;

			std::map<long long, bool> showOnPageIds;
			std::map<long long, bool> showOnAccessPathIds;
			std::set<std::string> showOnAccessHostNames;
			std::set<std::string> showOnLanguageCodes;
			std::set<long long> showOnStyleIds;
			ChildrenItemVector childrenItems;

			NavigationMenuItem(const Dbo::ptr<Dbo::NavigationMenuItem> &ptr)
				: id(ptr.id()),
				menuId(ptr->menuPtr.id()),
				order(ptr->order),
				labelKey(ptr->labelKey->key()),
				labelModuleId(ptr->labelKey->modulePtr().id()),
				accessPathId(ptr->accessPathPtr.id()),
				wildcardData(ptr->wildcardData),
				parentItemId(ptr->parentItem.id()),
				target(ptr->target),
				preload(ptr->preload),
				whitelistFlags(ptr->whitelistFlags),
				loggedInVisibility(ptr->loggedInVisibility)
			{ }
		};
	
	}
}

#endif

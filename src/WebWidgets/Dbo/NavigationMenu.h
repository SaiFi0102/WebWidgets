#ifndef NAVIGATIONMENU_DBO_H
#define NAVIGATIONMENU_DBO_H

#include "Dbo/DboTraits.h"

class NavigationMenu
{
	public:
		NavigationMenuCollections MenuItemsCollection;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::hasMany(a, MenuItemsCollection, Wt::Dbo::ManyToOne, "NavigationMenu");
		}

		static const char *TableName()
		{
			return "navigationmenus";
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

		Wt::Dbo::ptr<NavigationMenu> MenuPtr;
		int Order;
		Wt::Dbo::ptr<SingularKey> LabelKey;
		Wt::Dbo::ptr<PageAccessPath> AccessPathPtr;
		std::string WildcardData;
		AnchorTarget Target;

		NavigationMenuItemCollections ChildrenItems;
		Wt::Dbo::ptr<NavigationMenuItem> ParentItem;

		Wt::Dbo::ptr<Page> ShowOnPage;
		bool ShowOnChildrenPages;
		Wt::Dbo::ptr<PageAccessPath> ShowOnAccessPath;
		Wt::Dbo::ptr<AccessHostName> ShowOnAccessHost;
		Wt::Dbo::ptr<Language> ShowOnLanguage;
		Wt::Dbo::ptr<Style> ShowOnStyle;

		NavigationMenuItem()
			: Order(0), Target(TargetSelf), ShowOnChildrenPages(true)
		{ }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, MenuPtr, "NavigationMenu", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Order, "Order");
			Wt::Dbo::belongsTo(a, LabelKey, "LabelKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, AccessPathPtr, "AccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, WildcardData, "WildcardData");
			Wt::Dbo::field(a, Target, "Target");

			Wt::Dbo::hasMany(a, ChildrenItems, Wt::Dbo::ManyToOne, "Parent_Item");
			Wt::Dbo::belongsTo(a, ParentItem, "Parent_Item", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);

			Wt::Dbo::belongsTo(a, ShowOnPage, "ShowOnPage", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, ShowOnChildrenPages, "ShowOnChildrenPages");
			Wt::Dbo::belongsTo(a, ShowOnAccessPath, "ShowOnAccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, ShowOnAccessHost, "ShowOnAccessHost", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, ShowOnLanguage, "ShowOnLanguage", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, ShowOnStyle, "ShowOnStyle", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "navigationmenuitems";
		}
};
class NavigationMenuItemData
{
	public:
		long long MenuId;
		int Order;
		std::string LabelKey;
		long long LabelModuleId;
		long long AccessPathId;
		std::string WildcardData;
		NavigationMenuItem::AnchorTarget Target;
		long long ParentItemId;
		long long ShowOnPageId;
		bool ShowOnChildrenPages;
		long long ShowOnAccessPathId;
		std::string ShowOnAccessHostName;
		std::string ShowOnLanguageCode;
		long long ShowOnStyleId;

		NavigationMenuItemData(Wt::Dbo::ptr<NavigationMenuItem> Ptr);
};

#endif
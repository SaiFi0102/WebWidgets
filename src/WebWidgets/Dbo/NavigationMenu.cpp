#include "Dbo/NavigationMenu.h"

NavigationMenuItemData::NavigationMenuItemData(const Wt::Dbo::ptr<NavigationMenuItem> &Ptr)
	: Id(Ptr.id()),
	MenuId(Ptr->MenuPtr.id()),
	Order(Ptr->Order),
	LabelKey(Ptr->LabelKey->Key()),
	LabelModuleId(Ptr->LabelKey->ModulePtr().id()),
	AccessPathId(Ptr->AccessPathPtr.id()),
	WildcardData(Ptr->WildcardData),
	ParentItemId(Ptr->ParentItem.id()),
	Target(Ptr->Target),
	Preload(Ptr->Preload),
	BlacklistBits(Ptr->BlacklistBits)
{ }

Wt::Dbo::dbo_traits<MenuItemOnPage>::IdType Wt::Dbo::dbo_traits<MenuItemOnPage>::invalidId()
{
	return IdType();
}

Wt::Dbo::dbo_traits<MenuItemOnPageAccessPath>::IdType Wt::Dbo::dbo_traits<MenuItemOnPageAccessPath>::invalidId()
{
	return IdType();
}

std::ostream & operator<<(std::ostream &o, const MenuItemOnPageKey &c)
{
	return o << "(" << c.MenuItemPtr << ", " << c.PagePtr << ")";
}

std::ostream & operator<<(std::ostream &o, const MenuItemOnPageAccessPathKey &c)
{
	return o << "(" << c.MenuItemPtr << ", " << c.AccessPathPtr << ")";
}

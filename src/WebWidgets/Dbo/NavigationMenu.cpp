#include "Dbo/NavigationMenu.h"
#include "Dbo/Language.h"

NavigationMenuItemData::NavigationMenuItemData(Wt::Dbo::ptr<NavigationMenuItem> Ptr)
	: MenuId(Ptr->MenuPtr.id()),
	Order(Ptr->Order),
	LabelKey(Ptr->LabelKey->Key()),
	LabelModuleId(Ptr->LabelKey->ModulePtr().id()),
	AccessPathId(Ptr->AccessPathPtr.id()),
	WildcardData(Ptr->WildcardData),
	ParentItemId(Ptr->ParentItem.id()),
	Target(Ptr->Target),
	ShowOnPageId(Ptr->ShowOnPage.id()),
	ShowOnChildrenPages(Ptr->ShowOnChildrenPages),
	ShowOnAccessPathId(Ptr->ShowOnAccessPath.id()),
	ShowOnAccessHostName(Ptr->ShowOnAccessHost.id()),
	ShowOnLanguageCode(Ptr->ShowOnLanguage.id()),
	ShowOnStyleId(Ptr->ShowOnStyle.id())
{ }

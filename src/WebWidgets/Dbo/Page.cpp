#include "Page.h"

PageData::PageData(Wt::Dbo::ptr<Page> Ptr)
	: BasePage(*Ptr),
	DataSurrogateKey(Ptr.id()),
	_ModuleId(Ptr->ModulePtr().id()),
	ParentPageId(Ptr->ParentPage.id()),
	TitleKey(Ptr->TitleKey->Key()),
	TitleModuleId(Ptr->TitleKey->ModulePtr().id())
{ }

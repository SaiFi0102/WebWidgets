#include "Dbo/AccessPath.h"

AccessPathData::AccessPathData(Wt::Dbo::ptr<AccessPath> Ptr)
	: DataSurrogateKey(Ptr.id()), BaseAccessPath(*Ptr),
	LanguageCode(Ptr->LanguagePtr.id())
{
	if(Ptr->PagePtr)
	{
		PageId = Ptr->PagePtr.id();
	}
	else
	{
		PageId = -1;
	}
}
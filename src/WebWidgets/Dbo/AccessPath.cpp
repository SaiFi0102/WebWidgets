#include "AccessPath.h"

PageAccessPathData::PageAccessPathData(Wt::Dbo::ptr<PageAccessPath> Ptr)
	: DataSurrogateKey(Ptr.id()),
	HostName(Ptr->AccessHostNamePtr().id()),
	InternalPath(Ptr->InternalPath()),
	PageId(Ptr->PagePtr.id()),
	ParentAccessPathId(Ptr->ParentAccessPathPtr().id())
{
	FullPath = InternalPath;
	Wt::Dbo::ptr<PageAccessPath> ParentAccessPathPtr = Ptr->ParentAccessPathPtr();
	while(ParentAccessPathPtr)
	{
		FullPath.insert(0, ParentAccessPathPtr->InternalPath() + "/");
		ParentAccessPathPtr = ParentAccessPathPtr->ParentAccessPathPtr();
	}
}

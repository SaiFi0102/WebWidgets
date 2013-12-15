#include "Dbo/Page.h"

PageKeys::PageKeys()
	: id(-1)
{ }
PageKeys::PageKeys(long long id, Wt::Dbo::ptr<Module> ModulePtr)
	: id(id), ModulePtr(ModulePtr)
{ }
bool PageKeys::operator== (const PageKeys &other) const
{
	return id == other.id && ModulePtr == other.ModulePtr;
}
bool PageKeys::operator< (const PageKeys &other) const
{
	if(id < other.id)
	{
		return true;
	}
	else if(id == other.id)
	{
		return ModulePtr < other.ModulePtr;
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const PageKeys &c)
{
	return o << "(" << c.id << ", " << c.ModulePtr << ")";
}

Wt::Dbo::dbo_traits<Page>::IdType Wt::Dbo::dbo_traits<Page>::invalidId()
{
	return Wt::Dbo::dbo_traits<Page>::IdType();
}
const char* Wt::Dbo::dbo_traits<Page>::surrogateIdField()
{
	return 0;
}

Page::Page()
{ }
Page::Page(long long id, Wt::Dbo::ptr<Module> ModulePtr)
	: _Id(id, ModulePtr)
{ }
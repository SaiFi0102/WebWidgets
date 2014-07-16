#include "Dbo/Page.h"

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
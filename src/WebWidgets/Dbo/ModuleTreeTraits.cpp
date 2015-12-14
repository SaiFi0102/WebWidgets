#include "Dbo/ModuleTreeDbos.h"

Wt::Dbo::dbo_traits<WW::Dbo::MenuItemOnPage>::IdType Wt::Dbo::dbo_traits<WW::Dbo::MenuItemOnPage>::invalidId()
{
	return IdType();
}

Wt::Dbo::dbo_traits<WW::Dbo::MenuItemOnPageAccessPath>::IdType Wt::Dbo::dbo_traits<WW::Dbo::MenuItemOnPageAccessPath>::invalidId()
{
	return IdType();
}

namespace WW
{
	namespace Dbo
	{
		std::ostream &operator<<(std::ostream &o, const WW::Dbo::MenuItemOnPageAccessPathKey &c)
		{
			return o << "(" << c.menuItemPtr << ", " << c.accessPathPtr << ")";
		}
		std::ostream &operator<<(std::ostream &o, const WW::Dbo::MenuItemOnPageKey &c)
		{
			return o << "(" << c.menuItemPtr << ", " << c.pagePtr << ")";
		}
	}
}
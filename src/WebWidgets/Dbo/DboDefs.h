#ifndef WW_DBO_DEFS_H
#define WW_DBO_DEFS_H

#include <Wt/Dbo/Dbo>

namespace WW
{
	namespace Dbo
	{
		template<class C>
		using ptr = Wt::Dbo::ptr<C>;
	}

	namespace Ddo
	{
		template<class C>
		using ptr = std::shared_ptr<C>;

		template<class C>
		using cPtr = std::shared_ptr<const C>;
	}
}

#endif
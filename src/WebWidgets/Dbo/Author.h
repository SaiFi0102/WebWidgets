#ifndef WW_DBO_AUTHOR_H
#define WW_DBO_AUTHOR_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{
		class Author
		{
		public:
			std::string name;
			std::string email;
			std::string website;

			ModuleCollection moduleCollection;
			StyleCollection styleCollection;

			Author() = default;
			Author(long long id)
				: _id(id)
			{ }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _id);
				Wt::Dbo::field(a, name, "name", 255);
				Wt::Dbo::field(a, email, "email", 255);
				Wt::Dbo::field(a, website, "website", 255);

				Wt::Dbo::hasMany(a, moduleCollection, Wt::Dbo::ManyToOne, "author");
				Wt::Dbo::hasMany(a, styleCollection, Wt::Dbo::ManyToOne, "author");
			}
			constexpr static const char *tableName()
			{
				return "author";
			}
		private:
			long long _id = -1;
		};
	}
}

#endif

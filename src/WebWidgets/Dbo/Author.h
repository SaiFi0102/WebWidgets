#ifndef AUTHOR_DBO_H
#define AUTHOR_DBO_H

#include "DboTraits.h"
#include "Dbo/Module.h"

class Author
{
	public:
		std::string Name;
		std::string Email;
		std::string Website;

		ModuleCollections ModuleCollection;
		StyleCollections StyleCollection;

		Author()
			: _id(-1)
		{};

		Author(long long id)
			: _id(id)
		{};

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _id);
			Wt::Dbo::field(a, Name, "Name", 255);
			Wt::Dbo::field(a, Email, "Email", 255);
			Wt::Dbo::field(a, Website, "Website", 255);

			Wt::Dbo::hasMany(a, ModuleCollection, Wt::Dbo::ManyToOne, "Author");
			Wt::Dbo::hasMany(a, StyleCollection, Wt::Dbo::ManyToOne, "Author");
		}
		static const char *TableName()
		{
			return "author";
		}
	private:
		long long _id;
};

#include "Dbo/Style.h"

#endif
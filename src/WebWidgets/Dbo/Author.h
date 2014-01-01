#ifndef AUTHOR_DBO_H
#define AUTHOR_DBO_H

#include "DboTraits.h"
#include "Dbo/Module.h"

class Author : public Wt::Dbo::Dbo<Author>
{
	public:
		std::string Name;
		boost::optional<std::string> Email;
		boost::optional<std::string> Website;

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
			Wt::Dbo::field(a, Name, "Name", 256);
			Wt::Dbo::field(a, Email, "Email", 256);
			Wt::Dbo::field(a, Website, "Website", 256);

			Wt::Dbo::hasMany(a, ModuleCollection, Wt::Dbo::ManyToOne, "Author");
			Wt::Dbo::hasMany(a, StyleCollection, Wt::Dbo::ManyToOne, "Author");
		}
		static const char *TableName()
		{
			return "authors";
		}
	private:
		long long _id;
};

#include "Dbo/Style.h"

#endif
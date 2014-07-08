#ifndef PAGE_DBO_H
#define PAGE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"

struct PageKeys
{
	long long id;
	Wt::Dbo::ptr<Module> ModulePtr;

	PageKeys();
	PageKeys(long long id, Wt::Dbo::ptr<Module> ModulePtr);

	bool operator< (const PageKeys &other) const;
	bool operator== (const PageKeys &other) const;
};
std::ostream &operator<< (std::ostream &o, const PageKeys &c);

class Page : public Wt::Dbo::Dbo<Page>
{
	public:
		std::string	DefaultInternalPath;
		std::string	Title;

		Wt::Dbo::ptr<AccessPath> AccessPathPtr;

		//PageCollections ChildrenPages;
		//Wt::Dbo::ptr<Page> ParentPage;

		Page();
		Page(long long id, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>());

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Page");
			Wt::Dbo::field(a, DefaultInternalPath, "DefaultInternalPath", 50);
			Wt::Dbo::field(a, Title, "Title");

			Wt::Dbo::belongsTo(a, AccessPathPtr, "accesspath", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);

			//Wt::Dbo::hasMany(a, ChildrenPages, Wt::Dbo::ManyToOne, "Parent_Page");
			//Wt::Dbo::belongsTo(a, ParentPage, "Parent_Page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "pages";
		}

	private:
		PageKeys _Id;
		friend class PagesDatabase;
};

#include "Dbo/AccessPath.h"

#endif
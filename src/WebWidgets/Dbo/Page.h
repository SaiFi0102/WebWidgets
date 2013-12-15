#ifndef PAGE_DBO_H
#define PAGE_DBO_H

#include <Wt/Dbo/Dbo>
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

namespace Wt
{
	namespace Dbo
	{
		template<class Action>
		void field(Action &action, PageKeys &Keys, const std::string &name, int size = -1)
		{
			field(action, Keys.id, "id");
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<Page> : public dbo_default_traits
		{
			typedef PageKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
}

class Page : public Wt::Dbo::Dbo<Page>
{
	public:
		std::string	InternalPath;
		std::string	DefaultInternalPath;
		std::string	Title;

		//PageCollections ChildrenPages;
		//Wt::Dbo::ptr<Page> ParentPage;

		Page();
		Page(long long id, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>());

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Page");
			Wt::Dbo::field(a, InternalPath, "InternalPath", 50);
			Wt::Dbo::field(a, DefaultInternalPath, "DefaultInternalPath", 50);
			Wt::Dbo::field(a, Title, "Title");

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

#endif
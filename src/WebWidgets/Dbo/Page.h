#ifndef PAGE_DBO_H
#define PAGE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"
#include "Dbo/NavigationMenu.h"

class BasePage
{
	protected:
		BasePage(const std::string &Name)
			: _Name(Name)
		{ }

		std::string _Name;

	public:
		std::string Name() const { return _Name; }
		std::string	DefaultInternalPath;
};
class Page : public BasePage
{
	protected:
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		PageAccessPathCollections PageAccessPathCollection;
		AccessHostNameCollections AccessHostNameCollection;
		MenuItemOnPageCollections MenuItemOnPageCollection;

		Wt::Dbo::ptr<SingularKey> TitleKey;

		PageCollections ChildrenPages;
		Wt::Dbo::ptr<Page> ParentPage;

		Page()
			: BasePage("")
		{ }
		Page(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr)
			: BasePage(Name), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _Name, "Name", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, DefaultInternalPath, "DefaultInternalPath", 50);
			Wt::Dbo::belongsTo(a, TitleKey, "TitleKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);

			Wt::Dbo::hasMany(a, PageAccessPathCollection, Wt::Dbo::ManyToOne, "Page");
			Wt::Dbo::hasMany(a, AccessHostNameCollection, Wt::Dbo::ManyToOne, "DefaultPage");
			Wt::Dbo::hasMany(a, MenuItemOnPageCollection, Wt::Dbo::ManyToOne, "Page");

			Wt::Dbo::hasMany(a, ChildrenPages, Wt::Dbo::ManyToOne, "Parent_Page");
			Wt::Dbo::belongsTo(a, ParentPage, "Parent_Page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "page";
		}

	private:
		friend class PagesDatabase;
};
class PageData : public BasePage, public DataSurrogateKey
{
	protected:
		long long _ModuleId;

	public:
		PageData(Wt::Dbo::ptr<Page> Ptr);

		std::string TitleKey;
		long long TitleModuleId;
		long long ParentPageId;
		long long ModuleId() const { return _ModuleId; }
};

#include "Dbo/AccessPath.h"

#endif

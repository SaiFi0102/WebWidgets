#ifndef PAGE_DBO_H
#define PAGE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"

class BasePage : public DataSurrogateKey
{
	protected:
		BasePage(long long id)
			: DataSurrogateKey(id)
		{ }

	public:
		std::string	DefaultInternalPath;
		std::string	Title;
};
class PageData : public BasePage
{
	protected:
		long long _ModuleId;

	public:
		long long AccessPathId;

		PageData(long long id, long long ModuleId)
			: BasePage(id), _ModuleId(ModuleId), AccessPathId(-1)
		{ }

		long long ModuleId() const { return _ModuleId; }
};
class Page : public BasePage
{
	protected:
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		Wt::Dbo::weak_ptr<AccessPath> AccessPathPtr;

		//PageCollections ChildrenPages;
		//Wt::Dbo::ptr<Page> ParentPage;

		Page()
			: BasePage(-1)
		{ }
		Page(long long id, Wt::Dbo::ptr<Module> ModulePtr)
			: BasePage(id), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _id, "pid");
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, DefaultInternalPath, "DefaultInternalPath", 50);
			Wt::Dbo::field(a, Title, "Title");

			Wt::Dbo::hasOne(a, AccessPathPtr, "accesspath");

			//Wt::Dbo::hasMany(a, ChildrenPages, Wt::Dbo::ManyToOne, "Parent_Page");
			//Wt::Dbo::belongsTo(a, ParentPage, "Parent_Page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "pages";
		}

	private:
		friend class PagesDatabase;
};

#include "Dbo/AccessPath.h"

#endif
#ifndef WW_DBO_PAGE_H
#define WW_DBO_PAGE_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{

		class BasePage
		{
		protected:
			BasePage() = default;
			BasePage(const std::string &name)
				: _name(name)
			{ }

			std::string _name;

		public:
			enum AuthRestriction
			{
				AuthUnrestricted = 0,
				AuthLoggedIn = 1,
				AuthLoggedInStrong = 2,
				AuthLoggedOut = 3
			};

			std::string name() const { return _name; }
			std::string	defaultInternalPath;
			bool preload = false;
			AuthRestriction authRestriction = AuthUnrestricted;
			bool allowOnDisabledLogin = false;
		};

		class Page : public BasePage
		{
		protected:
			ptr<Module> _modulePtr;

		public:
			PageAccessPathCollection pageAccessPathCollection;
			AccessHostNameCollection accessHostNameCollection;
			MenuItemOnPageCollection menuItemOnPageCollection;

			ptr<SingularKey> titleKey;

			PageCollection childrenPages;
			ptr<Page> parentPage;

			Page() = default;
			Page(const std::string &name, ptr<Module> modulePtr)
				: BasePage(name), _modulePtr(modulePtr)
			{ }

			ptr<Module> modulePtr() const { return _modulePtr; };

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, _name, "name", 50);
				Wt::Dbo::belongsTo(a, _modulePtr, "module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, defaultInternalPath, "defaultInternalPath", 50);
				Wt::Dbo::belongsTo(a, titleKey, "titleKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, preload, "preload");
				Wt::Dbo::field(a, authRestriction, "authRestriction");
				Wt::Dbo::field(a, allowOnDisabledLogin, "allowOnDisabledLogin");

				Wt::Dbo::hasMany(a, pageAccessPathCollection, Wt::Dbo::ManyToOne, "page");
				Wt::Dbo::hasMany(a, accessHostNameCollection, Wt::Dbo::ManyToOne, "defaultPage");
				Wt::Dbo::hasMany(a, menuItemOnPageCollection, Wt::Dbo::ManyToOne, "page");

				Wt::Dbo::hasMany(a, childrenPages, Wt::Dbo::ManyToOne, "parent_page");
				Wt::Dbo::belongsTo(a, parentPage, "parent_page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			}
			constexpr static const char *tableName()
			{
				return "page";
			}

		private:
			friend class PageDatabase;
		};

	}

	namespace Ddo
	{

		class Page : public Dbo::BasePage, public SurrogateKey
		{
		protected:
			long long _moduleId;

		public:
			Page(Dbo::ptr<Dbo::Page> ptr)
				: BasePage(*ptr),
				SurrogateKey(ptr.id()),
				_moduleId(ptr->modulePtr().id()),
				parentPageId(ptr->parentPage.id()),
				titleKey(ptr->titleKey->key()),
				titleModuleId(ptr->titleKey->modulePtr().id())
			{ }

			std::string titleKey;
			long long titleModuleId;
			long long parentPageId;
			long long moduleId() const { return _moduleId; }
		};

	}

}

#endif

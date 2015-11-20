#ifndef DBOINSTALLER_H
#define DBOINSTALLER_H

#include "Dbo/Module.h"
#include "Dbo/Configuration.h"
#include "Dbo/Language.h"
#include "Dbo/Page.h"

struct InstalledObjects
{
	//Authors
	Wt::Dbo::ptr<Author> SaifAuthor;
	Wt::Dbo::ptr<Author> EmwebAuthor;

	//Modules
	Wt::Dbo::ptr<Module> WtModule;
	Wt::Dbo::ptr<Module> ServerModule;
	Wt::Dbo::ptr<Module> LoggingModule;
	Wt::Dbo::ptr<Module> ConfigurationModule;
	Wt::Dbo::ptr<Module> LocalizationModule;
	Wt::Dbo::ptr<Module> AuthenticationModule;
	Wt::Dbo::ptr<Module> StylesModule;
	Wt::Dbo::ptr<Module> NavigationModule;

	//English
	Wt::Dbo::ptr<Language> EnglishLanguagePtr;

	//Language String Keys
	Wt::Dbo::ptr<SingularKey> HomePageTitle;
	Wt::Dbo::ptr<SingularKey> SiteMapPageTitle;

	//Pages
	Wt::Dbo::ptr<Page> LandingHomePage;
	Wt::Dbo::ptr<Page> SitemapPage;

	//Default Style
	Wt::Dbo::ptr<Style> DefaultStyle;

	//Access Paths
	Wt::Dbo::ptr<AccessHostName> GlobalAccessHost;
	Wt::Dbo::ptr<LanguageAccessPath> EnglishAccessPath;
	Wt::Dbo::ptr<PageAccessPath> HomePageAccessPath;
	Wt::Dbo::ptr<PageAccessPath> SiteMapAccessPath;
};

class DboInstaller
{
	public:
		DboInstaller(Wt::Dbo::SqlConnectionPool &SQLPool);
		DboInstaller(Wt::Dbo::SqlConnection &SQLConnection);

		void CreateTables();
		void InsertRows();
		void DropTables();
		void Install();
		void Reinstall();

		const InstalledObjects &Objects() const;

	protected:
		void MapClasses();

		void InsertAuthorModules();
		void InsertConfigurations();
		void InsertLanguages();

		InstalledObjects O;
		Wt::Dbo::Session DboSession;
};

#endif
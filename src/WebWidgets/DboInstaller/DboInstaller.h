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

	//Pages
	Wt::Dbo::ptr<Page> LandingHomePage;
	Wt::Dbo::ptr<Page> SitemapPage;

	//Default Style
	Wt::Dbo::ptr<Style> DefaultStyle;
	
	//Ptrs to gather IDs from after insertion
	Wt::Dbo::ptr<AccessPath> DefaultLanguageAccessPath;
	Wt::Dbo::ptr<AccessPath> MobileVersionAccessPath;
	Wt::Dbo::ptr<AccessPath> HomePageAccessPath;

	Wt::Dbo::ptr<ConfigurationLongInt> DefaultAccessPathVal;
	Wt::Dbo::ptr<ConfigurationLongInt> HomePageAccessPathIdVal;
	Wt::Dbo::ptr<ConfigurationLongInt> MobileAccessPathIdVal;
	Wt::Dbo::ptr<ConfigurationString> DefaultStyleNameVal;
	Wt::Dbo::ptr<ConfigurationLongInt> DefaultStyleAuthorVal;
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
#ifndef DBOINSTALLER_H
#define DBOINSTALLER_H

#include <WebWidgets/Dbo/Module.h>
#include <WebWidgets/Dbo/Configuration.h>
#include <WebWidgets/Dbo/Language.h>
#include <WebWidgets/Dbo/Page.h>
#include <map>

namespace WW
{
	namespace Installer
	{

		struct InstalledObjects
		{
			//Authors
			Dbo::ptr<Dbo::Author> SaifAuthor;
			Dbo::ptr<Dbo::Author> EmwebAuthor;

			//Modules
			Dbo::ptr<Dbo::Module> WtModule;
			Dbo::ptr<Dbo::Module> ServerModule;
			Dbo::ptr<Dbo::Module> LoggingModule;
			Dbo::ptr<Dbo::Module> ConfigurationModule;
			Dbo::ptr<Dbo::Module> LocalizationModule;
			Dbo::ptr<Dbo::Module> AuthenticationModule;
			Dbo::ptr<Dbo::Module> StylingModule;
			Dbo::ptr<Dbo::Module> NavigationModule;

			//English
			Dbo::ptr<Dbo::Language> EnglishLanguage;
			Dbo::ptr<Dbo::Language> FrenchLanguage;

			//Default Style
			Dbo::ptr<Dbo::Style> DefaultStyle;

			//Access Paths
			Dbo::ptr<Dbo::AccessHostName> GlobalAccessHost;
			Dbo::ptr<Dbo::LanguageAccessPath> EnglishAccessPath;
			Dbo::ptr<Dbo::PageAccessPath> HomePageAccessPath;
			Dbo::ptr<Dbo::PageAccessPath> SiteMapAccessPath;

			//Main menu
			Dbo::ptr<Dbo::NavigationMenu> MainMenu;
			//Main template
			Dbo::ptr<Dbo::Template> MainTemplate;
		};

		class DboInstaller
		{
		public:
			DboInstaller(Wt::Dbo::SqlConnectionPool &sqlPool);
			DboInstaller(Wt::Dbo::SqlConnection &sqlConnection);

			void createTables();
			void insertRows();
			void dropTables();
			void install();
			void reinstall();

			const InstalledObjects &objects() const { return O; }

		protected:
			void mapClasses();

			Dbo::ptr<Dbo::SingularKey> addSingularKeyString(Dbo::ptr<Dbo::Language> languagePtr, const std::string &key, Dbo::ptr<Dbo::Module> keyModulePtr, const std::string &string, bool isEmail = false);

			void insertAuthorModules();
			void insertConfigurations();
			void insertLanguages();
			void insertTestData();

			InstalledObjects O;
			Wt::Dbo::Session dboSession;

		private:
			typedef std::map<std::pair<std::string, Dbo::ptr<Dbo::Module>>, Dbo::ptr<Dbo::SingularKey>> LanguageKeyToPtrMap;
			LanguageKeyToPtrMap _languageKeyToPtrMap;
		};

	}
}

#endif
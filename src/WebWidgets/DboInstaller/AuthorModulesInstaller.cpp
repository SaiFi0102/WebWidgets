#include "DboInstaller/DboInstaller.h"
#include "DboDatabase/ModulesDatabase.h"

void DboInstaller::InsertAuthorModules()
{
	//Authors
	O.SaifAuthor = DboSession.add(new Author(1));
	O.SaifAuthor.modify()->Name = "Saif Ur Rehman";
	O.SaifAuthor.modify()->Email = "saifi0102@gmail.com";

	O.EmwebAuthor = DboSession.add(new Author(2));
	O.EmwebAuthor.modify()->Name = "Emweb Bvba";
	O.EmwebAuthor.modify()->Email = "info@emweb.be";
	O.EmwebAuthor.modify()->Website = "http://www.emweb.be";

	//Modules
	O.WtModule = DboSession.add(new Module(ModulesDatabase::Wt));
	O.WtModule.modify()->Name = "Wt";
	O.WtModule.modify()->VersionSeries = 3;
	O.WtModule.modify()->VersionMajor = 3;
	O.WtModule.modify()->VersionMinor = 3;
	O.WtModule.modify()->AuthorPtr = O.EmwebAuthor;

	O.ServerModule = DboSession.add(new Module(ModulesDatabase::Server));
	O.ServerModule.modify()->Name = "Server";
	O.ServerModule.modify()->VersionSeries = 1;
	O.ServerModule.modify()->VersionMajor = 0;
	O.ServerModule.modify()->VersionMinor = 0;
	O.ServerModule.modify()->AuthorPtr = O.SaifAuthor;

	O.LoggingModule = DboSession.add(new Module(ModulesDatabase::Logging));
	O.LoggingModule.modify()->Name = "Logging";
	O.LoggingModule.modify()->VersionSeries = 1;
	O.LoggingModule.modify()->VersionMajor = 0;
	O.LoggingModule.modify()->VersionMinor = 0;
	O.LoggingModule.modify()->AuthorPtr = O.SaifAuthor;

	O.ConfigurationModule = DboSession.add(new Module(ModulesDatabase::Configurations));
	O.ConfigurationModule.modify()->Name = "Configuration";
	O.ConfigurationModule.modify()->VersionSeries = 1;
	O.ConfigurationModule.modify()->VersionMajor = 0;
	O.ConfigurationModule.modify()->VersionMinor = 0;
	O.ConfigurationModule.modify()->AuthorPtr = O.SaifAuthor;

	O.LocalizationModule = DboSession.add(new Module(ModulesDatabase::Localization));
	O.LocalizationModule.modify()->Name = "Localization";
	O.LocalizationModule.modify()->VersionSeries = 1;
	O.LocalizationModule.modify()->VersionMajor = 0;
	O.LocalizationModule.modify()->VersionMinor = 0;
	O.LocalizationModule.modify()->AuthorPtr = O.SaifAuthor;

	O.AuthenticationModule = DboSession.add(new Module(ModulesDatabase::Authentication));
	O.AuthenticationModule.modify()->Name = "Authentication";
	O.AuthenticationModule.modify()->VersionSeries = 1;
	O.AuthenticationModule.modify()->VersionMajor = 0;
	O.AuthenticationModule.modify()->VersionMinor = 0;
	O.AuthenticationModule.modify()->AuthorPtr = O.SaifAuthor;

	O.StylesModule = DboSession.add(new Module(ModulesDatabase::Styles));
	O.StylesModule.modify()->Name = "Styles";
	O.StylesModule.modify()->VersionSeries = 1;
	O.StylesModule.modify()->VersionMajor = 0;
	O.StylesModule.modify()->VersionMinor = 0;
	O.StylesModule.modify()->AuthorPtr = O.SaifAuthor;

	O.NavigationModule = DboSession.add(new Module(ModulesDatabase::Navigation));
	O.NavigationModule.modify()->Name = "Navigation";
	O.NavigationModule.modify()->VersionSeries = 1;
	O.NavigationModule.modify()->VersionMajor = 0;
	O.NavigationModule.modify()->VersionMinor = 0;
	O.NavigationModule.modify()->AuthorPtr = O.SaifAuthor;
}
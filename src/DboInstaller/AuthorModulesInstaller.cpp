#include "DboInstaller.h"
#include <WebWidgets/DboDatabase/ModuleDatabase.h>

namespace WW
{
	namespace Installer
	{

void DboInstaller::insertAuthorModules()
{
	//Authors
	auto saifAuthor = new Dbo::Author(1);
	saifAuthor->name = "Saif Ur Rehman";
	saifAuthor->email = "saifi0102@gmail.com";
	O.SaifAuthor = dboSession.add(saifAuthor);

	auto emwebAuthor = new Dbo::Author(2);
	emwebAuthor->name = "Emweb Bvba";
	emwebAuthor->email = "info@emweb.be";
	emwebAuthor->website = "http://www.emweb.be";
	O.EmwebAuthor = dboSession.add(emwebAuthor);

	//Modules
	auto wtModule = new Dbo::Module(ModuleDatabase::Wt);
	wtModule->name = "Wt";
	wtModule->versionSeries = 3;
	wtModule->versionMajor = 3;
	wtModule->versionMinor = 5;
	wtModule->authorPtr = O.EmwebAuthor;
	O.WtModule = dboSession.add(wtModule);

	auto serverModule = new Dbo::Module(ModuleDatabase::Server);
	serverModule->name = "Server";
	serverModule->versionSeries = 0;
	serverModule->versionMajor = 0;
	serverModule->versionMinor = 0;
	serverModule->authorPtr = O.SaifAuthor;
	O.ServerModule = dboSession.add(serverModule);

	auto loggingModule = new Dbo::Module(ModuleDatabase::Logging);
	loggingModule->name = "Logging";
	loggingModule->versionSeries = 0;
	loggingModule->versionMajor = 0;
	loggingModule->versionMinor = 0;
	loggingModule->authorPtr = O.SaifAuthor;
	O.LoggingModule = dboSession.add(loggingModule);

	auto configurationModule = new Dbo::Module(ModuleDatabase::Configuration);
	configurationModule->name = "Configuration";
	configurationModule->versionSeries = 0;
	configurationModule->versionMajor = 0;
	configurationModule->versionMinor = 0;
	configurationModule->authorPtr = O.SaifAuthor;
	O.ConfigurationModule = dboSession.add(configurationModule);

	auto localizationModule = new Dbo::Module(ModuleDatabase::Localization);
	localizationModule->name = "Localization";
	localizationModule->versionSeries = 0;
	localizationModule->versionMajor = 0;
	localizationModule->versionMinor = 0;
	localizationModule->authorPtr = O.SaifAuthor;
	O.LocalizationModule = dboSession.add(localizationModule);

	auto authenticationModule = new Dbo::Module(ModuleDatabase::Authentication);
	authenticationModule->name = "Authentication";
	authenticationModule->versionSeries = 0;
	authenticationModule->versionMajor = 0;
	authenticationModule->versionMinor = 0;
	authenticationModule->authorPtr = O.SaifAuthor;
	O.AuthenticationModule = dboSession.add(authenticationModule);

	auto stylesModule = new Dbo::Module(ModuleDatabase::Styling);
	stylesModule->name = "Styling";
	stylesModule->versionSeries = 0;
	stylesModule->versionMajor = 0;
	stylesModule->versionMinor = 0;
	stylesModule->authorPtr = O.SaifAuthor;
	O.StylingModule = dboSession.add(stylesModule);

	auto navigationModule = new Dbo::Module(ModuleDatabase::Navigation);
	navigationModule->name = "Navigation";
	navigationModule->versionSeries = 0;
	navigationModule->versionMajor = 0;
	navigationModule->versionMinor = 0;
	navigationModule->authorPtr = O.SaifAuthor;
	O.NavigationModule = dboSession.add(navigationModule);
}

	}
}

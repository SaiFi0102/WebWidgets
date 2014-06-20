#include <Wt/WMessageResources>

#include "Objects/DboLocalizedStrings.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationsCache.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "Application/WServer.h"
#include "Application/Application.h"

DboLocalizedStrings::DboLocalizedStrings(LanguagesDatabase *Languages)
	: Languages(Languages)
{ }

void DboLocalizedStrings::refresh()
{}

void DboLocalizedStrings::hibernate()
{}

bool DboLocalizedStrings::resolveKey(const std::string &key, std::string &result)
{
	return resolveKey(key, ModulesDatabase::Wt, result);
}

bool DboLocalizedStrings::resolveKey(const std::string &key, long long ModuleId, std::string &result)
{
	Application *wapp = Application::instance();
	std::string Locale = wapp ? wapp->locale().name() : "";

	//If no locale is given or string not found in the locale, try to look for the string in the default locale
	if(Locale.empty() || !Languages->GetSingleString(Locale, key, ModuleId, result))
	{
		//Use default locale from configuration or function is called independent from Wt::WServer, use "en"
		WServer *server = WServer::instance();
		Locale = "en"; //Default
		if(server)
		{
			long long DefaultAccessPathId = wapp ? wapp->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1) : server->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1);
			Wt::Dbo::ptr<AccessPath> DefaultAccessPath = server->AccessPaths()->GetPtr(DefaultAccessPathId);
			if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
			{
				Locale = DefaultAccessPath->LanguagePtr.id();
			}
		}

		//Return false if default locale does not have the string either
		return Languages->GetSingleString(Locale, key, ModuleId, result);
	}
	return true;
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, std::string &result, uint64_t amount)
{
	return resolvePluralKey(key, ModulesDatabase::Wt, result, amount);
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, long long ModuleId, std::string &result, uint64_t amount)
{
	Application *wapp = Application::instance();
	std::string Locale = wapp ? wapp->locale().name() : "";
	std::string PluralExpression;

	//If no locale is given or plural expression for locale is not found or string not found in the locale, try to look for the string in the default locale
	if(Locale.empty()
		|| !Languages->GetPluralExpression(Locale, PluralExpression)
		|| !Languages->GetPluralString(Locale, key, ModuleId, Wt::WMessageResources::evalPluralCase(PluralExpression, amount), result))
	{
		//Use default locale from configuration or if function is called independent from Wt::WServer, use "en"
		WServer *server = WServer::instance();
		Locale = "en"; //Default
		if(server)
		{
			long long DefaultAccessPathId = wapp ? wapp->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1) : server->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1);
			Wt::Dbo::ptr<AccessPath> DefaultAccessPath = server->AccessPaths()->GetPtr(DefaultAccessPathId);
			if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
			{
				Locale = DefaultAccessPath->LanguagePtr.id();
			}
		}

		//Return false if default locale does not have the string either
		if(!Languages->GetPluralExpression(Locale, PluralExpression)
			|| !Languages->GetPluralString(Locale, key, ModuleId, Wt::WMessageResources::evalPluralCase(PluralExpression, amount), result))
		{
			return false;
		}
	}
	return true;
}

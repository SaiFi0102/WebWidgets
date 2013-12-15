#include "Objects/LocalizedStrings.h"
#include "DboDatabase/ModulesDatabase.h"
#include <Wt/WApplication>
#include <Wt/WMessageResources>
#include "Application/WServer.h"

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
	Wt::WApplication *wapp = Wt::WApplication::instance();
	std::string Locale = wapp ? wapp->locale() : "";

	//If no locale is given or string not found in the locale, try to look for the string in the default locale
	if(Locale.empty() || !Languages->GetSingleString(Locale, key, ModuleId, result))
	{
		//Use default locale from configuration or function is called independent from Wt::WServer, use "en"
		WServer *server = WServer::instance();
		if(server)
		{
			Locale = server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en");
		}
		else
		{
			Locale = "en";
		}

		//Return false if default locale does not have the string either
		if(!Languages->GetSingleString(Locale, key, ModuleId, result))
		{
			return false;
		}
	}
	return true;
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, std::string &result, uint64_t amount)
{
	return resolvePluralKey(key, ModulesDatabase::Wt, result, amount);
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, long long ModuleId, std::string &result, uint64_t amount)
{
	Wt::WApplication *wapp = Wt::WApplication::instance();
	std::string Locale = wapp ? wapp->locale() : "";
	std::string PluralExpression;

	//If no locale is given or plural expression for locale is not found or string not found in the locale, try to look for the string in the default locale
	if(Locale.empty()
		|| !Languages->GetPluralExpression(Locale, PluralExpression)
		|| !Languages->GetPluralString(Locale, key, ModuleId, Wt::WMessageResources::evalPluralCase(PluralExpression, amount), result))
	{
		//Use default locale from configuration or function is called independent from Wt::WServer, use "en"
		WServer *server = WServer::instance();
		if(server)
		{
			Locale = server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en");
		}
		else
		{
			Locale = "en";
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

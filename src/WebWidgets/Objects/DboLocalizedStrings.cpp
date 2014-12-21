#include <Wt/WMessageResources>

#include "Objects/DboLocalizedStrings.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationsCache.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/StylesDatabase.h"
#include "Dbo/Style.h"
#include "Application/WServer.h"
#include "Application/Application.h"

DboLocalizedStrings::DboLocalizedStrings(WServer *Server)
	: _Server(Server)
{ }

bool DboLocalizedStrings::resolveKey(const std::string &key, std::string &result)
{
	return resolveKey(key, ModulesDatabase::Wt, result);
}

bool DboLocalizedStrings::resolveKey(const std::string &key, long long ModuleId, std::string &result)
{
	Application *wapp = Application::instance();
	std::string Locale = wapp ? wapp->locale().name() : "";

	//If no locale is given or string not found in the locale, try to look for the string in the default locale
	if(Locale.empty() || !_Server->Languages()->GetSingleString(Locale, key, ModuleId, result))
	{
		//Use default locale from configuration or function is called independent from Wt::WServer, use "en"
		Locale = "en"; //Default
		long long DefaultAccessPathId = wapp ? wapp->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1) : _Server->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1);
		Wt::Dbo::ptr<AccessPath> DefaultAccessPath = _Server->AccessPaths()->GetPtr(DefaultAccessPathId);
		if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
		{
			Locale = DefaultAccessPath->LanguagePtr.id();
		}

		//Return false if default locale does not have the string either
		return _Server->Languages()->GetSingleString(Locale, key, ModuleId, result);
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
		|| !_Server->Languages()->GetPluralExpression(Locale, PluralExpression)
		|| !_Server->Languages()->GetPluralString(Locale, key, ModuleId, Wt::WMessageResources::evalPluralCase(PluralExpression, amount), result))
	{
		//Use default locale from configuration or if function is called independent from Wt::WServer, use "en"
		Locale = "en"; //Default
		long long DefaultAccessPathId = wapp ? wapp->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1) : _Server->Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1);
		Wt::Dbo::ptr<AccessPath> DefaultAccessPath = _Server->AccessPaths()->GetPtr(DefaultAccessPathId);
		if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
		{
			Locale = DefaultAccessPath->LanguagePtr.id();
		}

		//Return false if default locale does not have the string either
		if(!_Server->Languages()->GetPluralExpression(Locale, PluralExpression)
			|| !_Server->Languages()->GetPluralString(Locale, key, ModuleId, Wt::WMessageResources::evalPluralCase(PluralExpression, amount), result))
		{
			return false;
		}
	}
	return true;
}

bool DboLocalizedStrings::resolveTemplateKey(const std::string &templateName, long long moduleId, std::string &result)
{
	Application *app = Application::instance();

	if(app)
	{
		Wt::Dbo::ptr<Style> CurrentStyle = app->CurrentStyle();
		if(!CurrentStyle
			|| !_Server->Styles()->GetStyleTemplateStr(templateName, moduleId, CurrentStyle->Name(), CurrentStyle->AuthorPtr().id(), result))
		{
			if(!_Server->Styles()->GetTemplateStr(templateName, moduleId, result))
			{
				return false;
			}
		}
	}
	else
	{
		if(!_Server->Styles()->GetTemplateStr(templateName, moduleId, result))
		{
			return false;
		}
	}
	return true;
}

bool DboLocalizedStrings::loadTemplateStyleSheet(const std::string &templateName, long long moduleId)
{
	Application *app = Application::instance();
	if(app)
	{
		app->UseTemplateStyleSheet(_Server->Styles()->GetTemplatePtr(templateName, moduleId));
	}
	return true;
}

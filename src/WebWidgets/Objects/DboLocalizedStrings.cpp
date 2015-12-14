#include <Wt/WMessageResources>

#include "Objects/DboLocalizedStrings.h"
#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationCache.h"
#include "DboDatabase/LanguageDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/StyleDatabase.h"
#include "Application/WServer.h"
#include "Application/WApplication.h"

namespace WW
{

DboLocalizedStrings::DboLocalizedStrings(WServer *server)
	: _server(server)
{ }

bool DboLocalizedStrings::resolveKey(const std::string &key, std::string &result)
{
	return resolveKey(key, ModuleDatabase::Wt, result);
}

bool DboLocalizedStrings::resolveKey(const std::string &key, long long moduleId, std::string &result)
{
	WApplication *wapp = WApplication::instance();
	std::string locale = wapp ? wapp->locale().name() : "";

	//If no locale is given or string not found in the locale, try to look for the string in the default locale
	if(locale.empty() || !_server->languages()->getSingularString(locale, key, moduleId, result))
	{
		//Use default locale from configuration or else use "en"
		locale = _server->accessPaths()->accessHostNamePtr("")->languageCode;
		if(locale.empty())
			locale = "en";

		if(!_server->languages()->getSingularString(locale, key, moduleId, result))
		{
			if(locale == "en")
				return false;
		}
		else
			return true;

		//Return false if default locale does not have the string either
		return _server->languages()->getSingularString("en", key, moduleId, result);
	}
	return true;
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, std::string &result, uint64_t amount)
{
	return resolvePluralKey(key, ModuleDatabase::Wt, result, amount);
}

bool DboLocalizedStrings::resolvePluralKey(const std::string &key, long long moduleId, std::string &result, uint64_t amount)
{
	WApplication *wapp = WApplication::instance();
	std::string locale = wapp ? wapp->locale().name() : "";
	std::string pluralExpression;

	//If no locale is given or plural expression for locale is not found or string not found in the locale, try to look for the string in the default locale
	if(locale.empty()
		|| !_server->languages()->getPluralExpression(locale, pluralExpression)
		|| !_server->languages()->getPluralString(locale, key, moduleId, Wt::WMessageResources::evalPluralCase(pluralExpression, amount), result))
	{
		//Use default locale from configuration or else use "en"
		locale = _server->accessPaths()->accessHostNamePtr("")->languageCode;
		if(locale.empty())
			locale = "en";

		if((!_server->languages()->getPluralExpression(locale, pluralExpression)
			|| !_server->languages()->getPluralString(locale, key, moduleId, Wt::WMessageResources::evalPluralCase(pluralExpression, amount), result)))
		{
			if(locale == "en")
				return false;
		}
		else
			return true;

		//Return false if default locale does not have the string either
		if(!_server->languages()->getPluralExpression("en", pluralExpression)
			|| !_server->languages()->getPluralString("en", key, moduleId, Wt::WMessageResources::evalPluralCase(pluralExpression, amount), result))
		{
			return false;
		}
	}
	return true;
}

bool DboLocalizedStrings::resolveTemplateKey(const std::string &templateName, long long moduleId, std::string &result)
{
	WApplication *app = WApplication::instance();

	if(app)
	{
		Ddo::cPtr<Ddo::Style> currentStyle = app->currentStyle();
		if(!currentStyle
			|| !_server->styles()->getStyleTemplateStr(templateName, moduleId, currentStyle->name(), currentStyle->authorId(), result))
		{
			if(!_server->styles()->getTemplateStr(templateName, moduleId, result))
				return false;
		}
	}
	else
	{
		if(!_server->styles()->getTemplateStr(templateName, moduleId, result))
			return false;
	}
	return true;
}

bool DboLocalizedStrings::loadTemplateStyleSheet(const std::string &templateName, long long moduleId)
{
	WApplication *app = WApplication::instance();
	if(app)
		app->useTemplateStyleSheet(_server->styles()->getTemplatePtr(templateName, moduleId));

	return true;
}

}

#include "Application/Application.h"
#include "Objects/LocalizedStrings.h"
#include "Application/WServer.h"

#include <Wt/WText>
#include <Wt/WAnchor>

Application::Application(const Wt::WEnvironment &env)
	: Wt::WApplication(env)
{
	WServer *Server = WServer::instance();

	//Set environment locale in Application
	std::string LanguageAccept = env.locale().name();
	if(!LanguageAccept.empty())
	{
		if(Server->GetLanguages()->LanguageAcceptExists(LanguageAccept))
		{
			setLocale(Server->GetLanguages()->GetLocaleFromLanguageAccept(LanguageAccept));
		}
		else
		{
			std::string::size_type DashPosition = LanguageAccept.find('-');
			if(DashPosition == std::string::npos) //If Language-Accept is without country localization
			{
				if(Server->GetLanguages()->LanguageAcceptExists(LanguageAccept + "*")) //Check if language accept with wild card exist
				{
					setLocale(Server->GetLanguages()->GetLocaleFromLanguageAccept(LanguageAccept + "*"));
				}
				else //else set locale from default language
				{
					setLocale(Server->GetLanguages()->GetLocaleFromCode(Server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en")));
				}
			}
			else
			{
				std::string LanguageAcceptSubStr = LanguageAccept.substr(0, DashPosition - 1);
				if(Server->GetLanguages()->LanguageAcceptExists(LanguageAcceptSubStr + "*")) //Check if language accept with wild card exist
				{
					setLocale(Server->GetLanguages()->GetLocaleFromLanguageAccept(LanguageAcceptSubStr + "*"));
				}
				else //else set locale from default language
				{
					setLocale(Server->GetLanguages()->GetLocaleFromCode(Server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en")));
				}
			}
		}
	}
	else //If no language accept http header found, just set locale from default language
	{
		setLocale(Server->GetLanguages()->GetLocaleFromCode(Server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en")));
	}

	//Set language from internal path
	SetLanguageFromInternalPath(true);

	//Connect SetLanguageFromInternalPath()
	internalPathChanged().connect(boost::bind(&Application::SetLanguageFromInternalPath, this, false));

	//Use database back end localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server->GetLanguages()));

	//CSS Stylesheet
	//this->styleSheet().ruleModified()

	new Wt::WText("HI", root());
	new Wt::WBreak(root());
	new Wt::WText(internalPath(), root());
	new Wt::WBreak(root());
	new Wt::WText(locale().name(), root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/testing"), "Test Link", root());
}

Application *Application::CreateApplication(const Wt::WEnvironment &env)
{
	return new Application(env);
}

Application *Application::instance()
{
	return dynamic_cast<Application*>(Wt::WApplication::instance());
}

void Application::SetLanguageFromInternalPath(bool setpath)
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string DefaultLanguage = Server->GetConfigurations()->GetStr("DefaultLanguage", ModulesDatabase::Localization, "en");

	//If internal path is landing page then there is no need to do anything else
	if(InternalPath == "/")
	{
		if(setpath && locale().name() != DefaultLanguage)
		{
			setInternalPath("/" + locale().name());
		}
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::const_iterator Itr = Tokens.begin();

	//If first internal path is mobile then check next path
	if(*Itr == "mobile")
	{
		++Itr;
	}
	//Set language if the language is correct in internal path
	if(Server->GetLanguages()->LanguageCodeExists(*Itr))
	{
		if(locale().name() != *Itr)
		{
			setLocale(Server->GetLanguages()->GetLocaleFromCode(*Itr));
		}
	}
	else //if internal path has an invalid language or no language at all, set default
	{
		if(locale().name() != DefaultLanguage)
		{
			setLocale(Server->GetLanguages()->GetLocaleFromCode(DefaultLanguage));
		}
	}

	//Remove default language from internal path if there
	if(*Itr == DefaultLanguage)
	{
		InternalPath.erase(InternalPath.find(DefaultLanguage), DefaultLanguage.length() + 1);
		setInternalPath(InternalPath);
	}
	else if(setpath)
	{
		InternalPath.replace(InternalPath.find(*Itr), Itr->size(), locale().name());
		setInternalPath(InternalPath);
	}
}

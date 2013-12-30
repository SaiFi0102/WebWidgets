#include "Application/Application.h"
#include "Objects/LocalizedStrings.h"
#include "Application/WServer.h"

#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>

Application::Application(const Wt::WEnvironment &env)
	: Wt::WApplication(env), _LanguageFromHostname(false), _LocaleChanged(this), _SkipLanguageInternalPath(false)
{
	WServer *Server = WServer::instance();
	_UserLocale = env.locale();
	long long DefaultAccessPathId = Server->GetConfigurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1);
	Wt::WLocale ConfigDefaultLocale = Server->GetLanguages()->GetLocaleFromCode(Server->GetAccessPaths()->GetDbo(DefaultAccessPathId).LanguagePtr.id());

	//Check if user is using an AccessPath before checking LanguageAccept for Language
	std::string _hostname = env.hostName();
	if(Server->GetAccessPaths()->LanguageAccessPathExists(_hostname, "")) //Use hostname as received
	{
		setLocale(Server->GetLanguages()->GetLocaleFromCode(Server->GetAccessPaths()->GetDbo(_hostname, "").LanguagePtr.id()));
		_LanguageFromHostname = true;
	}
	else if(_hostname.substr(0, 4) == "www.") //If not found, try without "www."
	{
		_hostname = _hostname.substr(4);
		if(Server->GetAccessPaths()->LanguageAccessPathExists(_hostname, ""))
		{
			setLocale(Server->GetLanguages()->GetLocaleFromCode(Server->GetAccessPaths()->GetDbo(_hostname, "").LanguagePtr.id()));
			_LanguageFromHostname = true;
		}
	}
	
	//Set environment locale in Application if language was not found in access path
	if(!_LanguageFromHostname)
	{
		std::string LanguageAccept = env.locale().name();
		if(LanguageAccept.empty()) //If no language accept http header found, just set locale from default language
		{
			setLocale(ConfigDefaultLocale);
		}
		else
		{
			if(Server->GetLanguages()->LanguageAcceptExists(LanguageAccept))
			{
				setLocale(Server->GetLanguages()->GetLocaleFromLanguageAccept(LanguageAccept));
			}
			else //If not break up the LanguageAccept and check with wild cards
			{
				std::string LanguageAcceptLookup = LanguageAccept;
				std::string::size_type DashPosition = LanguageAccept.find('-');
				if(DashPosition != std::string::npos) //If there is a country specific language in language accept
				{
					LanguageAcceptLookup = LanguageAccept.substr(0, DashPosition); //Remove everything with and after the dash, "-".
				}

				if(Server->GetLanguages()->LanguageAcceptExists(LanguageAcceptLookup + "*")) //Check if LanguageAccept with wild card exist
				{
					setLocale(Server->GetLanguages()->GetLocaleFromLanguageAccept(LanguageAcceptLookup + "*"));
				}
				else //else set locale from default language
				{
					setLocale(ConfigDefaultLocale);
				}
			}
		}
	}
	_SessionDefaultLocale = locale();

	//Connect SetLanguageFromInternalPath() and Set language from internal path
	internalPathChanged().connect(boost::bind(&Application::SetLanguageFromInternalPath, this));
	SetLanguageFromInternalPath();

	//Use database back end localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server->GetLanguages()));

	//CSS Stylesheet
	//this->styleSheet().ruleModified()

	new Wt::WText("HI", root());
	new Wt::WBreak(root());
	auto ip = new Wt::WText(internalPath(), root());
	new Wt::WBreak(root());
	auto txt = new Wt::WText(locale().name(), root());
	internalPathChanged().connect(boost::bind<void>([this, ip](){
		ip->setText(internalPath());
	}));
	LocaleChanged().connect(boost::bind<void>([this, txt](){
		txt->setText(locale().name());
	}));
	new Wt::WBreak(root());
	new Wt::WText(std::string("Session Default Language: ") + _SessionDefaultLocale.name(), root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/testing"), "Test Link", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/en"), "/en", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/fr"), "/fr", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/english"), "/english", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/french"), "/french", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/"), "/", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums"), "/forums", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums/topic"), "/forums/topic", root());
	new Wt::WBreak(root());
	new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums/topic/1"), "/forums/topic/1", root());
	new Wt::WBreak(root());
	Wt::WPushButton *rc = new Wt::WPushButton("Reload Configurations", root());
	rc->clicked().connect(boost::bind<void>([Server](){
		Server->GetConfigurations()->FetchAll();
	}));
}

Application *Application::CreateApplication(const Wt::WEnvironment &env)
{
	return new Application(env);
}

Application *Application::instance()
{
	return dynamic_cast<Application*>(Wt::WApplication::instance());
}

void Application::SetLanguageFromInternalPath()
{
	if(_SkipLanguageInternalPath) //Skip when setInternalPath() was recalled from this function
	{
		_SkipLanguageInternalPath = false;
		return;
	}
	WServer *Server = WServer::instance();
	int IPLM = Server->GetConfigurations()->GetEnum("InternalPathMode", ModulesDatabase::Localization, 3); //Internal path language presentation mode
	if(IPLM == 5) //Do not get language from internal path
	{
		return;
	}
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	//If internal path is landing page then there is no need to know anything else
	if(InternalPath == "/")
	{
		if(IPLM == 1)
		{
			_SkipLanguageInternalPath = true;
			setInternalPath(Server->GetAccessPaths()->FirstAccessPath(locale().name(), HostName, _LanguageFromHostname), true);
		}
		else if(IPLM == 2 && locale().name() != _SessionDefaultLocale.name()) //Set default if always hide default
		{
			setLocale(_SessionDefaultLocale);
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
	if(!Itr->empty() && HostName.substr(0, 4) == "www." && !Server->GetAccessPaths()->LanguageAccessPathExists(HostName, *Itr)) //Use hostname without "www." if access path does not exists with the "www."
	{
		HostName = HostName.substr(4);
	}
	std::string LanguageCode;
	bool Found = false;
	if(!Itr->empty() && Server->GetAccessPaths()->LanguageAccessPathExists(HostName, *Itr))
	{
		LanguageCode = Server->GetAccessPaths()->GetDbo(HostName, *Itr).LanguagePtr.id();
		Found = true;
	}
	//If LanguageAccessPath exists but not if Language is set from hostname and using HostUnspecificLanguage is not allowed(false)
	else if(!Itr->empty() && Server->GetAccessPaths()->LanguageAccessPathExists("", *Itr)
		&& (!_LanguageFromHostname || Server->GetConfigurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false)))
	{
		LanguageCode = Server->GetAccessPaths()->GetDbo("", *Itr).LanguagePtr.id();
		Found = true;
	}

	if(Found)
	{
		if(locale().name() != LanguageCode)
		{
			setLocale(Server->GetLanguages()->GetLocaleFromCode(LanguageCode));
		}
		//Remove default language automatically from internal path if mode is always hide default and locale is default language
		if((IPLM == 2 || IPLM == 3) && locale().name() == _SessionDefaultLocale.name())
		{
			_SkipLanguageInternalPath = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr), Itr->size() + 1, ""), true);
		}
	}
	else
	{
		if(IPLM == 1 || (IPLM == 2 && locale().name() != _SessionDefaultLocale.name())) //If set to always shown but not if set to hide default and if current language is default
		{
			std::string CurrentLanguageAccessPath = Server->GetAccessPaths()->FirstAccessPath(locale().name(), HostName, _LanguageFromHostname);
			if(CurrentLanguageAccessPath == "/")
			{
				CurrentLanguageAccessPath = "";
			}
			//Prepend current language instead of replacing because that path mostly wont be language code
			_SkipLanguageInternalPath = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr),
				Itr->size() + 1,
				CurrentLanguageAccessPath + "/" + *Itr), true);
		}
	}
}

void Application::setLocale(const Wt::WLocale &locale)
{
	Wt::WApplication::setLocale(locale);
	_LocaleChanged.emit();
}

Wt::Signal<void> & Application::LocaleChanged()
{
	return _LocaleChanged;
}

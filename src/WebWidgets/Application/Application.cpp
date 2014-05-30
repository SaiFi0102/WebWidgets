#include "Application/Application.h"
#include "Application/WServer.h"
#include "Objects/LocalizedStrings.h"
#include "Objects/DboCssRule.h"

#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationsProxy.h"
#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/ReadLock.h"

#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>

Application::Application(const Wt::WEnvironment &env)
	: StartTime(boost::posix_time::microsec_clock::local_time()),
	Wt::WApplication(env),
	_LanguageFromHostname(false),
	_LocaleChanged(this),
	_SkipLanguageInternalPath(false),
	_MobileVersionFromHostname(false),
	_MobileVersionChanged(this),
	_MobileVersionFromInternalPath(false)
{
	//Enable server push
	enableUpdates();

	WServer *Server = WServer::instance();
	_Configurations = new ConfigurationsProxy(Server->Configurations(), this); //Local copy of configuration ptrs

	//Use database back end localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server->Languages()));

	//Set Default and Client's environment locale
	_ClientLocale = env.locale();
	Wt::Dbo::ptr<AccessPath> DefaultAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1));
	Wt::WLocale ConfigDefaultLocale;
	if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
	{
		ConfigDefaultLocale = Server->Languages()->GetLocaleFromCode(DefaultAccessPath->LanguagePtr.id());
	}
	else
	{
		ConfigDefaultLocale = Server->Languages()->GetLocaleFromCode("en");
	}

	//Check if user is using an AccessPath before checking LanguageAccept for Language
	std::string _hostname = env.hostName();
	Wt::Dbo::ptr<AccessPath> HostnameAccessPath = Server->AccessPaths()->GetPtr(_hostname, "");
	if(HostnameAccessPath) //Use hostname as received
	{
		if(HostnameAccessPath->LanguagePtr)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(HostnameAccessPath->LanguagePtr.id()));
			_LanguageFromHostname = true;
		}
		long long MobileAccessPathId = Configurations()->GetLongInt("MobileAccessPathId", ModulesDatabase::Navigation, -1);
		if(MobileAccessPathId != -1 && HostnameAccessPath.id() == MobileAccessPathId)
		{
			_MobileVersionFromHostname = true;
		}
	}
	else if(_hostname.substr(0, 4) == "www.") //If not found, try without "www."
	{
		_hostname = _hostname.substr(4);
		HostnameAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(_hostname, "");
		if(HostnameAccessPath)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(HostnameAccessPath->LanguagePtr.id()));
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
			if(Server->Languages()->LanguageAcceptExists(LanguageAccept))
			{
				setLocale(Server->Languages()->GetLocaleFromLanguageAccept(LanguageAccept));
			}
			else //If not break up the LanguageAccept and check with wild cards
			{
				std::string LanguageAcceptLookup = LanguageAccept;
				std::string::size_type DashPosition = LanguageAccept.find('-');
				if(DashPosition != std::string::npos) //If there is a country specific language in language accept
				{
					LanguageAcceptLookup = LanguageAccept.substr(0, DashPosition); //Remove everything with and after the dash, "-".
				}

				if(Server->Languages()->LanguageAcceptExists(LanguageAcceptLookup + "*")) //Check if LanguageAccept with wild card exist
				{
					setLocale(Server->Languages()->GetLocaleFromLanguageAccept(LanguageAcceptLookup + "*"));
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

	//Style CSS Stylesheet
	std::string DefaultStyleName = Configurations()->GetStr("DefaultStyleName", ModulesDatabase::Styles, "Default");
	long long DefaultStyleAuthor = Configurations()->GetLongInt("DefaultStyleAuthor", ModulesDatabase::Styles, 1);
	ChangeStyle(Server->Styles()->GetStylePtr(DefaultStyleName, DefaultStyleAuthor));

	//User stylesheet
	useStyleSheet(_UserStyleSheet);

	//TEST//
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
	new Wt::WText(std::string("Mobile Version: "), root());
	auto mvt = new Wt::WText(std::string(IsMobileVersion() ? "On" : "Off"), root());
	MobileVersionChanged().connect(boost::bind<void>([mvt](bool MobileVersion){
		mvt->setText(MobileVersion ? "On" : "Off");
	}, _1));
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
	new Wt::WBreak(root());
	rc->clicked().connect(boost::bind<void>([this, Server](){
		new Wt::WText(std::string("[Server Config; Before Reload()]Log Directory: ") + Server->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WText(std::string("[Cache Config; Before Reload()]Log Directory: ") + Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WBreak(root());
		Server->Configurations()->Reload();
		new Wt::WText(std::string("[Server Config; AFTER Reload()]Log Directory: ") + Server->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WText(std::string("[Cache Config; AFTER Reload()]Log Directory: ") + Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WBreak(root());
	}));

	Wt::WPushButton *rs = new Wt::WPushButton("Reload Styles", root());
	rs->clicked().connect(boost::bind<void>([Server](){
		Server->Styles()->Reload();
	}));
	Wt::WPushButton *cs = new Wt::WPushButton("Change Style", root());
	cs->clicked().connect(boost::bind<void>([this, Server](){
		if(!_CurrentStylePtr)
		{
			ChangeStyle(Server->Styles()->GetStylePtr("Default", 1));
		}
		else if(_CurrentStylePtr.id().Name == "Default")
		{
			ChangeStyle(Server->Styles()->GetStylePtr("test", 1));
		}
		else
		{
			ChangeStyle(Server->Styles()->GetStylePtr("Default", 1));
		}
	}));
	new Wt::WBreak(root());

	//Update Server's active application's mapping
	Server->NewApp(this);

	//Initialization duration
	boost::posix_time::ptime InitEndTime = boost::posix_time::microsec_clock::local_time();
	new Wt::WText(std::string("Application initialization duration: ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_milliseconds())
		+ " ms", root());
	new Wt::WText(std::string("; ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_microseconds())
		+ " µs", root());
}

Application::~Application()
{
	WServer *Server = WServer::instance();
	Server->AppDeleted(this);
}

Application *Application::CreateApplication(const Wt::WEnvironment &env)
{
	return new Application(env);
}

Application *Application::instance()
{
	return dynamic_cast<Application*>(Wt::WApplication::instance());
}

ConfigurationsProxy *Application::Configurations()
{
	return _Configurations;
}

void Application::SetLanguageFromInternalPath()
{
	if(_SkipLanguageInternalPath) //Skip when setInternalPath() was recalled from this function
	{
		_SkipLanguageInternalPath = false;
		return;
	}
	WServer *Server = WServer::instance();
	int IPLM = Configurations()->GetEnum("InternalPathMode", ModulesDatabase::Localization, 3); //Internal path language presentation mode
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
			setInternalPath(Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname), true);
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
	Tokenizer::iterator Itr = Tokens.begin();

	//If first internal path is mobile then check next path
	long long MobileAccessPathId = Configurations()->GetLongInt("MobileAccessPathId", ModulesDatabase::Navigation, -1);
	if(MobileAccessPathId != -1)
	{
		Wt::Dbo::ptr<AccessPath> CheckAccessPath = Server->AccessPaths()->GetPtr(HostName, *Itr);
		if(!CheckAccessPath)
		{
			if(HostName.substr(0, 4) == "www.")
			{
				CheckAccessPath = Server->AccessPaths()->GetPtr(HostName.substr(4), *Itr);
			}
			if(!CheckAccessPath)
			{
				CheckAccessPath = Server->AccessPaths()->GetPtr("", *Itr);
			}
		}
		if(CheckAccessPath.id() == MobileAccessPathId)
		{
			//Emit if MobileVersion just got enabled
			if(IsMobileVersion() == false)
			{
				_MobileVersionChanged.emit(true);
			}
			_MobileVersionFromInternalPath = true;
			++Itr;
			if(Itr == Tokens.end())
			{
				return;
			}
		}
		else
		{
			//Emit if MobileVersion just got disabled
			if(IsMobileVersion() == true && _MobileVersionFromHostname == false)
			{
				_MobileVersionChanged.emit(false);
			}
			_MobileVersionFromInternalPath = false;
		}
	}

	//Set language if the language is correct in internal path
	Wt::Dbo::ptr<AccessPath> LanguageAccessPath;
	if(!Itr->empty() && HostName.substr(0, 4) == "www."
		&& (LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) == 0) //Use hostname without "www." if access path does not exists with the "www."
	{
		HostName = HostName.substr(4);
	}
	std::string LanguageCode;
	bool Found = false;

	if(!Itr->empty() && (LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) != 0)
	{
		LanguageCode = LanguageAccessPath->LanguagePtr.id();
		Found = true;
	}
	//If LanguageAccessPath exists but not if Language is set from hostname and using HostUnspecificLanguage is not allowed(false)
	else if(!Itr->empty() && (LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr)) != 0
		&& (!_LanguageFromHostname || Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false)))
	{
		LanguageCode = LanguageAccessPath->LanguagePtr.id();
		Found = true;
	}

	if(Found)
	{
		if(locale().name() != LanguageCode)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageCode));
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
			std::string CurrentLanguageInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
			if(CurrentLanguageInternalPath == "/")
			{
				CurrentLanguageInternalPath = "";
			}
			//Prepend current language instead of replacing because that path mostly wont be language code
			_SkipLanguageInternalPath = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr),
				Itr->size() + 1,
				CurrentLanguageInternalPath + "/" + *Itr), true);
		}
	}
}

void Application::setLocale(const Wt::WLocale &locale)
{
	Wt::WApplication::setLocale(locale);
	_LocaleChanged.emit();
}

Wt::Signal<void> &Application::LocaleChanged()
{
	return _LocaleChanged;
}

std::string Application::InternalPathAfterReserved(const std::string &after) const
{
	return "";
}

bool Application::IsMobileVersion() const
{
	return _MobileVersionFromHostname ? true : _MobileVersionFromInternalPath;
}

Wt::Signal<bool> &Application::MobileVersionChanged()
{
	return _MobileVersionChanged;
}

Wt::Dbo::ptr<Style> Application::CurrentStyle()
{
	return _CurrentStylePtr;
}

void Application::ChangeStyle(Wt::Dbo::ptr<Style> StylePtr)
{
	if(!StylePtr || _CurrentStylePtr == StylePtr)
	{
		return;
	}
	SetStyle(StylePtr);
}

void Application::SetStyle(Wt::Dbo::ptr<Style> StylePtr)
{
	typedef std::list< Wt::Dbo::ptr<StyleCssRule> > StyleCssRuleList;

	//Remove CSS rules
	WServer *Server = WServer::instance();
	styleSheet().clear();

	//Add CSS rules for new style
	StyleCssRuleList CssRules = Server->Styles()->GetStyleCssRules(StylePtr.id().Name, StylePtr.id().AuthorPtr.id());
	for(StyleCssRuleList::const_iterator itr = CssRules.begin();
		itr != CssRules.end();
		++itr)
	{
		DboCssRule *Rule = new DboCssRule(*itr);
		styleSheet().addRule(Rule);
	}

	_CurrentStylePtr = StylePtr;
	_StyleChanged.emit();
}

Wt::Signal<void> &Application::StyleChanged()
{
	return _StyleChanged;
}

Wt::WCssStyleSheet &Application::UserStyleSheet()
{
	return _UserStyleSheet;
}

void Application::RefreshLocaleStrings()
{
	refresh();
}

void Application::RefreshStyleStrings()
{
	//Refresh Templates, Style WCssStyleSheet, Template WCssStyleSheets
	WServer *Server = WServer::instance();

	//Style CssStyleSheet
	Wt::Dbo::ptr<Style> NewStylePtr = Server->Styles()->GetStylePtr(CurrentStyle().id().Name, CurrentStyle().id().AuthorPtr.id());
	if(!NewStylePtr)
	{
		//Default style is taken from server's active configuration instead of cached configuration because it is possible
		//for the default style to get changed from the configuration and that old default style being changed/deleted
		std::string DefaultStyleName = Server->Configurations()->GetStr("DefaultStyleName", ModulesDatabase::Styles, "Default");
		long long DefaultStyleAuthor = Server->Configurations()->GetLongInt("DefaultStyleAuthor", ModulesDatabase::Styles, 1);
		NewStylePtr = Server->Styles()->GetStylePtr(DefaultStyleName, DefaultStyleAuthor);
	}
	SetStyle(NewStylePtr);

	//Template CssStyleSheets, remove all rules and add new
	typedef std::list< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleList;
	for(TemplateStyleSheetMap::iterator itr = _TemplateStyleSheets.begin();
		itr != _TemplateStyleSheets.end();
		++itr)
	{
		itr->second.clear();
		TemplateCssRuleList CssRules = Server->Styles()->GetTemplateCssRules(itr->first.first, itr->first.second);
		for(TemplateCssRuleList::const_iterator it = CssRules.begin();
			it != CssRules.end();
			++it)
		{
			DboCssRule *Rule = new DboCssRule(*it);
			itr->second.addRule(Rule);
		}
	}

	//Send changes
	triggerUpdate();
}

void Application::RefreshPageStrings()
{

}

void Application::UseTemplateStyleSheet(Wt::Dbo::ptr<Template> TemplatePtr)
{
	typedef std::list< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleList;

	//Ignore if its an empty ptr
	if(!TemplatePtr)
	{
		return;
	}

	//Ignore if the stylesheet is already loaded
	TemplateStyleSheetMap::const_iterator itr = _TemplateStyleSheets.find(std::make_pair(TemplatePtr.id().Name, TemplatePtr.id().ModulePtr.id()));
	if(itr != _TemplateStyleSheets.end())
	{
		return;
	}

	//Create the stylesheet if its not loaded
	WServer *Server = WServer::instance();
	Wt::WCssStyleSheet TemplateStyleSheet;
	
	//Ignore if there are no CSS rules for this template
	TemplateCssRuleList CssRules = Server->Styles()->GetTemplateCssRules(TemplatePtr.id().Name, TemplatePtr.id().ModulePtr.id());
	if(CssRules.empty())
	{
		return;
	}

	for(TemplateCssRuleList::const_iterator itr = CssRules.begin();
		itr != CssRules.end();
		++itr)
	{
		DboCssRule *Rule = new DboCssRule(*itr);
		TemplateStyleSheet.addRule(Rule);
	}

	//Add to template style sheets map and application to be loaded
	_TemplateStyleSheets[std::make_pair(TemplatePtr.id().Name, TemplatePtr.id().ModulePtr.id())] = TemplateStyleSheet;
	useStyleSheet(_TemplateStyleSheets[std::make_pair(TemplatePtr.id().Name, TemplatePtr.id().ModulePtr.id())]);
}

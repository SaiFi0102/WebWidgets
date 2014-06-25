#include "Application/Application.h"
#include "Application/WServer.h"
#include "Objects/DboLocalizedStrings.h"
#include "Objects/DboCssRule.h"

#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationsCache.h"
#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/ReadLock.h"

#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <WebUtils.h>

Application::Application(const Wt::WEnvironment &env)
	: StartTime(boost::posix_time::microsec_clock::local_time()),
	Wt::WApplication(env),
	_LocaleChanged(this), _InternalPathChanged(this), _InternalPathAfterReservedChanged(this), _MobileVersionChanged(this), //Signals
	_LanguageFromHostname(false), _SkipReservedPathInterpretation(false), _ReservedInternalPath("/"), _OldReservedInternalPath("/"), //Localization related bools
	_MobileVersionFromHostname(false), _MobileVersionFromInternalPath(false) //Mobile UI related bools
{
	//Enable server push
	enableUpdates();

	//Cache configurations
	WServer *Server = WServer::instance();
	_Configurations = new ConfigurationsCache(Server->Configurations(), this); //Local copy of configuration ptrs

	//Use database backend localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server));

	//Set Default and Client's environment locale
	_ClientLocale = env.locale();
	Wt::Dbo::ptr<AccessPath> DefaultLanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(Configurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1));
	Wt::WLocale ConfigDefaultLocale;
	if(DefaultLanguageAccessPath && DefaultLanguageAccessPath->LanguagePtr)
	{
		ConfigDefaultLocale = Server->Languages()->GetLocaleFromCode(DefaultLanguageAccessPath->LanguagePtr.id());
	}
	else
	{
		ConfigDefaultLocale = Server->Languages()->GetLocaleFromCode("en");
	}

	//Check if user is using an Hostname AccessPath before checking LanguageAccept for Language
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

	//Connect slots that should run first(must be mutually exclusive)
	Wt::WApplication::internalPathChanged().connect(boost::bind(&Application::HandleWtInternalPathChanged, this));

	//Set language from internal path
	InterpretReservedInternalPath();

	//Style CSS Stylesheet
	std::string DefaultStyleName = Configurations()->GetStr("DefaultStyleName", ModulesDatabase::Styles, "Default");
	long long DefaultStyleAuthor = Configurations()->GetLongInt("DefaultStyleAuthor", ModulesDatabase::Styles, 1);
	ChangeStyle(Server->Styles()->GetStylePtr(DefaultStyleName, DefaultStyleAuthor));

	//User stylesheet
	//useStyleSheet(_UserStyleSheet);

	//TEST UI//
	CreateTestUI();

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

ConfigurationsCache *Application::Configurations() const
{
	return _Configurations;
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

std::string Application::InternalPathAfterReservedNextPart(const std::string &after) const
{
	return internalPathNextPart(_ReservedInternalPath + after);
}

std::string Application::InternalPathAfterReserved() const
{
	return internalSubPath(_ReservedInternalPath);
}

void Application::setInternalPathAfterReserved(const std::string &path, bool emitChange)
{
	//Skip reinterpreting reserved internal path
	if(emitChange)
	{
		_SkipReservedPathInterpretation = true;
	}

	//Set path
	setInternalPath(_ReservedInternalPath + path, emitChange);
	if(emitChange)
	{
		_InternalPathAfterReservedChanged.emit(path);
	}
}

std::string Application::InternalPathReserved() const
{
	return _ReservedInternalPath;
}

Wt::Signal<std::string> &Application::internalPathAfterReservedChanged()
{
	return _InternalPathAfterReservedChanged;
}

Wt::Signal<std::string> &Application::internalPathChanged()
{
	return _InternalPathChanged;
}

bool Application::IsMobileVersion() const
{
	return _MobileVersionFromHostname || _MobileVersionFromInternalPath;
}

Wt::Signal<bool> &Application::MobileVersionChanged()
{
	return _MobileVersionChanged;
}

Wt::Dbo::ptr<Style> Application::CurrentStyle() const
{
	return _CurrentStylePtr;
}

void Application::ChangeStyle(Wt::Dbo::ptr<Style> StylePtr)
{
	if(!StylePtr || StylePtr.isTransient() || _CurrentStylePtr.id() == StylePtr.id())
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
	refresh(); //To reload styletemplates
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
	triggerUpdate();
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
			itr->second.addRule(new DboCssRule(*it));
		}
	}

	//Send changes
	refresh();
	triggerUpdate();
}

void Application::RefreshPageStrings()
{
	//...
	triggerUpdate();
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

void Application::HandleWtInternalPathChanged()
{
	//Call events in order
	InterpretReservedInternalPath();

	//Emit our version of internalPathChanged()
	internalPathChanged().emit(internalPath());

	//Check if internal path after reserved changed
	std::string NewReservedInternalPath = InternalPathAfterReserved();
	if(Wt::Utils::append(oldInternalPath_, '/').substr(_OldReservedInternalPath.size()) != NewReservedInternalPath)
	{
		internalPathAfterReservedChanged().emit(NewReservedInternalPath);
	}
}

void Application::InterpretReservedInternalPath()
{
	//Skip when setInternalPath() was recalled from this function OR when reserved internal path was not changed
	if(_SkipReservedPathInterpretation)
	{
		_SkipReservedPathInterpretation = false;
		return;
	}

	//Internal path language presentation mode
	int IPLM = Configurations()->GetEnum("InternalPathMode", ModulesDatabase::Localization, 3);
	if(environment().agentIsSpiderBot() && (IPLM == 3 || IPLM == 4)) //Change NoRestriction modes to AlwaysShowHideDef for bots
	{
		IPLM = 2;
	}

	//Old reserved internal path
	_OldReservedInternalPath = _ReservedInternalPath;

	switch(IPLM)
	{
		case 1:
			IRIPAlwaysShow();
		break;
		case 2:
			IRIPAlwaysShowHideDef();
		break;
		case 3:
			IRIPNoRestrictionHideDef();
		break;
		case 4:
			IRIPNoRestriction();
		break;
	}

	//Reserved internal path should not be empty
	if(_ReservedInternalPath.empty())
	{
		_ReservedInternalPath = "/";
	}
}

bool Application::IRIPMobileVersion(const std::string &HostName, const std::string &Path)
{
	WServer *Server = WServer::instance();
	long long MobileAccessPathId = Configurations()->GetLongInt("MobileAccessPathId", ModulesDatabase::Navigation, -1);

	if(MobileAccessPathId != -1)
	{
		//Check all possible mobile access paths
		Wt::Dbo::ptr<AccessPath> MobileCheckAccessPath = Server->AccessPaths()->GetPtr(HostName, Path);
		if(!MobileCheckAccessPath)
		{
			if(HostName.substr(0, 4) == "www.")
			{
				MobileCheckAccessPath = Server->AccessPaths()->GetPtr(HostName.substr(4), Path);
			}
			if(!MobileCheckAccessPath)
			{
				MobileCheckAccessPath = Server->AccessPaths()->GetPtr("", Path);
			}
		}

		//Check if the path is a mobile access path
		if(MobileCheckAccessPath.id() == MobileAccessPathId)
		{
			//Emit if MobileVersion just got enabled
			if(IsMobileVersion() == false)
			{
				_MobileVersionChanged.emit(true);
			}
			_MobileVersionFromInternalPath = true;
			_ReservedInternalPath += "/" + Path; //Add mobile access path to reserved path
			return true;
		}
		else
		{
			//Emit if MobileVersion just got disabled
			if(IsMobileVersion() == true && _MobileVersionFromHostname == false)
			{
				_MobileVersionChanged.emit(false);
			}
			_MobileVersionFromInternalPath = false;
			return false;
		}
	}
	return false;
}

void Application::IRIPAlwaysShow()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	if(InternalPath == "/")
	{
		_SkipReservedPathInterpretation = true;
		_ReservedInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
		setInternalPath(_ReservedInternalPath, true);
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();
	_ReservedInternalPath = ""; //Reset reserved internal path

	//Check mobile version
	if(IRIPMobileVersion(HostName, *Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Wt::Dbo::ptr<AccessPath> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr))
			&& HostName.substr(0, 4) == "www.") //Use hostname without "www." if access path does not exists with the "www."
		{
			HostName = HostName.substr(4);
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr); //Check again if not found(without www.)
		}
		if(!LanguageAccessPath //Check again if STILL not found(without hostname)
			&& (!_LanguageFromHostname || Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false))) //But not if language is set from hostname and using HostUnspecificLanguage is not allowed(false)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguagePtr.id())
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguagePtr.id()));
		}
		_ReservedInternalPath += "/" + *Itr; //Add language internal path to reserved
	}
	else //Not found
	{
		std::string CurrentLanguageInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
		if(CurrentLanguageInternalPath == "/")
		{
			CurrentLanguageInternalPath = "";
		}
		//Prepend current language instead of replacing because probably that path wont be a language code
		_SkipReservedPathInterpretation = true;
		setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr),
			Itr->size() + 1,
			CurrentLanguageInternalPath + "/" + *Itr), true);

		//Add language internal path to reserved
		_ReservedInternalPath += "/" + CurrentLanguageInternalPath;
	}
}

void Application::IRIPAlwaysShowHideDef()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	if(InternalPath == "/")
	{
		if(locale().name() != _SessionDefaultLocale.name())
		{
			_ReservedInternalPath = "/";
			setLocale(_SessionDefaultLocale);
		}
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();
	_ReservedInternalPath = ""; //Reset reserved internal path

	//Check mobile version
	if(IRIPMobileVersion(HostName, *Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Wt::Dbo::ptr<AccessPath> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr))
			&& HostName.substr(0, 4) == "www.") //Use hostname without "www." if access path does not exists with the "www."
		{
			HostName = HostName.substr(4);
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr); //Check again if not found(without www.)
		}
		if(!LanguageAccessPath //Check again if STILL not found(without hostname)
			&& (!_LanguageFromHostname || Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false))) //But not if language is set from hostname and using HostUnspecificLanguage is not allowed(false)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguagePtr.id())
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguagePtr.id()));
		}
		//Remove default language from internal path if its there
		if(locale().name() == _SessionDefaultLocale.name())
		{
			_SkipReservedPathInterpretation = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr), Itr->size() + 1, ""), true);
		}
		else
		{
			_ReservedInternalPath += "/" + *Itr; //Add language internal path to reserved
		}
	}
	else //Not found
	{
		if(locale().name() != _SessionDefaultLocale.name()) //if set to hide default and if current language is NOT default
		{
			std::string CurrentLanguageInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
			if(CurrentLanguageInternalPath == "/")
			{
				CurrentLanguageInternalPath = "";
			}
			//Prepend current language instead of replacing because probably that path wont be a language code
			_SkipReservedPathInterpretation = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr),
				Itr->size() + 1,
				CurrentLanguageInternalPath + "/" + *Itr), true);

			//Add language internal path to reserved
			_ReservedInternalPath += "/" + CurrentLanguageInternalPath;
		}
	}
}

void Application::IRIPNoRestrictionHideDef()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	if(InternalPath == "/")
	{
		_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();
	_ReservedInternalPath = ""; //Reset reserved internal path

	//Check mobile version
	if(IRIPMobileVersion(HostName, *Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Wt::Dbo::ptr<AccessPath> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr))
			&& HostName.substr(0, 4) == "www.") //Use hostname without "www." if access path does not exists with the "www."
		{
			HostName = HostName.substr(4);
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr); //Check again if not found(without www.)
		}
		if(!LanguageAccessPath //Check again if STILL not found(without hostname)
			&& (!_LanguageFromHostname || Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false))) //But not if language is set from hostname and using HostUnspecificLanguage is not allowed(false)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguagePtr.id())
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguagePtr.id()));
		}
		//Remove default language automatically from internal path if locale is default language
		if(locale().name() == _SessionDefaultLocale.name())
		{
			_SkipReservedPathInterpretation = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr), Itr->size() + 1, ""), true);
		}
		else
		{
			_ReservedInternalPath += "/" + *Itr; //Add language internal path to reserved
		}
	}
}

void Application::IRIPNoRestriction()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	if(InternalPath == "/")
	{
		_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();
	_ReservedInternalPath = ""; //Reset reserved internal path

	//Check mobile version
	if(IRIPMobileVersion(HostName, *Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Wt::Dbo::ptr<AccessPath> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr))
			&& HostName.substr(0, 4) == "www.") //Use hostname without "www." if access path does not exists with the "www."
		{
			HostName = HostName.substr(4);
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr); //Check again if not found(without www.)
		}
		if(!LanguageAccessPath //Check again if STILL not found(without hostname)
			&& (!_LanguageFromHostname || Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false))) //But not if language is set from hostname and using HostUnspecificLanguage is not allowed(false)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguagePtr.id())
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguagePtr.id()));
		}
		_ReservedInternalPath += "/" + *Itr; //Add language internal path to reserved
	}
}

void Application::CreateTestUI()
{
	//TEST//
	WServer *Server = WServer::instance();

	new Wt::WText("HI", root());
	new Wt::WBreak(root());
	(new Wt::WText("Internal path: ", root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto ip = new Wt::WText(internalPath(), root());
	new Wt::WBreak(root());
	(new Wt::WText("Reserved internal path: ", root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto ipr = new Wt::WText(InternalPathReserved(), root());
	new Wt::WBreak(root());
	(new Wt::WText("Internal path after reserve(subpath): ", root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto ipar = new Wt::WText(InternalPathAfterReserved(), root());
	new Wt::WBreak(root());
	(new Wt::WText("Internal path after reserve next part(Arg '/'): ", root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto iparnp = new Wt::WText(InternalPathAfterReservedNextPart("/"), root());
	new Wt::WBreak(root());
	(new Wt::WText("Current language: ", root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto txt = new Wt::WText(locale().name(), root());
	internalPathChanged().connect(boost::bind<void>([this, ip, ipr](){
		ip->setText(internalPath());
		ipr->setText(InternalPathReserved());
	}));
	internalPathAfterReservedChanged().connect(boost::bind<void>([this, ipar, iparnp](){
		ipar->setText(InternalPathAfterReserved());
		iparnp->setText(InternalPathAfterReservedNextPart("/"));
	}));
	LocaleChanged().connect(boost::bind<void>([this, txt](){
		txt->setText(locale().name());
	}));
	new Wt::WBreak(root());
	(new Wt::WText(std::string("Session Default Language: ") + _SessionDefaultLocale.name(), root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	new Wt::WBreak(root());
	(new Wt::WText(std::string("Mobile Version: "), root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
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
	Wt::WPushButton *rl = new Wt::WPushButton("Reload languages", root());
	rl->clicked().connect(boost::bind<void>([Server](){
		Server->Languages()->Reload();
	}));
	new Wt::WBreak(root());
	new Wt::WText(Wt::WString::tr("Wt.Auth.email"), root());
	new Wt::WBreak(root());
	new Wt::WText(Wt::WString::tr("test", ModulesDatabase::Authentication), root());
	new Wt::WBreak(root());
	new Wt::WBreak(root());
	new Wt::WTemplate(Wt::WString::tstr("styletpl", ModulesDatabase::Styles), root());
	new Wt::WTemplate(Wt::WString::tstr("templatetpl", ModulesDatabase::Styles), root());
}

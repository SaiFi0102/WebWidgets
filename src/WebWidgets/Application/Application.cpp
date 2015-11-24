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
#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/NavigationMenusDatabase.h"
#include "DboDatabase/ReadLock.h"

#include "Widgets/PagesMenu.h"
#include "Widgets/PagesStackWidget.h"
#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <Wt/WResource>
#include <Wt/WStackedWidget>
#include <Wt/WMenu>
#include <WebUtils.h>

//From https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c/4654718#4654718
bool is_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while(it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

Application::Application(const Wt::WEnvironment &env)
	: StartTime(boost::posix_time::microsec_clock::local_time()),
	Wt::WApplication(env),
	_LocaleChanged(this), _InternalPathChanged(this), _InternalPathAfterReservedChanged(this), _MobileVersionChanged(this), //Signals
	_PageChanged(this), _ReservedInternalPathChanged(this), //Signals
	_LanguageFromHostname(false), _SkipReservedPathInterpretation(false), _ReservedInternalPath("/"), _OldReservedInternalPath("/"), //Localization
	_MobileVersionFromHostname(false), _MobileVersionFromInternalPath(false), //Mobile UI
	_MainTemplate(0), _PageStack(0), _PageMenu(0) //Widgets
{
	//Enable server push
	enableUpdates();

	//Cache configurations
	WServer *Server = WServer::instance();
	_Configurations = new ConfigurationsCache(Server->Configurations()); //Local copy of configuration ptrs

	//Set AccessHostName and its default settings
	SetAccessHostNameDefaults();
	setLocale(_SessionDefaultLocale); //Default language
	SetStyle(_DefaultStylePtr); //Default style

	//Use database backend localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server));

	//Internal paths
	setInternalPathDefaultValid(true);

	//Connect slots that should run first
	Wt::WApplication::internalPathChanged().connect(std::bind(&Application::HandleWtInternalPathChanged, this));

	//Set language from internal path
	InterpretReservedInternalPath();

	//Main stack widget
	_PageStack = new PagesStackWidget();

	//Main navigation menu
	_PageMenu = new PagesMenu(1);
	_PageMenu->setInternalPathEnabled();
	_PageMenu->itemSelected().connect(std::bind(&Application::InterpretPageInternalPath, this));

	//Main template widget
	_MainTemplate = new Wt::WTemplate(Wt::WString::tstr("main", ModulesDatabase::Styles), root());
	_MainTemplate->bindWidget("page-content", _PageStack);
	_MainTemplate->bindWidget("navigation", _PageMenu);

	//Page change signal/slot and home page
	internalPathAfterReservedChanged().connect(std::bind(&Application::InterpretPageInternalPath, this));
	InterpretPageInternalPath();

	//User stylesheet
	//useStyleSheet(_UserStyleSheet);

	//Initialization duration
	boost::posix_time::ptime InitEndTime = boost::posix_time::microsec_clock::local_time();
	new Wt::WText(std::string("Application initialization duration: ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_milliseconds())
		+ " ms", root());
	new Wt::WText(std::string("; ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_microseconds())
		+ " µs", root());
	new Wt::WBreak(root());
}

Application::~Application()
{
	delete _Configurations;
}

void Application::SetAccessHostNameDefaults()
{
	WServer *Server = WServer::instance();
	ReadLock Lock(Server->DatabaseManager());

	//Set AccessHostNames
	std::string _hostname = environment().hostName();
	_GlobalAccessHost = Server->AccessPaths()->AccessHostNamePtr("");
	_AccessHostName = Server->AccessPaths()->AccessHostOrGlobalPtr(_hostname);

	//Home page
	if(_AccessHostName->DefaultPageId == -1)
	{
		if(_GlobalAccessHost->DefaultPageId != -1)
		{
			_HomePagePtr = Server->Pages()->GetPtr(_GlobalAccessHost->DefaultPageId);
		}
		else
		{
			_HomePagePtr = 0;
		}
	}
	else
	{
		_HomePagePtr = Server->Pages()->GetPtr(_AccessHostName->DefaultPageId);
	}

	//Default style
	if(_AccessHostName->StyleId == -1)
	{
		if(_GlobalAccessHost->StyleId == -1)
		{
			_DefaultStylePtr = Server->Styles()->FirstStylePtr();
		}
		else
		{
			_DefaultStylePtr = Server->Styles()->GetStylePtr(_GlobalAccessHost->StyleId);
		}
	}
	else
	{
		_DefaultStylePtr = Server->Styles()->GetStylePtr(_AccessHostName->StyleId);
	}

	//Language and Mobile Mode
	_ClientLocale = environment().locale();
	std::string DefaultLanguageCode = Server->Languages()->DefaultLanguageCode(_hostname);
	_LanguageFromHostname = false;
	if(_AccessHostName != _GlobalAccessHost)
	{
		//Check if user is using Hostname AccessPath before checking LanguageAccept for Language
		if(!_AccessHostName->LanguageCode.empty())
		{
			_SessionDefaultLocale = Server->Languages()->GetLocaleFromCode(_AccessHostName->LanguageCode, _hostname);
			_LanguageFromHostname = true;
		}

		//Mobile mode
		bool omv = IsMobileVersion();
		_MobileVersionFromHostname = _AccessHostName->MobileMode;
		bool nmv = IsMobileVersion();
		if(omv != nmv)
		{
			MobileVersionChanged().emit(nmv);
		}
	}

	//Set environment locale in Application if language was not found in AccessHostName
	if(!_LanguageFromHostname)
	{
		std::string LanguageAccept = environment().locale().name();
		if(LanguageAccept.empty()) //If no language accept http header found, just set locale from default language
		{
			_SessionDefaultLocale = Server->Languages()->GetLocaleFromCode(DefaultLanguageCode, _hostname);
		}
		else
		{
			if(Server->Languages()->LanguageAcceptExists(LanguageAccept))
			{
				_SessionDefaultLocale = Server->Languages()->GetLocaleFromLanguageAccept(LanguageAccept, _hostname);
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
					_SessionDefaultLocale = Server->Languages()->GetLocaleFromLanguageAccept(LanguageAcceptLookup + "*", _hostname);
				}
				else //else set locale from default language
				{
					_SessionDefaultLocale = Server->Languages()->GetLocaleFromCode(DefaultLanguageCode, _hostname);
				}
			}
		}
	}
}

void Application::setLocale(const Wt::WLocale &locale)
{
	Wt::WApplication::setLocale(locale);
	_LocaleChanged.emit();
}

std::string Application::InternalPathAfterReservedNextPart(const std::string &after) const
{
	if(_ReservedInternalPath == "/")
	{
		return internalPathNextPart(after);
	}
	return internalPathNextPart(_ReservedInternalPath + after);
}

std::string Application::InternalPathAfterReserved() const
{
	return Wt::Utils::prepend((internalSubPath(_ReservedInternalPath)), '/');
}

std::string Application::OldInternalPathAfterReserved() const
{
	std::string current = Wt::Utils::append(oldInternalPath_, '/');

	if(!pathMatches(current, _OldReservedInternalPath))
	{
		return std::string();
	}
	return current.substr(_OldReservedInternalPath.length());
}

void Application::setInternalPathAfterReserved(const std::string &path, bool emitChange)
{
	//Set path
	if(_ReservedInternalPath == "/")
	{
		setInternalPath(path, false);
	}
	else
	{
		setInternalPath(_ReservedInternalPath + path, false);
	}
	if(emitChange)
	{
		_InternalPathAfterReservedChanged.emit(path);
	}
}

void Application::ChangeStyle(const std::string &StyleName, long long AuthorId)
{
	WServer *Server = WServer::instance();
	std::shared_ptr<const StyleData> StylePtr = Server->Styles()->GetStylePtr(StyleName, AuthorId);
	if(!StylePtr)
	{
		return;
	}
	if(_CurrentStylePtr && _CurrentStylePtr->id() == StylePtr->id())
	{
		return;
	}

	SetStyle(StylePtr);
	refresh(); //To reload styletemplates
}

void Application::SetStyle(std::shared_ptr<const StyleData> StylePtr)
{
	//Remove CSS rules
	styleSheet().clear();

	//No style
	if(!StylePtr)
	{
		_CurrentStylePtr = 0;
		_StyleChanged.emit();
		return;
	}

	//Add CSS rules for new style
	WServer *Server = WServer::instance();
	for(StyleData::StyleCssRuleSet::const_iterator itr = StylePtr->StyleCssRules.begin();
		itr != StylePtr->StyleCssRules.end();
		++itr)
	{
		DboCssRule *Rule = new DboCssRule(*itr, this);
		styleSheet().addRule(Rule);
	}

	_CurrentStylePtr = StylePtr;
	_StyleChanged.emit();
}

void Application::SetPage(std::shared_ptr<const PageData> PagePtr)
{
	WServer *Server = WServer::instance();
	_CurrentPagePtr = PagePtr;

	AbstractPage *PageHandler = Server->Pages()->GetPage(_CurrentPagePtr ? _CurrentPagePtr->id() : -1);
	if(!_CurrentPagePtr || !PageHandler)
	{
		setInternalPathValid(false);
		_PageChanged.emit();
		return;
	}
	
	_PageChanged.emit();
}

void Application::RefreshDboDatabasePtrs()
{
	Application *app = Application::instance();
	if(!app)
	{
		return;
	}

	app->SetAccessHostNameDefaults();

	WServer *Server = WServer::instance();
	ReadLock Lock(Server->DatabaseManager());
	//Refresh Templates, Style WCssStyleSheet, Template WCssStyleSheets
	//Style CssStyleSheet
	std::shared_ptr<const StyleData> NewStylePtr;
	if(app->CurrentStyle())
	{
		NewStylePtr = Server->Styles()->GetStylePtr(app->CurrentStyle()->id());
		if(!NewStylePtr)
		{
			NewStylePtr = app->_DefaultStylePtr;
		}
	}
	else
	{
		NewStylePtr = app->_DefaultStylePtr;
	}
	app->SetStyle(NewStylePtr);

	//Template CssStyleSheets, remove all rules and add new
	typedef std::set< std::shared_ptr<const TemplateCssRuleData> > TemplateCssRuleList;
	for(TemplateStyleSheetMap::iterator itr = app->_TemplateStyleSheets.begin();
		itr != app->_TemplateStyleSheets.end();
		++itr)
	{
		itr->second.clear();
		std::shared_ptr<const TemplateData> TemplatePtr = Server->Styles()->GetTemplatePtr(itr->first.first, itr->first.second);
		if(!TemplatePtr)
		{
			continue;
		}
		for(TemplateData::TemplateCssRuleSet::const_iterator it = TemplatePtr->TemplateCssRules.begin();
			it != TemplatePtr->TemplateCssRules.end();
			++it)
		{
			itr->second.addRule(new DboCssRule(*it, app));
		}
	}

	app->InterpretPageInternalPath();
	//Hacky way to check if locale was changed by InterpretReservedInternalPath()
	//refresh() is called from setLocale() in InterpretReservedInternalPath()
	const Wt::WLocale *locptr = &app->locale();
	app->InterpretReservedInternalPath();
	if(locptr == &app->locale())
	{
		app->setLocale(app->_SessionDefaultLocale);
	}

	app->triggerUpdate();
}

void Application::UseTemplateStyleSheet(std::shared_ptr<const TemplateData> TemplatePtr)
{
	typedef std::set< std::shared_ptr<const TemplateCssRuleData> > TemplateCssRuleList;

	//Ignore if its an empty ptr
	if(!TemplatePtr)
	{
		return;
	}

	//Ignore if the stylesheet is already loaded
	if(_TemplateStyleSheets.find(std::make_pair(TemplatePtr->Name(), TemplatePtr->ModuleId())) != _TemplateStyleSheets.end())
	{
		return;
	}

	//Create the stylesheet if its not loaded
	WServer *Server = WServer::instance();
	Wt::WCssStyleSheet TemplateStyleSheet;
	
	//Ignore if there are no CSS rules for this template
	if(TemplatePtr->TemplateCssRules.empty())
	{
		return;
	}

	for(TemplateCssRuleList::const_iterator itr = TemplatePtr->TemplateCssRules.begin();
		itr != TemplatePtr->TemplateCssRules.end();
		++itr)
	{
		DboCssRule *Rule = new DboCssRule(*itr, this);
		TemplateStyleSheet.addRule(Rule);
	}

	//Add to template style sheets map and application to be loaded
	_TemplateStyleSheets[std::make_pair(TemplatePtr->Name(), TemplatePtr->ModuleId())] = TemplateStyleSheet;
	useStyleSheet(_TemplateStyleSheets[std::make_pair(TemplatePtr->Name(), TemplatePtr->ModuleId())]);
}

void Application::HandleWtInternalPathChanged()
{
	//Call events in order
	InterpretReservedInternalPath();

	//Emit our version of internalPathChanged()
	internalPathChanged().emit(internalPath());

	//Check if internal path after reserved changed
	if(InternalPathAfterReserved() != OldInternalPathAfterReserved())
	{
		internalPathAfterReservedChanged().emit(InternalPathAfterReserved());
	}

	if(_OldReservedInternalPath != _ReservedInternalPath)
	{
		reservedInternalPathChanged().emit(ReservedInternalPath());
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
	_ReservedInternalPath = "/";

	WServer *Server = WServer::instance();
	ReadLock Lock(Server->DatabaseManager());
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

	if(_OldReservedInternalPath != _ReservedInternalPath)
	{
		_PageMenu->setInternalBasePath(_ReservedInternalPath);
	}
}

bool Application::IRIPMobileVersion(const std::string &Path)
{
	//If AccessHostName is not global hostname and AccessHostName mobile mode is on no need to check internal paths
	std::string MobileInternalPath = _AccessHostName->MobileInternalPath;
	if(_AccessHostName != _GlobalAccessHost)
	{
		if(_MobileVersionFromHostname)
		{
			return false;
		}
		if(MobileInternalPath.empty())
		{
			MobileInternalPath = _GlobalAccessHost->MobileInternalPath;
		}
	}
	if(MobileInternalPath.empty())
	{
		return false;
	}

	if(Path == MobileInternalPath)
	{
		_ReservedInternalPath += Path; //Add mobile access path to reserved path
		//Emit if MobileVersion just got enabled
		bool omv = IsMobileVersion();
		_MobileVersionFromInternalPath = true;
		if(omv == false)
		{
			_MobileVersionChanged.emit(true);
		}
		return true;
	}
	else
	{
		//Emit if MobileVersion just got disabled
		bool omv = IsMobileVersion();
		_MobileVersionFromInternalPath = false;
		if(omv == true)
		{
			_MobileVersionChanged.emit(false);
		}
		return false;
	}
}

void Application::IRIPAlwaysShow()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string HostName = environment().hostName();

	if(InternalPath == "/")
	{
		_ReservedInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
		_SkipReservedPathInterpretation = true;
		setInternalPath(_ReservedInternalPath, true);
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	std::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) //Check again if not found(without hostname)
			&& !_LanguageFromHostname)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguageCode)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguageCode, HostName));
		}
		if(_ReservedInternalPath != "/")
		{
			_ReservedInternalPath += "/";
		}
		_ReservedInternalPath += *Itr; //Add language internal path to reserved
	}
	else //Not found
	{
		std::string CurrentLanguageInternalPath = Server->AccessPaths()->FirstInternalPath(locale().name(), HostName, _LanguageFromHostname);
		if(_ReservedInternalPath == "/")
		{
			_ReservedInternalPath = "";
		}
		_ReservedInternalPath += CurrentLanguageInternalPath; //Add language internal path to reserved
		if(CurrentLanguageInternalPath == "/")
		{
			CurrentLanguageInternalPath = "";
		}
		//Prepend current language instead of replacing because probably that path wont be a language code
		_SkipReservedPathInterpretation = true;
		setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr),
			Itr->size() + 1,
			CurrentLanguageInternalPath + "/" + *Itr), true);
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
			//_ReservedInternalPath = "/";
			setLocale(_SessionDefaultLocale);
		}
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	std::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) //Check again if not found(without hostname)
			&& !_LanguageFromHostname)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguageCode)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguageCode, HostName));
		}
		//Remove default language from internal path if its there
		if(locale().name() == _SessionDefaultLocale.name())
		{
			_SkipReservedPathInterpretation = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr), Itr->size() + 1, ""), true);
		}
		else
		{
			if(_ReservedInternalPath != "/")
			{
				_ReservedInternalPath += "/";
			}
			_ReservedInternalPath += *Itr; //Add language internal path to reserved
		}
	}
	else //Not found
	{
		if(locale().name() != _SessionDefaultLocale.name()) //if set to hide default and if current language is NOT default
		{
			setLocale(_SessionDefaultLocale);
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
		//_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	std::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) //Check again if not found(without hostname)
			&& !_LanguageFromHostname)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguageCode)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguageCode, HostName));
		}
		//Remove default language automatically from internal path if locale is default language
		if(locale().name() == _SessionDefaultLocale.name())
		{
			std::string NewPath = std::string(InternalPath).replace(InternalPath.find(std::string("/") + *Itr), Itr->size() + 1, "");
			if(NewPath.empty())
			{
				NewPath = "/";
			}
			_SkipReservedPathInterpretation = true;
			setInternalPath(NewPath, true);
		}
		else
		{
			if(_ReservedInternalPath != "/")
			{
				_ReservedInternalPath += "/";
			}
			_ReservedInternalPath += *Itr; //Add language internal path to reserved
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
		//_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);
	Tokenizer::iterator Itr = Tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*Itr))
	{
		++Itr;
		if(Itr == Tokens.end())
		{
			return;
		}
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	std::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
	if(!Itr->empty())
	{
		if(!(LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr(HostName, *Itr)) //Check again if not found(without hostname)
			&& !_LanguageFromHostname)
		{
			LanguageAccessPath = Server->AccessPaths()->LanguageAccessPathPtr("", *Itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(LanguageAccessPath) //Found
	{
		if(locale().name() != LanguageAccessPath->LanguageCode)
		{
			setLocale(Server->Languages()->GetLocaleFromCode(LanguageAccessPath->LanguageCode, HostName));
		}
		if(_ReservedInternalPath != "/")
		{
			_ReservedInternalPath += "/";
		}
		_ReservedInternalPath += *Itr; //Add language internal path to reserved
	}
}

void Application::InterpretPageInternalPath()
{
	WServer *Server = WServer::instance();
	std::string InternalPath = InternalPathAfterReserved();
	std::string HostName = environment().hostName();

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> Sep("/");
	Tokenizer Tokens(InternalPath, Sep);

	ReadLock Lock(Server->DatabaseManager());

	//Check if internal path includes page access path
	std::shared_ptr<const PageAccessPathData> PageAccessPathPtr, ParentAccessPathPtr;
	for(Tokenizer::iterator Itr = Tokens.begin();
		Itr != Tokens.end();
		++Itr)
	{
		ParentAccessPathPtr = PageAccessPathPtr;
		long long ParentAccessPathId = ParentAccessPathPtr ? ParentAccessPathPtr->id() : -1;
		if(!(PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr(HostName, *Itr, ParentAccessPathId)))
		{
			PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr("", *Itr, ParentAccessPathId);
		}
		if(!PageAccessPathPtr && ParentAccessPathPtr)
		{
			if(is_number(*Itr) && !(PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr(HostName, "#", ParentAccessPathId)))
			{
				PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr("", "#", ParentAccessPathId);
			}
			else if(!(PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr(HostName, "?", ParentAccessPathId)))
			{
				PageAccessPathPtr = Server->AccessPaths()->PageAccessPathPtr("", "?", ParentAccessPathId);
			}
		}
		if(!PageAccessPathPtr)
		{
			PageAccessPathPtr = 0;
			break;
		}
	}
	_PageAccessPathPtr = PageAccessPathPtr;

	std::shared_ptr<const PageData> PagePtr;
	if(PageAccessPathPtr)
	{
		PagePtr = Server->Pages()->GetPtr(PageAccessPathPtr->PageId);
	}
	else if(InternalPath == "/") //If still not found use the default homepage if user is on homepage
	{
		PagePtr = _HomePagePtr;
	}

	if(PagePtr)
	{
		if(!_CurrentPagePtr || PagePtr->id() != _CurrentPagePtr->id())
		{
			SetPage(PagePtr);
		}
	}
	else
	{
		SetPage(0);
	}
}

Wt::Signal<std::string> &Application::internalPathChanged()
{
	Wt::WApplication::internalPathChanged();
	return _InternalPathChanged;
}

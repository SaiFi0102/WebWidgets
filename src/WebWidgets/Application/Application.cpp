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
#include "DboDatabase/ReadLock.h"

#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <Wt/WResource>
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
	_LocaleChanged(this), _InternalPathChanged(this), _InternalPathAfterReservedChanged(this), _MobileVersionChanged(this), _PageChanged(this), //Signals
	_LanguageFromHostname(false), _SkipReservedPathInterpretation(false), _ReservedInternalPath("/"), _OldReservedInternalPath("/"), //Localization
	_MobileVersionFromHostname(false), _MobileVersionFromInternalPath(false) //Mobile UI
{
	//Enable server push
	enableUpdates();

	//Cache configurations
	WServer *Server = WServer::instance();
	_Configurations = new ConfigurationsCache(Server->Configurations()); //Local copy of configuration ptrs

	//Use database backend localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(Server));

	//Set AccessHostNames
	std::string _hostname = env.hostName();
	{
		ReadLock AccessPathLock(Server->AccessPaths()); //So that both ptrs are from the same state
		_GlobalAccessHost = Server->AccessPaths()->AccessHostNamePtr("");
		_AccessHostName = Server->AccessPaths()->AccessHostOrGlobalPtr(_hostname);
	}

	//Set Default and Client's environment locale
	ReadLock LanguagesLock(Server->Languages());
	_ClientLocale = env.locale();
	std::string DefaultLanguageCode = Server->Languages()->DefaultLanguageCode(_hostname);

	//Check if user is using an Hostname AccessPath before checking LanguageAccept for Language
	if(_AccessHostName != _GlobalAccessHost)
	{
		if(!_AccessHostName->LanguageCode.empty())
		{
			setLocale(Server->Languages()->GetLocaleFromCode(_AccessHostName->LanguageCode, _hostname));
			_LanguageFromHostname = true;
		}
		_MobileVersionFromHostname = _AccessHostName->MobileMode;
	}
	
	//Set environment locale in Application if language was not found in access path
	if(!_LanguageFromHostname)
	{
		std::string LanguageAccept = env.locale().name();
		if(LanguageAccept.empty()) //If no language accept http header found, just set locale from default language
		{
			setLocale(Server->Languages()->GetLocaleFromCode(DefaultLanguageCode, _hostname));
		}
		else
		{
			if(Server->Languages()->LanguageAcceptExists(LanguageAccept))
			{
				setLocale(Server->Languages()->GetLocaleFromLanguageAccept(LanguageAccept, _hostname));
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
					setLocale(Server->Languages()->GetLocaleFromLanguageAccept(LanguageAcceptLookup + "*", _hostname));
				}
				else //else set locale from default language
				{
					setLocale(Server->Languages()->GetLocaleFromCode(DefaultLanguageCode, _hostname));
				}
			}
		}
	}
	LanguagesLock.Unlock();
	_SessionDefaultLocale = locale();

	//Internal paths
	setInternalPathDefaultValid(true);

	//Connect slots that should run first
	Wt::WApplication::internalPathChanged().connect(boost::bind(&Application::HandleWtInternalPathChanged, this));

	//Set language from internal path
	InterpretReservedInternalPath();

	//Page change signal/slot and home page
	if(_AccessHostName->DefaultPageId == -1)
	{
		if(_GlobalAccessHost->DefaultPageId != -1)
		{
			_HomePagePtr = Server->Pages()->GetPtr(_GlobalAccessHost->DefaultPageId);
		}
	}
	else
	{
		_HomePagePtr = Server->Pages()->GetPtr(_AccessHostName->DefaultPageId);
	}
	internalPathAfterReservedChanged().connect(boost::bind(&Application::InterpretPageInternalPath, this));
	InterpretPageInternalPath();

	//Style CSS Stylesheet
	std::string DefaultStyleName = Configurations()->GetStr("DefaultStyleName", ModulesDatabase::Styles, "Default");
	long long DefaultStyleAuthor = Configurations()->GetLongInt("DefaultStyleAuthor", ModulesDatabase::Styles, 1);
	boost::shared_ptr<const StyleData> StylePtr = Server->Styles()->GetStylePtr(DefaultStyleName, DefaultStyleAuthor);
	SetStyle(StylePtr);

	//User stylesheet
	//useStyleSheet(_UserStyleSheet);

	//TEST UI//
	CreateTestUI();

	//Initialization duration
	boost::posix_time::ptime InitEndTime = boost::posix_time::microsec_clock::local_time();
	new Wt::WText(std::string("Application initialization duration: ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_milliseconds())
		+ " ms", root());
	new Wt::WText(std::string("; ")
		+ boost::lexical_cast<std::string>((InitEndTime - StartTime).total_microseconds())
		+ " �s", root());
	new Wt::WBreak(root());
}

Application::~Application()
{
	delete _Configurations;
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
	boost::shared_ptr<const StyleData> StylePtr = Server->Styles()->GetStylePtr(StyleName, AuthorId);
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

void Application::SetStyle(boost::shared_ptr<const StyleData> StylePtr)
{
	typedef std::set< boost::shared_ptr<const StyleCssRuleData> > StyleCssRuleList;

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
	StyleCssRuleList CssRules = Server->Styles()->GetStyleCssRules(StylePtr->Name(), StylePtr->AuthorId());
	for(StyleCssRuleList::const_iterator itr = CssRules.begin();
		itr != CssRules.end();
		++itr)
	{
		DboCssRule *Rule = new DboCssRule(*itr, this);
		styleSheet().addRule(Rule);
	}

	_CurrentStylePtr = StylePtr;
	_StyleChanged.emit();
}

void Application::SetPage(boost::shared_ptr<const PageData> PagePtr)
{
	_CurrentPagePtr = PagePtr;

	if(!_CurrentPagePtr)
	{
		setInternalPathValid(false);
	}
	_PageChanged.emit();
}

void Application::RefreshLocaleStrings()
{
	Application *app = Application::instance();
	if(!app)
	{
		return;
	}
	app->refresh();
	app->triggerUpdate();
}

void Application::RefreshStyleStrings()
{
	Application *app = Application::instance();
	if(!app)
	{
		return;
	}

	//Refresh Templates, Style WCssStyleSheet, Template WCssStyleSheets
	WServer *Server = WServer::instance();

	//Style CssStyleSheet
	boost::shared_ptr<const StyleData> NewStylePtr;
	if(app->CurrentStyle())
	{
		NewStylePtr = Server->Styles()->GetStylePtr(app->CurrentStyle()->Name(), app->CurrentStyle()->AuthorId());
		if(!NewStylePtr)
		{
			//Default style is taken from server's active configuration instead of cached configuration because it is possible
			//for the default style to get changed from the configuration and that old default style being changed/deleted
			std::string DefaultStyleName = Server->Configurations()->GetStr("DefaultStyleName", ModulesDatabase::Styles, "Default");
			long long DefaultStyleAuthor = Server->Configurations()->GetLongInt("DefaultStyleAuthor", ModulesDatabase::Styles, 1);
			NewStylePtr = Server->Styles()->GetStylePtr(DefaultStyleName, DefaultStyleAuthor);
		}
	}
	app->SetStyle(NewStylePtr);

	//Template CssStyleSheets, remove all rules and add new
	typedef std::set< boost::shared_ptr<const TemplateCssRuleData> > TemplateCssRuleList;
	for(TemplateStyleSheetMap::iterator itr = app->_TemplateStyleSheets.begin();
		itr != app->_TemplateStyleSheets.end();
		++itr)
	{
		itr->second.clear();
		TemplateCssRuleList CssRules = Server->Styles()->GetTemplateCssRules(itr->first.first, itr->first.second);
		for(TemplateCssRuleList::const_iterator it = CssRules.begin();
			it != CssRules.end();
			++it)
		{
			itr->second.addRule(new DboCssRule(*it, app));
		}
	}

	//Send changes
	app->refresh();
	app->triggerUpdate();
}

void Application::RefreshPageStrings()
{
	//Application *app = Application::instance();
	//if(!app)
	//{
	//	return;
	//}

	//triggerUpdate();
}

void Application::UseTemplateStyleSheet(boost::shared_ptr<const TemplateData> TemplatePtr)
{
	typedef std::set< boost::shared_ptr<const TemplateCssRuleData> > TemplateCssRuleList;

	//Ignore if its an empty ptr
	if(!TemplatePtr)
	{
		return;
	}

	//Ignore if the stylesheet is already loaded
	TemplateStyleSheetMap::const_iterator itr = _TemplateStyleSheets.find(std::make_pair(TemplatePtr->Name(), TemplatePtr->ModuleId()));
	if(itr != _TemplateStyleSheets.end())
	{
		return;
	}

	//Create the stylesheet if its not loaded
	WServer *Server = WServer::instance();
	Wt::WCssStyleSheet TemplateStyleSheet;
	
	//Ignore if there are no CSS rules for this template
	TemplateCssRuleList CssRules = Server->Styles()->GetTemplateCssRules(TemplatePtr->Name(), TemplatePtr->ModuleId());
	if(CssRules.empty())
	{
		return;
	}

	for(TemplateCssRuleList::const_iterator itr = CssRules.begin();
		itr != CssRules.end();
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
	internalPathAfterReservedChanged().emit(InternalPathAfterReserved());
// 	std::string NewReservedInternalPath = InternalPathAfterReserved();
// 	std::string OldReservedInternalPath = Wt::Utils::append(oldInternalPath_, '/').substr(_OldReservedInternalPath.size());
// 	if(OldReservedInternalPath != NewReservedInternalPath)
// 	{
// 		internalPathAfterReservedChanged().emit(NewReservedInternalPath);
// 	}
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
	ReadLock LanguagesLock(Server->Languages());
	ReadLock AccessPathsLock(Server->AccessPaths());
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
		//Emit if MobileVersion just got enabled
		_ReservedInternalPath += Path; //Add mobile access path to reserved path
		if(IsMobileVersion() == false)
		{
			_MobileVersionChanged.emit(true);
		}
		_MobileVersionFromInternalPath = true;
		return true;
	}
	else
	{
		//Emit if MobileVersion just got disabled
		if(IsMobileVersion() == true)
		{
			_MobileVersionChanged.emit(false);
		}
		_MobileVersionFromInternalPath = false;
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
	boost::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
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
	boost::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
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
	boost::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
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
	boost::shared_ptr<const LanguageAccessPathData> LanguageAccessPath;
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

	ReadLock PagesLock(Server->Pages());
	ReadLock AccessPathsLock(Server->AccessPaths());

	//Check if internal path includes page access path
	boost::shared_ptr<const PageAccessPathData> PageAccessPathPtr, ParentAccessPathPtr;
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

	boost::shared_ptr<const PageData> PagePtr;
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
		if(!_CurrentPagePtr)
		{
			SetPage(PagePtr);
		}
		else if(PagePtr->id() != _CurrentPagePtr->id())
		{
			SetPage(PagePtr);
		}
	}
	else
	{
		SetPage(0);
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
	(new Wt::WText(std::string("Current Page: "), root()))->decorationStyle().font().setWeight(Wt::WFont::Bold);
	auto cpt = new Wt::WText(std::string(CurrentPage() ? CurrentPage()->Title : "Invalid Page(404)"), root());
	new Wt::WBreak(root());
	PageChanged().connect(boost::bind<void>([this, cpt](){
		cpt->setText(std::string(CurrentPage() ? CurrentPage()->Title : "Invalid Page(404)"));
	}));

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
		Server->DatabaseManager()->Reload();
		new Wt::WText(std::string("[Server Config; AFTER Reload()]Log Directory: ") + Server->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WText(std::string("[Cache Config; AFTER Reload()]Log Directory: ") + Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root());
		new Wt::WBreak(root());
	}));

	Wt::WPushButton *rs = new Wt::WPushButton("Reload Styles", root());
	rs->clicked().connect(boost::bind<void>([Server](){
		Server->DatabaseManager()->Reload();
	}));
	Wt::WPushButton *cs = new Wt::WPushButton("Change Style", root());
	cs->clicked().connect(boost::bind<void>([this, Server](){
		if(!_CurrentStylePtr)
		{
			ChangeStyle("Default", 1);
		}
		else if(_CurrentStylePtr->Name() == "Default")
		{
			ChangeStyle("Test", 1);
		}
		else
		{
			ChangeStyle("Default", 1);
		}
		triggerUpdate();
	}));
	new Wt::WBreak(root());
	Wt::WPushButton *rl = new Wt::WPushButton("Reload languages", root());
	rl->clicked().connect(boost::bind<void>([Server](){
		Server->DatabaseManager()->Reload();
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

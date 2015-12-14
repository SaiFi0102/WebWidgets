#include "Application/WApplication.h"
#include "Application/WServer.h"
#include "Objects/DboLocalizedStrings.h"
#include "Objects/DboCssRule.h"

#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/LanguageDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationCache.h"
#include "DboDatabase/StyleDatabase.h"
#include "DboDatabase/PageDatabase.h"
#include "DboDatabase/NavigationMenuDatabase.h"
#include "DboDatabase/ReadLock.h"

#include "Widgets/DboMenu.h"
#include "Widgets/PageStackWidget.h"
#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <Wt/WResource>
#include <Wt/WStackedWidget>
#include <Wt/WMenu>
#include <Wt/WebUtils.h>

//Test
#include "Pages/TestPage.h"
//#include <Wt/WBootstrapTheme>

namespace WW
{

//From https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c/4654718#4654718
bool is_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while(it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

WApplication::WApplication(const Wt::WEnvironment &env)
	: _startTime(boost::posix_time::microsec_clock::local_time()),
	Wt::WApplication(env),
	_localeChanged(this), _internalPathChanged(this), _internalPathAfterReservedChanged(this), _mobileVersionChanged(this), //Signals
	_pageChanged(this), _reservedInternalPathChanged(this) //Signals
{
	//Enable server push
	enableUpdates();

	//Cache configurations
	WServer *server = WServer::instance();
	_configurations = new ConfigurationCache(server->configurations()); //Local copy of configuration ptrs

	//Set AccessHostName and its default settings
	setAccessHostNameDefaults();
	setLocale(_sessionDefaultLocale); //Default language
	setStyle(_defaultStylePtr); //Default style

	//Use database backend localized strings instead of WMessageResourceBundle
	setLocalizedStrings(new DboLocalizedStrings(server));

	//Internal paths
	setInternalPathDefaultValid(true);

	//Connect slots that should run first
	Wt::WApplication::internalPathChanged().connect(std::bind(&WApplication::handleWtInternalPathChanged, this));

	//Set language from internal path
	interpretReservedInternalPath(true);

	//Main stack widget
	_pageStack = new PageStackWidget();

	//Main navigation menu
	_pageMenu = new DboMenu(1, _pageStack);

	//Main template widget
	_mainTemplate = new Wt::WTemplate(Wt::WString::tstr("main", ModuleDatabase::Styling), root());
	_mainTemplate->bindWidget("page-content", _pageStack);
	_mainTemplate->bindWidget("navigation", _pageMenu);
	TestPage testPage;
	_mainTemplate->bindWidget("test-page", testPage.createContent());

	//Page change signal/slot and home page
	interpretInternalPathAfterReserved();

	//User stylesheet
	//useStyleSheet(_UserStyleSheet);

	//Test theme
	//setTheme(new Wt::WBootstrapTheme());

	//Initialization duration
	boost::posix_time::ptime initEndTime = boost::posix_time::microsec_clock::local_time();
	new Wt::WText(std::string("Application initialization duration: ")
		+ boost::lexical_cast<std::string>((initEndTime - _startTime).total_milliseconds())
		+ " ms", root());
	new Wt::WText(std::string("; ")
		+ boost::lexical_cast<std::string>((initEndTime - _startTime).total_microseconds())
		+ " µs", root());
	new Wt::WBreak(root());
}

WApplication::~WApplication()
{
	delete _configurations;
}

void WApplication::setAccessHostNameDefaults()
{
	WServer *server = WServer::instance();
	ReadLock lock(server->databaseManager());

	//Set AccessHostNames
	std::string _hostname = environment().hostName();
	_globalAccessHost = server->accessPaths()->accessHostNamePtr("");
	_accessHostName = server->accessPaths()->accessHostOrGlobalPtr(_hostname);

	//Home page
	if(_accessHostName->defaultPageId == -1)
	{
		if(_globalAccessHost->defaultPageId != -1)
			_homePagePtr = server->pages()->getPtr(_globalAccessHost->defaultPageId);
		else
			_homePagePtr = nullptr;
	}
	else
		_homePagePtr = server->pages()->getPtr(_accessHostName->defaultPageId);

	//Default style
	if(_accessHostName->styleId == -1)
	{
		if(_globalAccessHost->styleId == -1)
			_defaultStylePtr = server->styles()->firstStylePtr();
		else
			_defaultStylePtr = server->styles()->getStylePtr(_globalAccessHost->styleId);
	}
	else
		_defaultStylePtr = server->styles()->getStylePtr(_accessHostName->styleId);

	//Language and Mobile Mode
	_clientLocale = environment().locale();
	std::string defaultLanguageCode = server->languages()->defaultLanguageCode(_hostname);
	_languageFromHostname = false;
	if(_accessHostName != _globalAccessHost)
	{
		//Check if user is using Hostname AccessPath before checking LanguageAccept for Language
		if(!_accessHostName->languageCode.empty())
		{
			_sessionDefaultLocale = server->languages()->getLocaleFromCode(_accessHostName->languageCode, _hostname);
			_languageFromHostname = true;
		}

		//Mobile mode
		bool omv = isMobileVersion();
		_mobileVersionFromHostname = _accessHostName->mobileMode;
		bool nmv = isMobileVersion();

		if(omv != nmv)
			mobileVersionChanged().emit(nmv);
	}

	//Set environment locale in Application if language was not found in AccessHostName
	if(!_languageFromHostname)
	{
		std::string languageAccept = environment().locale().name();
		if(languageAccept.empty()) //If no language accept http header found, just set locale from default language
			_sessionDefaultLocale = server->languages()->getLocaleFromCode(defaultLanguageCode, _hostname);
		else
		{
			if(server->languages()->languageAcceptExists(languageAccept))
				_sessionDefaultLocale = server->languages()->getLocaleFromLanguageAccept(languageAccept, _hostname);
			else //If not break up the LanguageAccept and check with wild cards
			{
				std::string languageAcceptLookup = languageAccept;
				std::string::size_type dashPosition = languageAccept.find('-');
				if(dashPosition != std::string::npos) //If there is a country specific language in language accept
					languageAcceptLookup = languageAccept.substr(0, dashPosition); //Remove everything with and after the dash, "-".

				if(server->languages()->languageAcceptExists(languageAcceptLookup + "*")) //Check if LanguageAccept with wild card exist
					_sessionDefaultLocale = server->languages()->getLocaleFromLanguageAccept(languageAcceptLookup + "*", _hostname);
				else //else set locale from default language
					_sessionDefaultLocale = server->languages()->getLocaleFromCode(defaultLanguageCode, _hostname);
			}
		}
	}
}

void WApplication::setLocale(const Wt::WLocale &locale)
{
	Wt::WApplication::setLocale(locale);
	localeChanged().emit();
}

std::string WApplication::internalPathAfterReservedNextPart(const std::string &after) const
{
	if(_reservedInternalPath == "/")
		return internalPathNextPart(after);

	return internalPathNextPart(_reservedInternalPath + after);
}

std::string WApplication::internalPathAfterReserved() const
{
	return Wt::Utils::prepend((internalSubPath(_reservedInternalPath)), '/');
}

std::string WApplication::oldInternalPathAfterReserved() const
{
	std::string current = Wt::Utils::append(oldInternalPath_, '/');
	if(!pathMatches(current, _oldReservedInternalPath))
		return std::string();

	return current.substr(_oldReservedInternalPath.length());
}

void WApplication::setInternalPathAfterReserved(const std::string &path, bool emitChange)
{
	//Set path
	if(_reservedInternalPath == "/")
		setInternalPath(path, false);
	else
		setInternalPath(_reservedInternalPath + path, false);

	interpretInternalPathAfterReserved();
	if(emitChange)
	{
		internalPathChanged().emit(_reservedInternalPath + path);
		internalPathAfterReservedChanged().emit(path);
	}
}

void WApplication::changeStyle(const std::string &styleName, long long authorId)
{
	WServer *server = WServer::instance();
	Ddo::cPtr<Ddo::Style> stylePtr = server->styles()->getStylePtr(styleName, authorId);
	if(!stylePtr)
		return;

	if(_currentStylePtr && _currentStylePtr->id() == stylePtr->id())
		return;

	setStyle(stylePtr);
	refresh(); //To reload styletemplates
}

void WApplication::setStyle(Ddo::cPtr<Ddo::Style> stylePtr)
{
	//Remove CSS rules
	styleSheet().clear();

	//No style
	if(!stylePtr)
	{
		_currentStylePtr = nullptr;
		styleChanged().emit();
		return;
	}

	//Add CSS rules for new style
	WServer *server = WServer::instance();
	for(const auto &ptr : stylePtr->styleCssRules)
	{
		DboCssRule *rule = new DboCssRule(ptr, this);
		styleSheet().addRule(rule);
	}

	_currentStylePtr = stylePtr;
	styleChanged().emit();
}

void WApplication::setPage(Ddo::cPtr<Ddo::Page> pagePtr, Ddo::cPtr<Ddo::Page> validParentPagePtr)
{
	WServer *server = WServer::instance();
	_currentPagePtr = pagePtr;
	_validParentPagePtr = validParentPagePtr;

	PageHandler *pageHandler = server->pages()->getPage(_currentPagePtr ? _currentPagePtr->id() : -1);
	if(!_currentPagePtr || !pageHandler)
	{
		setInternalPathValid(false);
		pageChanged().emit();
		return;
	}
	
	pageChanged().emit();
}

void WApplication::refreshDdoPtrs()
{
	WApplication *app = WApplication::instance();
	if(!app)
		return;

	app->setAccessHostNameDefaults();

	WServer *server = WServer::instance();
	ReadLock lock(server->databaseManager());

	//Refresh Templates, Style WCssStyleSheet, Template WCssStyleSheets
	//Style CssStyleSheet
	Ddo::cPtr<Ddo::Style> newStylePtr;
	if(app->currentStyle())
	{
		newStylePtr = server->styles()->getStylePtr(app->currentStyle()->id());
		if(!newStylePtr)
			newStylePtr = app->_defaultStylePtr;
	}
	else
		newStylePtr = app->_defaultStylePtr;

	app->setStyle(newStylePtr);

	//Template CssStyleSheets, remove all rules and add new
	typedef std::set< Ddo::cPtr<Ddo::TemplateCssRule> > TemplateCssRuleList;
	for(TemplateStyleSheetMap::iterator itr = app->_templateStyleSheets.begin();
		itr != app->_templateStyleSheets.end();
		++itr)
	{
		itr->second.clear();
		Ddo::cPtr<Ddo::Template> templatePtr = server->styles()->getTemplatePtr(itr->first.first, itr->first.second);
		if(!templatePtr)
			continue;

		for(const auto &ptr : templatePtr->templateCssRules)
		{
			itr->second.addRule(new DboCssRule(ptr, app));
		}
	}

	//Hacky way to check if locale was not set by InterpretReservedInternalPath()
	//This happens probably because no locale was found
	const Wt::WLocale *localePtr = &app->locale();
	app->interpretReservedInternalPath(false);
	if(localePtr == &app->locale()) //If setLocale isn't called from interpretReservedInternalPath()
	{
		if(server->languages()->languageCodeExists(app->locale().name()))
			app->setLocale(server->languages()->getLocaleFromCode(app->locale().name(), app->environment().hostName()), false);
		else
			app->setLocale(app->_sessionDefaultLocale, false);
	}
	app->interpretInternalPathAfterReserved();

	app->refresh();
	app->triggerUpdate();
}

void WApplication::useTemplateStyleSheet(Ddo::cPtr<Ddo::Template> templatePtr)
{
	typedef std::set< Ddo::cPtr<Ddo::TemplateCssRule> > TemplateCssRuleList;

	//Ignore if its an empty ptr
	if(!templatePtr)
		return;

	//Ignore if the stylesheet is already loaded
	if(_templateStyleSheets.find(std::make_pair(templatePtr->name(), templatePtr->moduleId())) != _templateStyleSheets.end())
		return;

	//Create the stylesheet if its not loaded
	Wt::WCssStyleSheet templateStyleSheet;
	
	//Ignore if there are no CSS rules for this template
	if(templatePtr->templateCssRules.empty())
		return;

	for(TemplateCssRuleList::const_iterator itr = templatePtr->templateCssRules.begin();
		itr != templatePtr->templateCssRules.end();
		++itr)
	{
		DboCssRule *rule = new DboCssRule(*itr, this);
		templateStyleSheet.addRule(rule);
	}

	//Add to template style sheets map and application to be loaded
	_templateStyleSheets[std::make_pair(templatePtr->name(), templatePtr->moduleId())] = templateStyleSheet;
	useStyleSheet(_templateStyleSheets[std::make_pair(templatePtr->name(), templatePtr->moduleId())]);
}

void WApplication::handleWtInternalPathChanged()
{
	//Call events in order
	interpretReservedInternalPath(true);
	interpretInternalPathAfterReserved();

	//Emit our version of internalPathChanged()
	internalPathChanged().emit(internalPath());

	//Check if internal path after reserved changed
	if(internalPathAfterReserved() != oldInternalPathAfterReserved())
		internalPathAfterReservedChanged().emit(internalPathAfterReserved());

	if(_oldReservedInternalPath != _reservedInternalPath)
		reservedInternalPathChanged().emit(reservedInternalPath());
}

void WApplication::interpretReservedInternalPath(bool refresh)
{
	//Skip when setInternalPath() was recalled from this function OR when reserved internal path was not changed
	if(_skipReservedPathInterpretation)
	{
		_skipReservedPathInterpretation = false;
		return;
	}

	//Internal path language presentation mode
	int IPLM = configurations()->getEnum("InternalPathMode", ModuleDatabase::Localization, 3);
	if(environment().agentIsSpiderBot() && (IPLM == 3 || IPLM == 4)) //Change NoRestriction modes to AlwaysShowHideDef for bots
		IPLM = 2;

	//Old reserved internal path
	_oldReservedInternalPath = _reservedInternalPath;
	_reservedInternalPath = "/";

	WServer *server = WServer::instance();
	ReadLock lock(server->databaseManager());
	switch(IPLM)
	{
		case 1: IRIPAlwaysShow(refresh); break;
		case 2: IRIPAlwaysShowHideDef(refresh); break;
		case 3: IRIPNoRestrictionHideDef(refresh); break;
		case 4: IRIPNoRestriction(refresh); break;
	}
}

bool WApplication::IRIPMobileVersion(const std::string &path)
{
	//If AccessHostName is not global hostname and AccessHostName mobile mode is on no need to check internal paths
	std::string mobileInternalPath = _accessHostName->mobileInternalPath;
	if(_accessHostName != _globalAccessHost)
	{
		if(_mobileVersionFromHostname)
			return false;

		if(mobileInternalPath.empty())
			mobileInternalPath = _globalAccessHost->mobileInternalPath;
	}
	if(mobileInternalPath.empty())
		return false;

	if(path == mobileInternalPath)
	{
		_reservedInternalPath += path; //Add mobile access path to reserved path
		//Emit if MobileVersion just got enabled
		bool omv = isMobileVersion();
		_mobileVersionFromInternalPath = true;
		if(omv == false)
			mobileVersionChanged().emit(true);

		return true;
	}
	else
	{
		//Emit if MobileVersion just got disabled
		bool omv = isMobileVersion();
		_mobileVersionFromInternalPath = false;
		if(omv == true)
			mobileVersionChanged().emit(false);

		return false;
	}
}

void WApplication::IRIPAlwaysShow(bool refrsh)
{
	WServer *server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string hostName = environment().hostName();

	if(InternalPath == "/")
	{
		_reservedInternalPath = server->accessPaths()->firstLanguagePath(locale().name(), hostName, _languageFromHostname);
		_skipReservedPathInterpretation = true;
		setInternalPath(_reservedInternalPath, true);
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> sep("/");
	Tokenizer tokens(InternalPath, sep);
	Tokenizer::iterator itr = tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*itr))
	{
		++itr;
		if(itr == tokens.end())
			return;
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPath;
	if(!itr->empty())
	{
		if(!(languageAccessPath = server->accessPaths()->languageAccessPathPtr(hostName, *itr)) //Check again if not found(without hostname)
			&& !_languageFromHostname)
		{
			languageAccessPath = server->accessPaths()->languageAccessPathPtr("", *itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(languageAccessPath) //Found
	{
		if(locale().name() != languageAccessPath->languageCode)
			setLocale(server->languages()->getLocaleFromCode(languageAccessPath->languageCode, hostName), refrsh);

		if(_reservedInternalPath != "/")
			_reservedInternalPath += "/";

		_reservedInternalPath += *itr; //Add language internal path to reserved
	}
	else //Not found
	{
		std::string currentLanguageInternalPath = server->accessPaths()->firstLanguagePath(locale().name(), hostName, _languageFromHostname);
		if(_reservedInternalPath == "/")
			_reservedInternalPath = "";

		_reservedInternalPath += currentLanguageInternalPath; //Add language internal path to reserved
		if(currentLanguageInternalPath == "/")
			currentLanguageInternalPath = "";

		//Prepend current language instead of replacing because probably that path wont be a language code
		_skipReservedPathInterpretation = true;
		setInternalPath(
			std::string(InternalPath).replace(InternalPath.find(std::string("/") + *itr),
			itr->size() + 1,
			currentLanguageInternalPath + "/" + *itr), true
		);
	}
}

void WApplication::IRIPAlwaysShowHideDef(bool refrsh)
{
	WServer *server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string hostName = environment().hostName();

	if(InternalPath == "/")
	{
		if(locale().name() != _sessionDefaultLocale.name())
		{
			//_ReservedInternalPath = "/";
			setLocale(_sessionDefaultLocale, refrsh);
		}
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> sep("/");
	Tokenizer tokens(InternalPath, sep);
	Tokenizer::iterator itr = tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*itr))
	{
		++itr;
		if(itr == tokens.end())
			return;
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPath;
	if(!itr->empty())
	{
		if(!(languageAccessPath = server->accessPaths()->languageAccessPathPtr(hostName, *itr)) //Check again if not found(without hostname)
			&& !_languageFromHostname)
		{
			languageAccessPath = server->accessPaths()->languageAccessPathPtr("", *itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(languageAccessPath) //Found
	{
		if(locale().name() != languageAccessPath->languageCode)
			setLocale(server->languages()->getLocaleFromCode(languageAccessPath->languageCode, hostName), refrsh);

		//Remove default language from internal path if its there
		if(locale().name() == _sessionDefaultLocale.name())
		{
			_skipReservedPathInterpretation = true;
			setInternalPath(std::string(InternalPath).replace(InternalPath.find(std::string("/") + *itr), itr->size() + 1, ""), true);
		}
		else
		{
			if(_reservedInternalPath != "/")
				_reservedInternalPath += "/";

			_reservedInternalPath += *itr; //Add language internal path to reserved
		}
	}
	else //Not found
	{
		if(locale().name() != _sessionDefaultLocale.name()) //if set to hide default and if current language is NOT default
			setLocale(_sessionDefaultLocale, refrsh);
	}
}

void WApplication::IRIPNoRestrictionHideDef(bool refrsh)
{
	WServer *server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string hostName = environment().hostName();

	if(InternalPath == "/")
	{
		//_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> sep("/");
	Tokenizer tokens(InternalPath, sep);
	Tokenizer::iterator itr = tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*itr))
	{
		++itr;
		if(itr == tokens.end())
			return;
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPath;
	if(!itr->empty())
	{
		if(!(languageAccessPath = server->accessPaths()->languageAccessPathPtr(hostName, *itr)) //Check again if not found(without hostname)
			&& !_languageFromHostname)
		{
			languageAccessPath = server->accessPaths()->languageAccessPathPtr("", *itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(languageAccessPath) //Found
	{
		if(locale().name() != languageAccessPath->languageCode)
			setLocale(server->languages()->getLocaleFromCode(languageAccessPath->languageCode, hostName), refrsh);

		//Remove default language automatically from internal path if locale is default language
		if(locale().name() == _sessionDefaultLocale.name())
		{
			std::string newPath = std::string(InternalPath).replace(InternalPath.find(std::string("/") + *itr), itr->size() + 1, "");
			if(newPath.empty())
				newPath = "/";

			_skipReservedPathInterpretation = true;
			setInternalPath(newPath, true);
		}
		else
		{
			if(_reservedInternalPath != "/")
				_reservedInternalPath += "/";

			_reservedInternalPath += *itr; //Add language internal path to reserved
		}
	}
}

void WApplication::IRIPNoRestriction(bool refrsh)
{
	WServer *server = WServer::instance();
	std::string InternalPath = internalPath();
	std::string hostName = environment().hostName();

	if(InternalPath == "/")
	{
		//_ReservedInternalPath = "/";
		return;
	}

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> sep("/");
	Tokenizer tokens(InternalPath, sep);
	Tokenizer::iterator itr = tokens.begin();

	//Check mobile version
	if(IRIPMobileVersion(*itr))
	{
		++itr;
		if(itr == tokens.end())
			return;
	}

	//Check if internal path includes language access path and set LanguageAccessPath ptr
	Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPath;
	if(!itr->empty())
	{
		if(!(languageAccessPath = server->accessPaths()->languageAccessPathPtr(hostName, *itr)) //Check again if not found(without hostname)
			&& !_languageFromHostname)
		{
			languageAccessPath = server->accessPaths()->languageAccessPathPtr("", *itr);
		}
	}

	//Check if LanguageAccessPath was found and set locale accordingly
	if(languageAccessPath) //Found
	{
		if(locale().name() != languageAccessPath->languageCode)
			setLocale(server->languages()->getLocaleFromCode(languageAccessPath->languageCode, hostName), refrsh);

		if(_reservedInternalPath != "/")
			_reservedInternalPath += "/";

		_reservedInternalPath += *itr; //Add language internal path to reserved
	}
}

void WApplication::interpretPageInternalPath()
{
	WServer *server = WServer::instance();
	std::string InternalPath = internalPathAfterReserved();
	std::string hostName = environment().hostName();

	//Tokenize internal path
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;
	boost::char_separator<char> sep("/");
	Tokenizer tokens(InternalPath, sep);

	ReadLock lock(server->databaseManager());

	//Check if internal path includes page access path
	Ddo::cPtr<Ddo::PageAccessPath> pageAccessPathPtr, parentAccessPathPtr;
	for(Tokenizer::iterator itr = tokens.begin();
		itr != tokens.end();
		++itr)
	{
		parentAccessPathPtr = pageAccessPathPtr;
		long long parentAccessPathId = parentAccessPathPtr ? parentAccessPathPtr->id() : -1;
		if(!(pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr(hostName, *itr, parentAccessPathId)))
			pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr("", *itr, parentAccessPathId);

		if(!pageAccessPathPtr && parentAccessPathPtr)
		{
			if(is_number(*itr) && !(pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr(hostName, "#", parentAccessPathId)))
				pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr("", "#", parentAccessPathId);
			else if(!(pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr(hostName, "?", parentAccessPathId)))
				pageAccessPathPtr = server->accessPaths()->pageAccessPathPtr("", "?", parentAccessPathId);
		}
		if(!pageAccessPathPtr)
		{
			pageAccessPathPtr = nullptr;
			break;
		}
	}
	_pageAccessPathPtr = pageAccessPathPtr;

	Ddo::cPtr<Ddo::Page> pagePtr, validParentPagePtr;
	if(pageAccessPathPtr)
		pagePtr = server->pages()->getPtr(pageAccessPathPtr->pageId);
	else if(InternalPath == "/") //If still not found use the default homepage if user is on homepage
		pagePtr = _homePagePtr;

	//ParentAccessPathPtr points to the last valid parent page access path ptr
	if(parentAccessPathPtr)
		validParentPagePtr = server->pages()->getPtr(parentAccessPathPtr->pageId);

	if(pagePtr)
	{
		if(!_currentPagePtr || pagePtr->id() != _currentPagePtr->id())
			setPage(pagePtr, validParentPagePtr);
	}
	else
		setPage(nullptr, validParentPagePtr);
}

Wt::Signal<std::string> &WApplication::internalPathChanged()
{
	Wt::WApplication::internalPathChanged();
	return _internalPathChanged;
}

}
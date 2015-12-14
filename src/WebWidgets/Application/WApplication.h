#ifndef WW_WAPPLICATION_H
#define WW_WAPPLICATION_H

#include <unordered_map>
#include "Objects/PairHash.h"
#include "Application/Session.h"
#include "Dbo/ModuleTreeDbos.h"
#include <Wt/WApplication>

namespace WW
{
	class ConfigurationCache;
	class DboMenu;
	class PageStackWidget;
	class Session;

	class WApplication : public Wt::WApplication
	{
	public:
		WApplication(const Wt::WEnvironment &env);
		virtual ~WApplication() override;

		static WApplication *instance() { return dynamic_cast<WApplication*>(Wt::WApplication::instance()); }
		static WApplication *createApplication(const Wt::WEnvironment &env) { return new WApplication(env); }

		//Cached database(s)
		ConfigurationCache *configurations() const { return _configurations; }

		//URL and Internal Paths
		Ddo::cPtr<Ddo::AccessHostName> accessHostName() const { return _accessHostName; }
		std::string internalPathAfterReservedNextPart(const std::string &path) const; //path must start with '/'
		std::string internalPathAfterReserved() const;
		std::string reservedInternalPath() const { return _reservedInternalPath; }
		Wt::Signal<std::string> &internalPathChanged();
		Wt::Signal<std::string> &internalPathAfterReservedChanged() { return _internalPathAfterReservedChanged; }
		Wt::Signal<std::string> &reservedInternalPathChanged() { return _reservedInternalPathChanged; }
		void setInternalPathAfterReserved(const std::string &path, bool emitChange = false);

		//Localization
		void setLocale(const Wt::WLocale &locale);
		Wt::Signal<void> &localeChanged() { return _localeChanged; }

		//Mobile UI
		bool isMobileVersion() const { return _mobileVersionFromHostname || _mobileVersionFromInternalPath; }
		Wt::Signal<bool> &mobileVersionChanged() { return _mobileVersionChanged; }

		//Styling
		Ddo::cPtr<Ddo::Style> currentStyle() const { return _currentStylePtr; }
		//void ChangeStyle(long long StyleId);
		void changeStyle(const std::string &styleName, long long authorId);
		Wt::Signal<void> &styleChanged() { return _styleChanged; }
		Wt::WCssStyleSheet &userStyleSheet() { return _userStyleSheet; }

		//Paging
		Ddo::cPtr<Ddo::Page> currentPage() const { return _currentPagePtr; }
		Ddo::cPtr<Ddo::Page> validParentPage() const { return _validParentPagePtr; }
		Ddo::cPtr<Ddo::PageAccessPath> currentPageAccessPath() const { return _pageAccessPathPtr; }
		Wt::Signal<void> &pageChanged() { return _pageChanged; }
		PageStackWidget *pageStack() { return _pageStack; }

		//Session
		Session &session() { return _session; }

		//Database reload handler
		static void refreshDdoPtrs();

	protected:
		typedef std::unordered_map<std::pair<std::string, long long>, Wt::WCssStyleSheet> TemplateStyleSheetMap;

		//Internal path change handlers
		using Wt::WApplication::setLocale;
		void handleWtInternalPathChanged();
		void interpretReservedInternalPath(bool refresh);
		void interpretInternalPathAfterReserved() { interpretPageInternalPath(); }
		bool IRIPMobileVersion(const std::string &path);
		void IRIPAlwaysShow(bool refresh);
		void IRIPAlwaysShowHideDef(bool refresh);
		void IRIPNoRestrictionHideDef(bool refresh);
		void IRIPNoRestriction(bool refresh);
		void interpretPageInternalPath();
		std::string oldInternalPathAfterReserved() const;

		//Refresh
		void setAccessHostNameDefaults();

		//Styling
		void setStyle(Ddo::cPtr<Ddo::Style> stylePtr);
		void useTemplateStyleSheet(Ddo::cPtr<Ddo::Template> templatePtr);

		//Paging
		void setPage(Ddo::cPtr<Ddo::Page> pagePtr, Ddo::cPtr<Ddo::Page> validParentPagePtr);

		Wt::WTemplate *_mainTemplate = nullptr;
		PageStackWidget *_pageStack = nullptr;
		DboMenu *_pageMenu = nullptr;

		Wt::WLocale _clientLocale;
		Wt::WLocale _sessionDefaultLocale;
		Ddo::cPtr<Ddo::AccessHostName> _accessHostName;
		Ddo::cPtr<Ddo::AccessHostName> _globalAccessHost;
		bool _languageFromHostname = false; //And independent of internal path
		bool _skipReservedPathInterpretation = false; //Used to skip InterpretReservedInternalPath on internalPathChanged()
		std::string _reservedInternalPath = "/";
		std::string _oldReservedInternalPath = "/";
		Wt::Signal<void> _localeChanged;
		Wt::Signal<std::string> _internalPathChanged;
		Wt::Signal<std::string> _reservedInternalPathChanged;
		Wt::Signal<std::string> _internalPathAfterReservedChanged;

		bool _mobileVersionFromHostname = false; //And independent of internal path
		bool _mobileVersionFromInternalPath = false; //Or in combination with the hostname
		Wt::Signal<bool> _mobileVersionChanged;

		Ddo::cPtr<Ddo::Page> _homePagePtr;
		Ddo::cPtr<Ddo::Style> _currentStylePtr;
		Ddo::cPtr<Ddo::Style> _defaultStylePtr;
		Wt::Signal<void> _styleChanged;
		Wt::WCssStyleSheet _userStyleSheet;
		TemplateStyleSheetMap _templateStyleSheets;

		Ddo::cPtr<Ddo::Page> _currentPagePtr;
		Ddo::cPtr<Ddo::Page> _validParentPagePtr;
		Ddo::cPtr<Ddo::PageAccessPath> _pageAccessPathPtr;
		Wt::Signal<void> _pageChanged;

		Session _session;

		ConfigurationCache *_configurations = nullptr;

		boost::posix_time::ptime _startTime;

	private:
		friend class DboLocalizedStrings;
		friend class TestPage;
	};
}

#endif
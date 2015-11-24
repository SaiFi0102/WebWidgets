#ifndef APPLICATION_H
#define APPLICATION_H

#include <unordered_map>
#include "Objects/PairHash.h"
#include <Wt/WApplication>
#include "Dbo/Style.h"

class ConfigurationsCache;
class PagesMenu;
class PagesStackWidget;

class Application : public Wt::WApplication
{
	public:
		Application(const Wt::WEnvironment &env);
		~Application();

		static Application *instance() { return dynamic_cast<Application*>(Wt::WApplication::instance()); }
		static Application *CreateApplication(const Wt::WEnvironment &env) { return new Application(env); }

		//Cached database(s)
		ConfigurationsCache *Configurations() const { return _Configurations; }

		//URL and Internal Paths
		std::shared_ptr<const AccessHostNameData> AccessHostName() const { return _AccessHostName; }
		std::string InternalPathAfterReservedNextPart(const std::string &path) const; //path must start with '/'
		std::string InternalPathAfterReserved() const;
		std::string ReservedInternalPath() const { return _ReservedInternalPath; }
		Wt::Signal<std::string> &internalPathChanged();
		Wt::Signal<std::string> &internalPathAfterReservedChanged() { return _InternalPathAfterReservedChanged; }
		Wt::Signal<std::string> &reservedInternalPathChanged() { return _ReservedInternalPathChanged; }
		void setInternalPathAfterReserved(const std::string &path, bool emitChange = false);

		//Localization
		void setLocale(const Wt::WLocale &locale);
		Wt::Signal<void> &LocaleChanged() { return _LocaleChanged; }

		//Mobile UI
		bool IsMobileVersion() const { return _MobileVersionFromHostname || _MobileVersionFromInternalPath; }
		Wt::Signal<bool> &MobileVersionChanged() { return _MobileVersionChanged; }

		//Styling
		std::shared_ptr<const StyleData> CurrentStyle() const { return _CurrentStylePtr; }
		//void ChangeStyle(long long StyleId);
		void ChangeStyle(const std::string &StyleName, long long AuthorId);
		Wt::Signal<void> &StyleChanged() { return _StyleChanged; }
		Wt::WCssStyleSheet &UserStyleSheet() { return _UserStyleSheet; }

		//Paging
		std::shared_ptr<const PageData> CurrentPage() const { return _CurrentPagePtr; }
		std::shared_ptr<const PageAccessPathData> CurrentPageAccessPath() const { return _PageAccessPathPtr; }
		Wt::Signal<void> &PageChanged() { return _PageChanged; }

		//Database reload handler
		static void RefreshDboDatabasePtrs();

	protected:
		typedef std::unordered_map<std::pair<std::string, long long>, Wt::WCssStyleSheet> TemplateStyleSheetMap;

		//Internal path change handlers
		void HandleWtInternalPathChanged();
		void InterpretReservedInternalPath();
		bool IRIPMobileVersion(const std::string &Path);
		void IRIPAlwaysShow();
		void IRIPAlwaysShowHideDef();
		void IRIPNoRestrictionHideDef();
		void IRIPNoRestriction();
		void InterpretPageInternalPath();
		std::string OldInternalPathAfterReserved() const;

		//Refresh
		void SetAccessHostNameDefaults();

		//Styling
		void SetStyle(std::shared_ptr<const StyleData> StylePtr);
		void UseTemplateStyleSheet(std::shared_ptr<const TemplateData> TemplatePtr);

		//Paging
		void SetPage(std::shared_ptr<const PageData> PagePtr);

		Wt::WTemplate *_MainTemplate;
		PagesStackWidget *_PageStack;
		PagesMenu *_PageMenu;

		Wt::WLocale _ClientLocale;
		Wt::WLocale _SessionDefaultLocale;
		std::shared_ptr<const AccessHostNameData> _AccessHostName;
		std::shared_ptr<const AccessHostNameData> _GlobalAccessHost;
		bool _LanguageFromHostname; //And independent of internal path
		bool _SkipReservedPathInterpretation; //Used to skip InterpretReservedInternalPath on internalPathChanged()
		std::string _ReservedInternalPath;
		std::string _OldReservedInternalPath;
		Wt::Signal<void> _LocaleChanged;
		Wt::Signal<std::string> _InternalPathChanged;
		Wt::Signal<std::string> _ReservedInternalPathChanged;
		Wt::Signal<std::string> _InternalPathAfterReservedChanged;

		bool _MobileVersionFromHostname; //And independent of internal path
		bool _MobileVersionFromInternalPath; //Or in combination with the hostname
		Wt::Signal<bool> _MobileVersionChanged;

		std::shared_ptr<const PageData> _HomePagePtr;
		std::shared_ptr<const StyleData> _CurrentStylePtr;
		std::shared_ptr<const StyleData> _DefaultStylePtr;
		Wt::Signal<void> _StyleChanged;
		Wt::WCssStyleSheet _UserStyleSheet;
		TemplateStyleSheetMap _TemplateStyleSheets;

		std::shared_ptr<const PageData> _CurrentPagePtr;
		std::shared_ptr<const PageAccessPathData> _PageAccessPathPtr;
		Wt::Signal<void> _PageChanged;

		ConfigurationsCache *_Configurations;

		boost::posix_time::ptime StartTime;

		friend class DboLocalizedStrings;
		friend class TestPage;

	//private:
};

#endif
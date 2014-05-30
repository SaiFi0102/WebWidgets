#ifndef APPLICATION_H
#define APPLICATION_H

#include <Wt/WApplication>
//#include <Wt/Dbo/Session>
#include <boost/unordered_map.hpp>
#include "Dbo/Style.h"

class ConfigurationsProxy;

class Application : public Wt::WApplication
{
	public:
		Application(const Wt::WEnvironment &env);
		~Application();

		static Application *instance();
		static Application *CreateApplication(const Wt::WEnvironment &env);

		ConfigurationsProxy *Configurations();

		std::string InternalPathAfterReserved(const std::string &after = "") const;

		void setLocale(const Wt::WLocale &locale);
		Wt::Signal<void> &LocaleChanged();

		bool IsMobileVersion() const;
		Wt::Signal<bool> &MobileVersionChanged();

		Wt::Dbo::ptr<Style> CurrentStyle();
		void ChangeStyle(Wt::Dbo::ptr<Style> StylePtr);
		Wt::Signal<void> &StyleChanged();
		Wt::WCssStyleSheet &UserStyleSheet();

		void RefreshLocaleStrings();
		void RefreshStyleStrings();
		void RefreshPageStrings();

	protected:
		typedef boost::unordered_map<std::pair<std::string, long long>, Wt::WCssStyleSheet> TemplateStyleSheetMap;

		void SetLanguageFromInternalPath();
		
		void SetStyle(Wt::Dbo::ptr<Style> StylePtr);
		void UseTemplateStyleSheet(Wt::Dbo::ptr<Template> TemplatePtr);

		Wt::Signal<void> _LocaleChanged;
		Wt::WLocale _ClientLocale;
		Wt::WLocale _SessionDefaultLocale;
		bool _LanguageFromHostname;
		bool _SkipLanguageInternalPath;

		bool _MobileVersionFromHostname;
		bool _MobileVersionFromInternalPath;
		Wt::Signal<bool> _MobileVersionChanged;

		Wt::Dbo::ptr<Style> _CurrentStylePtr;
		Wt::Signal<void> _StyleChanged;
		Wt::WCssStyleSheet _UserStyleSheet;
		TemplateStyleSheetMap _TemplateStyleSheets;

		ConfigurationsProxy *_Configurations;

		boost::posix_time::ptime StartTime;
};

#endif
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Wt/WApplication>
//#include <Wt/Dbo/Session>
#include <boost/unordered_map.hpp>
#include "Dbo/Style.h"

class ConfigurationsCache;

class Application : public Wt::WApplication
{
	public:
		Application(const Wt::WEnvironment &env);
		~Application();

		static Application *instance();
		static Application *CreateApplication(const Wt::WEnvironment &env);

		//Cached database(s)
		ConfigurationsCache *Configurations() const;

		//URL and Internal Paths
		std::string InternalPathAfterReservedNextPart(const std::string &path) const; //path must start with '/'
		std::string InternalPathAfterReserved() const;
		std::string InternalPathReserved() const;
		Wt::Signal<void> &internalPathAfterReservedChanged();
		void setInternalPathAfterReserved(const std::string &path, bool emitChange = false);

		//Localization
		void setLocale(const Wt::WLocale &locale);
		Wt::Signal<void> &LocaleChanged();

		//Mobile UI
		bool IsMobileVersion() const;
		Wt::Signal<bool> &MobileVersionChanged();

		//Styling
		Wt::Dbo::ptr<Style> CurrentStyle() const;
		void ChangeStyle(Wt::Dbo::ptr<Style> StylePtr);
		Wt::Signal<void> &StyleChanged();
		Wt::WCssStyleSheet &UserStyleSheet();

		//Database reload handlers
		void RefreshLocaleStrings();
		void RefreshStyleStrings();
		void RefreshPageStrings();

	protected:
		typedef boost::unordered_map<std::pair<std::string, long long>, Wt::WCssStyleSheet> TemplateStyleSheetMap;

		void InterpretReservedInternalPath();
		
		void SetStyle(Wt::Dbo::ptr<Style> StylePtr);
		void UseTemplateStyleSheet(Wt::Dbo::ptr<Template> TemplatePtr);

		Wt::WLocale _ClientLocale;
		Wt::WLocale _SessionDefaultLocale;
		bool _LanguageFromHostname; //And independent of internal path
		bool _SkipReservedPathInterpretation; //Used to skip InterpretReservedInternalPath on internalPathChanged()
		std::string _ReservedInternalPath;
		Wt::Signal<void> _LocaleChanged;
		Wt::Signal<void> _InternalPathAfterReservedChanged;

		bool _MobileVersionFromHostname; //And independent of internal path
		bool _MobileVersionFromInternalPath; //Or in combination with the hostname
		Wt::Signal<bool> _MobileVersionChanged;

		Wt::Dbo::ptr<Style> _CurrentStylePtr;
		Wt::Signal<void> _StyleChanged;
		Wt::WCssStyleSheet _UserStyleSheet;
		TemplateStyleSheetMap _TemplateStyleSheets;

		ConfigurationsCache *_Configurations;

		boost::posix_time::ptime StartTime;

		friend class DboLocalizedStrings;
};

#endif
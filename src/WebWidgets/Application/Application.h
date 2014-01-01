#ifndef APPLICATION_H
#define APPLICATION_H

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/Dbo/SqlConnectionPool>
#include <Wt/Dbo/Session>

class Application : public Wt::WApplication
{
	public:
		Application(const Wt::WEnvironment &env);

		static Application *instance();
		static Application *CreateApplication(const Wt::WEnvironment &env);

		void setLocale(const Wt::WLocale &locale);
		Wt::Signal<void> &LocaleChanged();

	protected:
		void SetLanguageFromInternalPath();

		Wt::Signal<void> _LocaleChanged;

		Wt::WLocale _ClientLocale;
		Wt::WLocale _SessionDefaultLocale;
		bool _LanguageFromHostname;
		bool _SkipLanguageInternalPath;
};

#endif
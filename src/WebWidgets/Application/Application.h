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

	protected:
		void SetLanguageFromInternalPath(bool SetInternalPath = false);

		Wt::WLocale EnviromentLocale;
};

#endif
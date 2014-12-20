#ifndef SERVER_WAPPLICATION_H
#define SERVER_WAPPLICATION_H

#include <Wt/WServer>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/OAuthService>

#include <Wt/Dbo/SqlConnectionPool>

#include <3rdparty/rapidxml/rapidxml.hpp>
#include <boost/thread.hpp>

class Application;
class DboInstaller;
class ModulesDatabase;
class ConfigurationsDatabase;
class LanguagesDatabase;
class StylesDatabase;
class PagesDatabase;
class AccessPathsDatabase;

typedef std::vector<const Wt::Auth::OAuthService *> OAuthServiceMap;

class WServer : public Wt::WServer
{
	public:
		WServer(const std::string& wtApplicationPath = std::string(), const std::string& wtConfigurationFile = std::string());
		~WServer();
		
		ModulesDatabase *Modules() const { return _Modules; }
		ConfigurationsDatabase *Configurations() const { return _Configurations; }
		LanguagesDatabase *Languages() const { return _Languages; }
		StylesDatabase *Styles() const { return _Styles; }
		PagesDatabase *Pages() const { return _Pages; }
		AccessPathsDatabase *AccessPaths() const { return _AccessPaths; }

		const Wt::Auth::AuthService &GetAuthService() const { return AuthService; }
		const Wt::Auth::PasswordService &GetPasswordService() const { return PasswordService; }
		const OAuthServiceMap &GetOAuthServices() const { OAuthServices; }

		Wt::WLogEntry log(const std::string &type) const;
		boost::posix_time::ptime StartPTime() const { return PTStart; }
		bool Start();
		void NewApp(Application *App);
		void AppDeleted(Application *App);

		void RefreshLocaleStrings();
		void RefreshStyleStrings();
		void RefreshPageStrings();

		static WServer *instance() { return dynamic_cast<WServer*>(Wt::WServer::instance()); }

	protected:
		typedef std::set<Application *> ApplicationSet;

		void ConfigureAuth();
		void CreateWtXmlConfiguration();
		void Initialize();

		Wt::Dbo::SqlConnectionPool *SQLPool;

		DboInstaller *_Installer;
		ModulesDatabase *_Modules;
		ConfigurationsDatabase *_Configurations;
		LanguagesDatabase *_Languages;
		StylesDatabase *_Styles;
		PagesDatabase *_Pages;
		AccessPathsDatabase *_AccessPaths;

		ApplicationSet _Applications;

		Wt::rapidxml::xml_document<> XmlDoc;
		boost::posix_time::ptime PTBeforeLoad;
		boost::posix_time::ptime PTStart;

		Wt::Auth::AuthService AuthService;
		Wt::Auth::PasswordService PasswordService;
		OAuthServiceMap OAuthServices;

	private:
		boost::recursive_mutex mutex;
		friend int main(int argc, char** argv);
};

#endif
#ifndef SERVER_WAPPLICATION_H
#define SERVER_WAPPLICATION_H

#include <Wt/WServer>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/OAuthService>

#include <Wt/Dbo/SqlConnectionPool>

#include <3rdparty/rapidxml/rapidxml.hpp>

class Application;
class DboInstaller;
class ModulesDatabase;
class ConfigurationsDatabase;
class LanguagesDatabase;
class StylesDatabase;
class PagesDatabase;
class AccessPathsDatabase;
class NavigationMenusDatabase;
class DboDatabaseManager;

typedef std::vector<const Wt::Auth::OAuthService *> OAuthServiceMap;

class WServer : public Wt::WServer
{
	public:
		WServer(const std::string& wtApplicationPath = std::string(), const std::string& wtConfigurationFile = std::string());
		~WServer();
		
		DboDatabaseManager *DatabaseManager() const { return _DboManager; }
		ModulesDatabase *Modules() const { return _Modules; }
		ConfigurationsDatabase *Configurations() const { return _Configurations; }
		LanguagesDatabase *Languages() const { return _Languages; }
		StylesDatabase *Styles() const { return _Styles; }
		PagesDatabase *Pages() const { return _Pages; }
		AccessPathsDatabase *AccessPaths() const { return _AccessPaths; }
		NavigationMenusDatabase *NavigationMenus() const { return _NavigationMenus; }

		const Wt::Auth::AuthService &GetAuthService() const { return AuthService; }
		const Wt::Auth::PasswordService &GetPasswordService() const { return PasswordService; }
		const OAuthServiceMap &GetOAuthServices() const { OAuthServices; }

		boost::posix_time::ptime StartPTime() const { return PTStart; }
		bool Start();

		static WServer *instance() { return dynamic_cast<WServer*>(Wt::WServer::instance()); }

	protected:
		void ConfigureAuth();
		void CreateWtXmlConfiguration();
		void Initialize();

		void DboDatabaseReloadHandler();

		Wt::Dbo::SqlConnectionPool *SQLPool;

		DboInstaller *_Installer;
		DboDatabaseManager *_DboManager;
		ModulesDatabase *_Modules;
		ConfigurationsDatabase *_Configurations;
		LanguagesDatabase *_Languages;
		StylesDatabase *_Styles;
		PagesDatabase *_Pages;
		AccessPathsDatabase *_AccessPaths;
		NavigationMenusDatabase *_NavigationMenus;

		Wt::rapidxml::xml_document<> XmlDoc;
		boost::posix_time::ptime PTBeforeLoad;
		boost::posix_time::ptime PTStart;

		Wt::Auth::AuthService AuthService;
		Wt::Auth::PasswordService PasswordService;
		OAuthServiceMap OAuthServices;

	private:
		friend int main(int argc, char** argv);
		friend class DboDatabaseManager;
};

#endif
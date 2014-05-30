#ifndef SERVER_WAPPLICATION_H
#define SERVER_WAPPLICATION_H

#include <Wt/WServer>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/OAuthService>

#include <Wt/Dbo/SqlConnectionPool>

#include <rapidxml/rapidxml.hpp>
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
		
		ModulesDatabase *Modules() const;
		ConfigurationsDatabase *Configurations() const;
		LanguagesDatabase *Languages() const;
		StylesDatabase *Styles() const;
		PagesDatabase *Pages() const;
		AccessPathsDatabase *AccessPaths() const;

		const Wt::Auth::AuthService &GetAuthService() const;
		const Wt::Auth::PasswordService &GetPasswordService() const;
		const OAuthServiceMap &GetOAuthServices() const;

		Wt::WLogEntry log(const std::string &type) const;
		boost::posix_time::ptime StartPTime() const;
		bool Start();
		void NewApp(Application *App);
		void AppDeleted(Application *App);

		void RefreshLocaleStrings();
		void RefreshStyleStrings();
		void RefreshPageStrings();

		static WServer *instance();

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

		rapidxml::xml_document<> XmlDoc;
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
#ifndef SERVER_WAPPLICATION_H
#define SERVER_WAPPLICATION_H

#include <Wt/WServer>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/OAuthService>

#include <Wt/Dbo/SqlConnectionPool>

#include <rapidxml/rapidxml.hpp>

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
		virtual ~WServer();
		
		ModulesDatabase *GetModules() const;
		ConfigurationsDatabase *GetConfigurations() const;
		LanguagesDatabase *GetLanguages() const;
		StylesDatabase *GetStyles() const;
		PagesDatabase *GetPages() const;
		AccessPathsDatabase *GetAccessPaths() const;

		const Wt::Auth::AuthService &GetAuthService() const;
		const Wt::Auth::PasswordService &GetPasswordService() const;
		const OAuthServiceMap &GetOAuthServices() const;

		Wt::WLogEntry log(const std::string &type) const;
		boost::posix_time::ptime StartPTime() const;
		bool Start();

		static WServer *instance();

	protected:
		void ConfigureAuth();

		Wt::Dbo::SqlConnectionPool *SQLPool;

		DboInstaller *Installer;
		ModulesDatabase *Modules;
		ConfigurationsDatabase *Configurations;
		LanguagesDatabase *Languages;
		StylesDatabase *Styles;
		PagesDatabase *Pages;
		AccessPathsDatabase *AccessPaths;

		rapidxml::xml_document<> XmlDoc;
		boost::posix_time::ptime PTBeforeLoad;
		boost::posix_time::ptime PTStart;

	private:
		void CreateWtXmlConfiguration();
		void Initialize();

		Wt::Auth::AuthService AuthService;
		Wt::Auth::PasswordService PasswordService;
		OAuthServiceMap OAuthServices;

		friend int main(int argc, char** argv);
};

#include "Objects/DboInstaller.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"

#endif
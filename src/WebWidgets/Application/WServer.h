#ifndef WW_WSERVER_H
#define WW_WSERVER_H

#include <Wt/WServer>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/OAuthService>

#include <Wt/Dbo/SqlConnectionPool>

#include <3rdparty/rapidxml/rapidxml.hpp>

namespace WW
{
	namespace Installer
	{
		class DboInstaller;
	}
	class WApplication;
	class ModuleDatabase;
	class ConfigurationsDatabase;
	class LanguageDatabase;
	class StyleDatabase;
	class PageDatabase;
	class AccessPathDatabase;
	class NavigationMenuDatabase;
	class DboDatabaseManager;

	typedef std::vector<const Wt::Auth::OAuthService*> OAuthServiceMap;

	class WServer : public Wt::WServer
	{
	public:
		WServer(const std::string& wtApplicationPath = std::string(), const std::string& wtConfigurationFile = std::string());
		virtual ~WServer() override;

		DboDatabaseManager *databaseManager() const { return _dboManager; }
		ModuleDatabase *modules() const { return _modules; }
		ConfigurationsDatabase *configurations() const { return _configurations; }
		LanguageDatabase *languages() const { return _languages; }
		StyleDatabase *styles() const { return _styles; }
		PageDatabase *pages() const { return _pages; }
		AccessPathDatabase *accessPaths() const { return _accessPaths; }
		NavigationMenuDatabase *navigationMenus() const { return _navigationMenus; }

		Wt::Dbo::SqlConnectionPool *sqlPool() const { return _sqlPool; }

		const Wt::Auth::AuthService &getAuthService() const { return _authService; }
		const Wt::Auth::PasswordService &getPasswordService() const { return _passwordService; }
		const OAuthServiceMap &getOAuthServices() const { return _oAuthServices; }

		boost::posix_time::ptime startPTime() const { return _ptStart; }
		bool start();

		static WServer *instance() { return dynamic_cast<WServer*>(Wt::WServer::instance()); }

	protected:
		void configureAuth();
		void createWtXmlConfiguration();
		void initialize();

		void dboDatabaseReloadHandler();

		Wt::Dbo::SqlConnectionPool *_sqlPool = nullptr;

		Installer::DboInstaller *_installer = nullptr;
		DboDatabaseManager *_dboManager = nullptr;
		ModuleDatabase *_modules = nullptr;
		ConfigurationsDatabase *_configurations = nullptr;
		LanguageDatabase *_languages = nullptr;
		StyleDatabase *_styles = nullptr;
		PageDatabase *_pages = nullptr;
		AccessPathDatabase *_accessPaths = nullptr;
		NavigationMenuDatabase *_navigationMenus = nullptr;

		Wt::rapidxml::xml_document<> _xmlDoc;
		boost::posix_time::ptime _ptBeforeLoad;
		boost::posix_time::ptime _ptStart;

		Wt::Auth::AuthService _authService;
		Wt::Auth::PasswordService _passwordService;
		OAuthServiceMap _oAuthServices;

	private:
		friend class DboDatabaseManager;
	};
}

#endif
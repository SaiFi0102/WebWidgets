#include "Application/WServer.h"
#include "Application/WApplication.h"
#include <DboInstaller/DboInstaller.h>
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/LanguageDatabase.h"
#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/StyleDatabase.h"
#include "DboDatabase/PageDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/NavigationMenuDatabase.h"
#include "Objects/DboLocalizedStrings.h"

#include "Pages/TestPage.h"
#include "Pages/AuthPages.h"

#include <fstream>
#include <3rdparty/rapidxml/rapidxml_print.hpp>

#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/GoogleService>
#include <Wt/Auth/FacebookService>

#include <Wt/Dbo/FixedSqlConnectionPool>
#include <Wt/Dbo/backend/MySQL>
#include <Wt/Dbo/backend/Sqlite3>
//#include <Wt/Dbo/backend/Postgres>
//#include <Wt/Dbo/backend/Firebird>

namespace WW
{

WServer::WServer(const std::string &wtApplicationPath, const std::string &wtConfigurationFile)
: Wt::WServer(wtApplicationPath, wtConfigurationFile), _passwordService(_authService)
{
	initialize();
}
void WServer::initialize()
{
	testSignal = new ServerSignal<>(this);
	//Start time
	_ptBeforeLoad = boost::posix_time::microsec_clock::local_time();

	/* *************************************************************************
	 * ***********************  Connect to SQL Server  *************************
	 * *************************************************************************/
	try
	{
		log("info") << "Connecting to database backend";

		//Wt::Dbo::SqlConnection *sqlConnection = new Wt::Dbo::backend::MySQL("wt", "root", "", "127.0.0.1");
		Wt::Dbo::SqlConnection *sqlConnection = new Wt::Dbo::backend::Sqlite3(":memory:");
		sqlConnection->setProperty("show-queries", "true");
		_sqlPool = new Wt::Dbo::FixedSqlConnectionPool(sqlConnection, 1);

		log("success") << "Successfully connected to database";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error connecting to database: " <<  e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error connecting to database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***********************  Initialize Databases  **************************
	 * *************************************************************************/
	try
	{
		_dboManager = new DboDatabaseManager(this, _sqlPool);
		_modules = new ModuleDatabase(_dboManager);
		_configurations = new ConfigurationsDatabase(_dboManager);
		_languages = new LanguageDatabase(_dboManager);
		_styles = new StyleDatabase(_dboManager);
		_pages = new PageDatabase(_dboManager);
		_accessPaths = new AccessPathDatabase(_dboManager);
		_navigationMenus = new NavigationMenuDatabase(_dboManager);
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error initializing databases: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***************************  Create Tables  *****************************
	 * *************************************************************************/
#define REINSTALL_DBO
#ifdef REINSTALL_DBO
	//Drop
	try
	{
		_installer = new Installer::DboInstaller(*_sqlPool);
		_installer->dropTables();
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("error") << "Database error dropping tables: " <<  e.what();
	}
	catch(std::exception &e)
	{
		log("error") << "Error dropping tables: " << e.what();
	}

	//Create
	try
	{
		_installer->createTables();
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error creating tables: " <<  e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error creating tables: " << e.what();
		throw e;
	}

	//Insert
// 	try
// 	{
		_installer->insertRows();
// 	}
// 	catch(Wt::Dbo::Exception &e)
// 	{
// 		log("fatal") << "Database error inserting data: " <<  e.what();
// 		throw e;
// 	}
// 	catch(std::exception &e)
// 	{
// 		log("fatal") << "Error inserting data: " << e.what();
// 		throw e;
// 	}
#endif

	/* *************************************************************************
	 * *************************  Load DboDatabases  ***************************
	 * *************************************************************************/
// 	try
// 	{
		log("info") << "Loading DboDatabaseManager";
		_dboManager->load();
// 	}
// 	catch(Wt::Dbo::Exception &e)
// 	{
// 		log("fatal") << "Database error loading DboDatabaseManager: " << e.what();
// 		throw e;
// 	}
// 	catch(std::exception &e)
// 	{
// 		log("fatal") << "Error loading DboDatabaseManager: " << e.what();
// 		throw e;
// 	}

	//Server localized strings
	setLocalizedStrings(new DboLocalizedStrings(this));

	pages()->register404PageHandler(new Default404Page());
	pages()->registerPageHandler("home", ModuleDatabase::Navigation, new AnotherPage());
	pages()->registerPageHandler("sitemap", ModuleDatabase::Navigation, new AnotherPage());
	pages()->registerPageHandler("login", ModuleDatabase::Authentication, new LoginPage());
	pages()->registerPageHandler("register", ModuleDatabase::Authentication, new RegistrationPage());

	/* *************************************************************************
	 * *********************  Create temporary XML file  ***********************
	 * *************************************************************************/
	try
	{
		log("info") << "Writing XML Configuration file";
		createWtXmlConfiguration();
		log("success") << "XML Configuration file created";
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error creating XML Configuration file: " << e.what();
		throw e;
	}

	//Configure authorization module
	configureAuth();
}

bool WServer::start()
{
	if(Wt::WServer::start())
	{
		//Load Finish Time
		_ptStart = boost::posix_time::microsec_clock::local_time();

		log("success") << "Server successfully started! Time taken to start: "
			<< boost::posix_time::time_duration(_ptStart - _ptBeforeLoad).total_milliseconds()
			<< " ms";
		return true;
	}
	return false;
}

void WServer::configureAuth()
{
	_authService.setAuthTokensEnabled(_configurations->getBool("EnableTokens", ModuleDatabase::Authentication, true), _configurations->getStr("LoginCookie", ModuleDatabase::Authentication, "logintoken"));
	_authService.setEmailVerificationEnabled(_configurations->getBool("EmailVerification", ModuleDatabase::Authentication, false));

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
	_passwordService.setVerifier(verifier);
	_passwordService.setAttemptThrottlingEnabled(true);
	_passwordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

	if(Wt::Auth::GoogleService::configured() && configurations()->getBool("GoogleOAuth", ModuleDatabase::Authentication, false))
		_oAuthServices.push_back(new Wt::Auth::GoogleService(_authService));
	if(Wt::Auth::FacebookService::configured() && configurations()->getBool("FacebookOAuth", ModuleDatabase::Authentication, false))
		_oAuthServices.push_back(new Wt::Auth::FacebookService(_authService));
}

void WServer::createWtXmlConfiguration()
{
	//<server> element
	Wt::rapidxml::xml_node<> *nodeServer = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "server");
	_xmlDoc.append_node(nodeServer);

	//<application-settings> element
	Wt::rapidxml::xml_node<> *nodeAppSett = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "application-settings");
	nodeAppSett->append_attribute(_xmlDoc.allocate_attribute("location", "*"));
	nodeServer->append_node(nodeAppSett);

	//<session-management> element
	Wt::rapidxml::xml_node<> *nodeSessMgmt = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "session-management");
	nodeAppSett->append_node(nodeSessMgmt);

	//<session-management> child element <shared-process>
	Wt::rapidxml::xml_node<> *nodeSharedProcess = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "shared-process");
	nodeSessMgmt->append_node(nodeSharedProcess);

	//<shared-process> child element <num-processes>
	nodeSharedProcess->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "num-processes", "1"));

	//<session-management> child element <tracking>
	Wt::rapidxml::xml_node<> *nodeTracking = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "tracking");
	switch(_configurations->getEnum("SessionTracking", ModuleDatabase::Server, 1))
	{
		default:
		case 1: nodeTracking->value("Auto"); break;
		case 2: nodeTracking->value("URL"); break;
	}
	nodeSessMgmt->append_node(nodeTracking);

	//<session-management> child element <reload-is-new-session>
	nodeSessMgmt->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "reload-is-new-session", "false"));

	//<session-management> child element <timeout>
	int confSessionTimeout = _configurations->getInt("SessionTimeout", ModuleDatabase::Server, 600);
	if(confSessionTimeout < 60) confSessionTimeout = 60;
	std::string confSessionTimeoutStr = boost::lexical_cast<std::string>(confSessionTimeout);
	nodeSessMgmt->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "timeout", confSessionTimeoutStr.c_str()));

	//<connector-fcgi> element
	Wt::rapidxml::xml_node<> *nodeConnectorFcgi = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "connector-fcgi");
	nodeAppSett->append_node(nodeConnectorFcgi);

	//<connector-fcgi> child element <run-directory>
	nodeConnectorFcgi->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "run-directory", "C:/witty")); //TODO Gotta understand this first

	//<connector-fcgi> child element <num-threads>
	nodeConnectorFcgi->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "num-threads", "5")); //TODO Gotta understand this first

	//<connector-isapi> element
	Wt::rapidxml::xml_node<> *nodeConnectorIsapi = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "connector-isapi");
	nodeAppSett->append_node(nodeConnectorIsapi);

	//<connector-isapi> child element <num-threads>
	nodeConnectorIsapi->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "num-threads", "10")); //TODO Gotta understand this first

	//<connector-isapi> child element <max-memory-request-size>
	nodeConnectorIsapi->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "max-memory-request-size", "128")); //TODO Gotta understand this first

	//<debug> element
	switch(_configurations->getEnum("JavascriptDebug", ModuleDatabase::Server, 1))
	{
		default:
		case 1: nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "false")); break;
		case 2: nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "true")); break;
		case 3: nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "naked")); break;
	}

	//<log-file> element
	std::string logFileStr = _configurations->getStr("LogDirectory", ModuleDatabase::Logging) + "/server.log";
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "log-file", logFileStr.c_str()));

	//<log-config> element
	std::string logConfigStr = "*";
	if(!_configurations->getBool("LogDebugLevel", ModuleDatabase::Logging, false))
		logConfigStr += " -debug";
	if(!_configurations->getBool("LogInfoLevel", ModuleDatabase::Logging, false))
		logConfigStr += " -info";
	if(!_configurations->getBool("LogWarnLevel", ModuleDatabase::Logging, true))
		logConfigStr += " -warning";
	if(!_configurations->getBool("LogSecureLevel", ModuleDatabase::Logging, true))
		logConfigStr += " -secure";
	if(!_configurations->getBool("LogErrorLevel", ModuleDatabase::Logging, true))
		logConfigStr += " -error";
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "log-config", logConfigStr.c_str()));

	//<max-request-size> element
	std::string maxReqSizeStr = boost::lexical_cast<std::string>(_configurations->getInt("MaxRequestSize", ModuleDatabase::Server, 128));
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "max-request-size", maxReqSizeStr.c_str()));

	//<ajax-puzzle> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "ajax-puzzle", _configurations->getBool("DosPuzzle", ModuleDatabase::Server, false) ? "true" : "false"));

	//<strict-event-serialization> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "strict-event-serialization", "false")); //TODO: Gotta understand this first

	//<webgl-detection> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "webgl-detection", "false")); //TODO: Configuration to change detection

	//<redirect-message> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "redirect-message", "Click here if the page does not refreshes."));

	//<behind-reverse-proxy> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "behind-reverse-proxy", _configurations->getBool("ReverseProxy", ModuleDatabase::Server, false) ? "true" : "false"));

	//<progressive-bootstrap> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "progressive-bootstrap", "false"));

	//<session-id-cookie> element
	nodeAppSett->append_node(_xmlDoc.allocate_node(Wt::rapidxml::node_element, "session-id-cookie", "true"));

	//<properties> element
	Wt::rapidxml::xml_node<> *nodeProperties = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "properties");
	nodeAppSett->append_node(nodeProperties);

	std::string propertyBaseUrl = _configurations->getStr("BaseURL", ModuleDatabase::Server);
	if(!propertyBaseUrl.empty())
	{
		Wt::rapidxml::xml_node<> *nodePropertyBaseURL = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", propertyBaseUrl.c_str());
		nodePropertyBaseURL->append_attribute(_xmlDoc.allocate_attribute("name", "baseURL"));
		nodeProperties->append_node(nodePropertyBaseURL);
	}

	std::string resourceUrlStr =  _configurations->getStr("ResourcesURL", ModuleDatabase::Styling, "resources/");
	Wt::rapidxml::xml_node<> *nodePropertyResourceURL = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", resourceUrlStr.c_str());
	nodePropertyResourceURL->append_attribute(_xmlDoc.allocate_attribute("name", "resourcesURL"));
	nodeProperties->append_node(nodePropertyResourceURL);

	//Google OAuth
	std::string googleClientIdStr = _configurations->getStr("GoogleClientId", ModuleDatabase::Authentication);
	std::string googleClientSecretStr = _configurations->getStr("GoogleClientSecret", ModuleDatabase::Authentication);
	std::string redirectStr = propertyBaseUrl + "oauth2callback";
	if(_configurations->getBool("GoogleOAuth", ModuleDatabase::Authentication, false) && !googleClientIdStr.empty() && !googleClientSecretStr.empty())
	{
		Wt::rapidxml::xml_node<> *nodeGoogleClientId = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", googleClientIdStr.c_str());
		nodeGoogleClientId->append_attribute(_xmlDoc.allocate_attribute("name", "google-oauth2-client-id"));
		nodeProperties->append_node(nodeGoogleClientId);

		Wt::rapidxml::xml_node<> *nodeGoogleClientSecret = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", googleClientSecretStr.c_str());
		nodeGoogleClientSecret->append_attribute(_xmlDoc.allocate_attribute("name", "google-oauth2-client-secret"));
		nodeProperties->append_node(nodeGoogleClientSecret);

		Wt::rapidxml::xml_node<> *nodeGoogleRedirect = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", redirectStr.c_str());
		nodeGoogleRedirect->append_attribute(_xmlDoc.allocate_attribute("name", "google-oauth2-redirect-endpoint"));
		nodeProperties->append_node(nodeGoogleRedirect);
	}

	//Facebook OAuth
	std::string facebookAppIdStr = _configurations->getStr("FacebookAppId", ModuleDatabase::Authentication);
	std::string facebookAppSecretStr = _configurations->getStr("FacebookAppSecret", ModuleDatabase::Authentication);
	if(_configurations->getBool("FacebookOAuth", ModuleDatabase::Authentication, false) && !facebookAppIdStr.empty() && !facebookAppSecretStr.empty())
	{
		Wt::rapidxml::xml_node<> *nodeFacebookAppId = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", facebookAppIdStr.c_str());
		nodeFacebookAppId->append_attribute(_xmlDoc.allocate_attribute("name", "facebook-oauth2-app-id"));
		nodeProperties->append_node(nodeFacebookAppId);

		Wt::rapidxml::xml_node<> *nodeFacebookAppSecret = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", facebookAppSecretStr.c_str());
		nodeFacebookAppSecret->append_attribute(_xmlDoc.allocate_attribute("name", "facebook-oauth2-app-secret"));
		nodeProperties->append_node(nodeFacebookAppSecret);

		Wt::rapidxml::xml_node<> *nodeGoogleRedirect = _xmlDoc.allocate_node(Wt::rapidxml::node_element, "property", redirectStr.c_str());
		nodeGoogleRedirect->append_attribute(_xmlDoc.allocate_attribute("name", "facebook-oauth2-redirect-endpoint"));
		nodeProperties->append_node(nodeGoogleRedirect);
	}

	//Write to file
	std::ofstream xmlFile("wt_config.xml", std::ios::trunc);
	xmlFile << _xmlDoc;
	xmlFile.close();
}

WServer::~WServer()
{
	//Deallocate
	for(OAuthServiceMap::size_type i = 0; i < _oAuthServices.size(); ++i)
		delete _oAuthServices[i];

	delete _installer;
	delete _dboManager; //Also deletes DboDatabases
	delete _sqlPool; //Also deletes SQLConnections
}

void WServer::dboDatabaseReloadHandler()
{
	postAll(std::bind(&WApplication::refreshDdoPtrs));
}

}

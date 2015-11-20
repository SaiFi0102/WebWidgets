#include "Application/WServer.h"
#include "Application/Application.h"
#include "DboInstaller/DboInstaller.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "Objects/DboLocalizedStrings.h"

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

WServer::WServer(const std::string &wtApplicationPath, const std::string &wtConfigurationFile)
	: Wt::WServer(wtApplicationPath, wtConfigurationFile), PasswordService(AuthService),
	SQLPool(0), _AccessPaths(0), _Configurations(0), _DboManager(0),
	_Installer(0), _Languages(0), _Modules(0), _Pages(0), _Styles(0)
{ }
void WServer::Initialize()
{
	//Start time
	PTBeforeLoad = boost::posix_time::microsec_clock::local_time();

	/* *************************************************************************
	 * ***********************  Connect to SQL Server  *************************
	 * *************************************************************************/
	try
	{
		log("info") << "Connecting to database backend";

		//Wt::Dbo::SqlConnection *SQLConnection = new Wt::Dbo::backend::MySQL("wt", "root", "", "127.0.0.1");
		Wt::Dbo::SqlConnection *SQLConnection = new Wt::Dbo::backend::Sqlite3(":memory:");
		SQLConnection->setProperty("show-queries", "false");
		SQLPool = new Wt::Dbo::FixedSqlConnectionPool(SQLConnection, 1);

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
		_DboManager = new DboDatabaseManager(this, SQLPool);
		_Modules = new ModulesDatabase(_DboManager);
		_Configurations = new ConfigurationsDatabase(_DboManager);
		_Languages = new LanguagesDatabase(_DboManager);
		_Styles = new StylesDatabase(_DboManager);
		_Pages = new PagesDatabase(_DboManager);
		_AccessPaths = new AccessPathsDatabase(_DboManager);
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error initializing databases: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***************************  Create Tables  *****************************
	 * *************************************************************************/
	bool REINSTALLDBO = true;
	if(REINSTALLDBO)
	{
		//Drop
		try
		{
			_Installer = new DboInstaller(*SQLPool);
			_Installer->DropTables();
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
			_Installer->CreateTables();
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
// 		try
// 		{
			_Installer->InsertRows();
// 		}
// 		catch(Wt::Dbo::Exception &e)
// 		{
// 			log("fatal") << "Database error inserting default data: " <<  e.what();
// 			throw e;
// 		}
// 		catch(std::exception &e)
// 		{
// 			log("fatal") << "Error inserting default data: " << e.what();
// 			throw e;
// 		}
	}

	/* *************************************************************************
	 * *************************  Load DboDatabases  ***************************
	 * *************************************************************************/
	try
	{
		log("info") << "Loading DboDatabaseManager";
		_DboManager->Load();
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading DboDatabaseManager: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error loading DboDatabaseManager: " << e.what();
		throw e;
	}

	//Server localized strings
	setLocalizedStrings(new DboLocalizedStrings(this));

	/* *************************************************************************
	 * *********************  Create temporary XML file  ***********************
	 * *************************************************************************/
	try
	{
		log("info") << "Writing XML Configuration file";
		CreateWtXmlConfiguration();
		log("success") << "XML Configuration file created";
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error creating XML Configuration file: " << e.what();
		throw e;
	}

	//Configure authorization module
	ConfigureAuth();
}

bool WServer::Start()
{
	if(start())
	{
		//Load Finish Time
		PTStart = boost::posix_time::microsec_clock::local_time();

		log("success") << "Server successfully started! Time taken to start: "
			<< boost::posix_time::time_duration(PTStart - PTBeforeLoad).total_milliseconds()
			<< " ms";
		return true;
	}
	return false;
}

void WServer::ConfigureAuth()
{
	AuthService.setAuthTokensEnabled(_Configurations->GetBool("EnableTokens", ModulesDatabase::Authentication, true), _Configurations->GetStr("LoginCookie", ModulesDatabase::Authentication, "logintoken"));
	AuthService.setEmailVerificationEnabled(_Configurations->GetBool("EmailVerification", ModulesDatabase::Authentication, false));

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
	PasswordService.setVerifier(verifier);
	PasswordService.setAttemptThrottlingEnabled(true);
	PasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

	if(Wt::Auth::GoogleService::configured() && Configurations()->GetBool("GoogleOAuth", ModulesDatabase::Authentication, false))
	{
		OAuthServices.push_back(new Wt::Auth::GoogleService(AuthService));
	}
	if(Wt::Auth::FacebookService::configured() && Configurations()->GetBool("FacebookOAuth", ModulesDatabase::Authentication, false))
	{
		OAuthServices.push_back(new Wt::Auth::FacebookService(AuthService));
	}
}

void WServer::CreateWtXmlConfiguration()
{
	//<server> element
	Wt::rapidxml::xml_node<> *NodeServer = XmlDoc.allocate_node(Wt::rapidxml::node_element, "server");
	XmlDoc.append_node(NodeServer);

	//<application-settings> element
	Wt::rapidxml::xml_node<> *NodeAppSett = XmlDoc.allocate_node(Wt::rapidxml::node_element, "application-settings");
	NodeAppSett->append_attribute(XmlDoc.allocate_attribute("location", "*"));
	NodeServer->append_node(NodeAppSett);

	//<session-management> element
	Wt::rapidxml::xml_node<> *NodeSessMgmt = XmlDoc.allocate_node(Wt::rapidxml::node_element, "session-management");
	NodeAppSett->append_node(NodeSessMgmt);

	//<session-management> child element <shared-process>
	Wt::rapidxml::xml_node<> *NodeSharedProcess = XmlDoc.allocate_node(Wt::rapidxml::node_element, "shared-process");
	NodeSessMgmt->append_node(NodeSharedProcess);

	//<shared-process> child element <num-processes>
	NodeSharedProcess->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "num-processes", "1"));

	//<session-management> child element <tracking>
	Wt::rapidxml::xml_node<> *NodeTracking = XmlDoc.allocate_node(Wt::rapidxml::node_element, "tracking");
	switch(_Configurations->GetEnum("SessionTracking", ModulesDatabase::Server, 1))
	{
		default:
		case 1:
			NodeTracking->value("Auto");
		break;
		case 2:
			NodeTracking->value("URL");
		break;
	}
	NodeSessMgmt->append_node(NodeTracking);

	//<session-management> child element <reload-is-new-session>
	NodeSessMgmt->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "reload-is-new-session", "false"));

	//<session-management> child element <timeout>
	int ConfSessionTimeout = _Configurations->GetInt("SessionTimeout", ModulesDatabase::Server, 600);
	if(ConfSessionTimeout < 60) ConfSessionTimeout = 60;
	std::string ConfSessionTimeoutStr = boost::lexical_cast<std::string>(ConfSessionTimeout);
	NodeSessMgmt->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "timeout", ConfSessionTimeoutStr.c_str()));

	//<connector-fcgi> element
	Wt::rapidxml::xml_node<> *NodeConnectorFcgi = XmlDoc.allocate_node(Wt::rapidxml::node_element, "connector-fcgi");
	NodeAppSett->append_node(NodeConnectorFcgi);

	//<connector-fcgi> child element <run-directory>
	NodeConnectorFcgi->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "run-directory", "C:/witty")); //TODO Gotta understand this first

	//<connector-fcgi> child element <num-threads>
	NodeConnectorFcgi->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "num-threads", "5")); //TODO Gotta understand this first

	//<connector-isapi> element
	Wt::rapidxml::xml_node<> *NodeConnectorIsapi = XmlDoc.allocate_node(Wt::rapidxml::node_element, "connector-isapi");
	NodeAppSett->append_node(NodeConnectorIsapi);

	//<connector-isapi> child element <num-threads>
	NodeConnectorIsapi->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "num-threads", "10")); //TODO Gotta understand this first

	//<connector-isapi> child element <max-memory-request-size>
	NodeConnectorIsapi->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "max-memory-request-size", "128")); //TODO Gotta understand this first

	//<debug> element
	switch(_Configurations->GetEnum("JavascriptDebug", ModulesDatabase::Server, 1))
	{
		default:
		case 1:
			NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "false"));
		break;
		case 2:
			NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "true"));
		break;
		case 3:
			NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "debug", "naked"));
		break;
	}

	//<log-file> element
	std::string LogFileStr = _Configurations->GetStr("LogDirectory", ModulesDatabase::Logging) + "/server.log";
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "log-file", LogFileStr.c_str()));

	//<log-config> element
	std::string LogConfigStr = "*";
	if(!_Configurations->GetBool("LogDebugLevel", ModulesDatabase::Logging, false))
	{
		LogConfigStr += " -debug";
	}
	if(!_Configurations->GetBool("LogInfoLevel", ModulesDatabase::Logging, false))
	{
		LogConfigStr += " -info";
	}
	if(!_Configurations->GetBool("LogWarnLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -warning";
	}
	if(!_Configurations->GetBool("LogSecureLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -secure";
	}
	if(!_Configurations->GetBool("LogErrorLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -error";
	}
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "log-config", LogConfigStr.c_str()));

	//<max-request-size> element
	std::string MaxReqSizeStr = boost::lexical_cast<std::string>(_Configurations->GetInt("MaxRequestSize", ModulesDatabase::Server, 128));
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "max-request-size", MaxReqSizeStr.c_str()));

	//<ajax-puzzle> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "ajax-puzzle", _Configurations->GetBool("DosPuzzle", ModulesDatabase::Server, false) ? "true" : "false"));

	//<strict-event-serialization> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "strict-event-serialization", "false")); //TODO: Gotta understand this first

	//<webgl-detection> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "webgl-detection", "false")); //TODO: Configuration to change detection

	//<redirect-message> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "redirect-message", "Click here if the page does not refreshes."));

	//<behind-reverse-proxy> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "behind-reverse-proxy", _Configurations->GetBool("ReverseProxy", ModulesDatabase::Server, false) ? "true" : "false"));

	//<progressive-bootstrap> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "progressive-bootstrap", "false"));

	//<session-id-cookie> element
	NodeAppSett->append_node(XmlDoc.allocate_node(Wt::rapidxml::node_element, "session-id-cookie", "true"));

	//<properties> element
	Wt::rapidxml::xml_node<> *NodeProperties = XmlDoc.allocate_node(Wt::rapidxml::node_element, "properties");
	NodeAppSett->append_node(NodeProperties);

	std::string PropertyBaseUrl = _Configurations->GetStr("BaseURL", ModulesDatabase::Server);
	if(!PropertyBaseUrl.empty())
	{
		Wt::rapidxml::xml_node<> *NodePropertyBaseURL = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", PropertyBaseUrl.c_str());
		NodePropertyBaseURL->append_attribute(XmlDoc.allocate_attribute("name", "baseURL"));
		NodeProperties->append_node(NodePropertyBaseURL);
	}

	std::string ResourceUrlStr =  _Configurations->GetStr("ResourcesURL", ModulesDatabase::Styles, "resources/");
	Wt::rapidxml::xml_node<> *NodePropertyResourceURL = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", ResourceUrlStr.c_str());
	NodePropertyResourceURL->append_attribute(XmlDoc.allocate_attribute("name", "resourcesURL"));
	NodeProperties->append_node(NodePropertyResourceURL);

	//Google OAuth
	std::string GoogleClientIdStr = _Configurations->GetStr("GoogleClientId", ModulesDatabase::Authentication);
	std::string GoogleClientSecretStr = _Configurations->GetStr("GoogleClientSecret", ModulesDatabase::Authentication);
	std::string RedirectStr = PropertyBaseUrl + "oauth2callback";
	if(_Configurations->GetBool("GoogleOAuth", ModulesDatabase::Authentication, false) && !GoogleClientIdStr.empty() && !GoogleClientSecretStr.empty())
	{
		Wt::rapidxml::xml_node<> *NodeGoogleClientId = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", GoogleClientIdStr.c_str());
		NodeGoogleClientId->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-client-id"));
		NodeProperties->append_node(NodeGoogleClientId);

		Wt::rapidxml::xml_node<> *NodeGoogleClientSecret = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", GoogleClientSecretStr.c_str());
		NodeGoogleClientSecret->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-client-secret"));
		NodeProperties->append_node(NodeGoogleClientSecret);

		Wt::rapidxml::xml_node<> *NodeGoogleRedirect = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", RedirectStr.c_str());
		NodeGoogleRedirect->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-redirect-endpoint"));
		NodeProperties->append_node(NodeGoogleRedirect);
	}

	//Facebook OAuth
	std::string FacebookAppIdStr = _Configurations->GetStr("FacebookAppId", ModulesDatabase::Authentication);
	std::string FacebookAppSecretStr = _Configurations->GetStr("FacebookAppSecret", ModulesDatabase::Authentication);
	if(_Configurations->GetBool("FacebookOAuth", ModulesDatabase::Authentication, false) && !FacebookAppIdStr.empty() && !FacebookAppSecretStr.empty())
	{
		Wt::rapidxml::xml_node<> *NodeFacebookAppId = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", FacebookAppIdStr.c_str());
		NodeFacebookAppId->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-app-id"));
		NodeProperties->append_node(NodeFacebookAppId);

		Wt::rapidxml::xml_node<> *NodeFacebookAppSecret = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", FacebookAppSecretStr.c_str());
		NodeFacebookAppSecret->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-app-secret"));
		NodeProperties->append_node(NodeFacebookAppSecret);

		Wt::rapidxml::xml_node<> *NodeGoogleRedirect = XmlDoc.allocate_node(Wt::rapidxml::node_element, "property", RedirectStr.c_str());
		NodeGoogleRedirect->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-redirect-endpoint"));
		NodeProperties->append_node(NodeGoogleRedirect);
	}

	//Write to file
	std::ofstream XmlFile("wt_config.xml", std::ios::trunc);
	XmlFile << XmlDoc;
	XmlFile.close();
}

WServer::~WServer()
{
	//Deallocate
	for(OAuthServiceMap::size_type i = 0; i < OAuthServices.size(); ++i)
	{
		delete OAuthServices[i];
	}

	delete _Installer;
	delete _DboManager; //Also deletes DboDatabases
	delete SQLPool; //Also deletes SQLConnections
}

void WServer::DboDatabaseReloadHandler()
{
	postAll(boost::bind(&Application::RefreshDboDatabasePtrs));
}

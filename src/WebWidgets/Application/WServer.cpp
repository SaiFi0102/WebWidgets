#include "Application/WServer.h"
#include "DboInstaller/DboInstaller.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"

#include <fstream>
#include <rapidxml/rapidxml_print.hpp>

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
	: Wt::WServer(wtApplicationPath, wtConfigurationFile), PasswordService(AuthService)
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
		log("info") << "Connecting to database";

		Wt::Dbo::SqlConnection *SQLConnection = new Wt::Dbo::backend::MySQL("wt", "root", "", "127.0.0.1");
		//Wt::Dbo::SqlConnection *SQLConnection = new Wt::Dbo::backend::Sqlite3(":memory:");
		SQLConnection->setProperty("show-queries", "true");
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
		log("fatal") << "Error while connecting to database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***********************  Initialize Databases  **************************
	 * *************************************************************************/
	try
	{
		Modules = new ModulesDatabase(*SQLPool, *this);
		Configurations = new ConfigurationsDatabase(*SQLPool, *this);
		Languages = new LanguagesDatabase(*SQLPool, *this);
		Styles = new StylesDatabase(*SQLPool, *this);
		Pages = new PagesDatabase(*SQLPool, *this);
		AccessPaths = new AccessPathsDatabase(*SQLPool, *this);
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while initializing databases: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***************************  Create Tables  *****************************
	 * *************************************************************************/
	bool REINSTALLDBO = false;
	if(REINSTALLDBO)
	{
		//Drop
		try
		{
			Installer = new DboInstaller(*SQLPool);
			Installer->DropTables();
		}
		catch(Wt::Dbo::Exception &e)
		{
			log("fatal") << "Database error dropping tables: " <<  e.what();
			//throw e;
		}
		catch(std::exception &e)
		{
			log("fatal") << "Error while dropping tables: " << e.what();
			//throw e;
		}

		//Create
		try
		{
			Installer->CreateTables();
		}
		catch(Wt::Dbo::Exception &e)
		{
			log("fatal") << "Database error creating tables: " <<  e.what();
			throw e;
		}
		catch(std::exception &e)
		{
			log("fatal") << "Error while creating tables: " << e.what();
			throw e;
		}

		//Insert
		try
		{
			Installer->InsertRows();
		}
		catch(Wt::Dbo::Exception &e)
		{
			log("fatal") << "Database error inserting default data: " <<  e.what();
			throw e;
		}
		catch(std::exception &e)
		{
			log("fatal") << "Error while inserting default data: " << e.what();
			throw e;
		}
	}

	/* *************************************************************************
	 * ******************************  Modules  ********************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading modules from database";
		Modules->FetchAll();
		log("success") << "Modules: " << Modules->CountModules() << " entries successfully loaded in " << Modules->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading modules from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading modules from database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ***************************  Configurations  ****************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading configurations from database";
		Configurations->FetchAll();
		log("success") << "Configurations: " << Configurations->CountConfigurations() << " entries successfully loaded in " << Configurations->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading configurations from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading configurations from database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * *****************************  Languages  *******************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading languages from database";
		Languages->FetchAll();
		log("success") << "Languages: " << Languages->CountSingle() << " Single and " << Languages->CountPlural() << " Plural entries from " << Languages->CountLanguages() << " Languages successfully loaded in " << Languages->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading languages from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading languages from database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ******************************  Styles  *********************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading styles/templates from database";
		Styles->FetchAll();
		log("success") << "Styles: " << Styles->CountStyles() << " Styles, " << Styles->CountTemplates() << " Templates, " << Styles->CountStyleTemplates() << " Styled Templates, " << Styles->CountStyleCssRules() << " Style CSS Rules and " << Styles->CountTemplateCssRules() << " Template CSS Rules successfully loaded in " << Styles->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading styles/templates from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading styles/templates from database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * *******************************  Pages  *********************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading pages from database";
		Pages->FetchAll();
		log("success") << "Pages: " << Pages->CountPages() << " Page entires successfully loaded in " << Pages->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading pages from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading pages from database: " << e.what();
		throw e;
	}

	/* *************************************************************************
	 * ****************************  Access Paths  *****************************
	 * *************************************************************************/
	//Fetch
	try
	{
		log("info") << "Loading access paths from database";
		AccessPaths->FetchAll();
		log("success") << "AccessPaths: " << AccessPaths->CountAccessPaths() << " Access Path entires successfully loaded in " << AccessPaths->GetLoadDurationinMS() << " Ms";
	}
	catch(Wt::Dbo::Exception &e)
	{
		log("fatal") << "Database error loading access paths from database: " << e.what();
		throw e;
	}
	catch(std::exception &e)
	{
		log("fatal") << "Error while loading access paths from database: " << e.what();
		throw e;
	}

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

ModulesDatabase *WServer::GetModules() const
{
	return Modules;
}
ConfigurationsDatabase *WServer::GetConfigurations() const
{
	return Configurations;
}
LanguagesDatabase *WServer::GetLanguages() const
{
	return Languages;
}
StylesDatabase *WServer::GetStyles() const
{
	return Styles;
}
PagesDatabase *WServer::GetPages() const
{
	return Pages;
}
AccessPathsDatabase *WServer::GetAccessPaths() const
{
	return AccessPaths;
}

const Wt::Auth::AuthService &WServer::GetAuthService() const
{
	return AuthService;
}
const Wt::Auth::PasswordService &WServer::GetPasswordService() const
{
	return PasswordService;
}
const OAuthServiceMap &WServer::GetOAuthServices() const
{
	return OAuthServices;
}

Wt::WLogEntry WServer::log(const std::string &type) const
{
	Wt::WApplication *app = Wt::WApplication::instance();

	if(app)
	{
		return app->log(type);
	}
	return Wt::WServer::log(type);
}

boost::posix_time::ptime WServer::StartPTime() const
{
	return PTStart;
}

bool WServer::Start()
{
	if(start())
	{
		//Load Finish Time
		PTStart = boost::posix_time::microsec_clock::local_time();

		log("success") << "Server successfully started! Time taken to start: "
			<< boost::posix_time::time_duration(PTStart - PTBeforeLoad).total_milliseconds()
			<< " MS";
		return true;
	}
	return false;
}

void WServer::ConfigureAuth()
{
	AuthService.setAuthTokensEnabled(Configurations->GetBool("EnableTokens", ModulesDatabase::Authentication, true), Configurations->GetStr("LoginCookie", ModulesDatabase::Authentication, "logintoken"));
	AuthService.setEmailVerificationEnabled(Configurations->GetBool("EmailVerification", ModulesDatabase::Authentication, false));

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
	PasswordService.setVerifier(verifier);
	PasswordService.setAttemptThrottlingEnabled(true);
	PasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

	if(Wt::Auth::GoogleService::configured() && GetConfigurations()->GetBool("GoogleOAuth", ModulesDatabase::Authentication, false))
	{
		OAuthServices.push_back(new Wt::Auth::GoogleService(AuthService));
	}
	if(Wt::Auth::FacebookService::configured() && GetConfigurations()->GetBool("FacebookOAuth", ModulesDatabase::Authentication, false))
	{
		OAuthServices.push_back(new Wt::Auth::FacebookService(AuthService));
	}
}

void WServer::CreateWtXmlConfiguration()
{
	//<server> element
	rapidxml::xml_node<> *NodeServer = XmlDoc.allocate_node(rapidxml::node_element, "server");
	XmlDoc.append_node(NodeServer);

	//<application-settings> element
	rapidxml::xml_node<> *NodeAppSett = XmlDoc.allocate_node(rapidxml::node_element, "application-settings");
	NodeAppSett->append_attribute(XmlDoc.allocate_attribute("location", "*"));
	NodeServer->append_node(NodeAppSett);

	//<session-management> element
	rapidxml::xml_node<> *NodeSessMgmt = XmlDoc.allocate_node(rapidxml::node_element, "session-management");
	NodeAppSett->append_node(NodeSessMgmt);

	//<session-management> child element <shared-process>
	rapidxml::xml_node<> *NodeSharedProcess = XmlDoc.allocate_node(rapidxml::node_element, "shared-process");
	NodeSessMgmt->append_node(NodeSharedProcess);

	//<shared-process> child element <num-processes>
	NodeSharedProcess->append_node(XmlDoc.allocate_node(rapidxml::node_element, "num-processes", "1"));

	//<session-management> child element <tracking>
	rapidxml::xml_node<> *NodeTracking = XmlDoc.allocate_node(rapidxml::node_element, "tracking");
	switch(Configurations->GetEnum("SessionTracking", ModulesDatabase::Server, 1))
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
	NodeSessMgmt->append_node(XmlDoc.allocate_node(rapidxml::node_element, "reload-is-new-session", "false"));

	//<session-management> child element <timeout>
	int ConfSessionTimeout = Configurations->GetInt("SessionTimeout", ModulesDatabase::Server, 600);
	if(ConfSessionTimeout < 60) ConfSessionTimeout = 60;
	std::string ConfSessionTimeoutStr = boost::lexical_cast<std::string>(ConfSessionTimeout);
	NodeSessMgmt->append_node(XmlDoc.allocate_node(rapidxml::node_element, "timeout", ConfSessionTimeoutStr.c_str()));

	//<connector-fcgi> element
	rapidxml::xml_node<> *NodeConnectorFcgi = XmlDoc.allocate_node(rapidxml::node_element, "connector-fcgi");
	NodeAppSett->append_node(NodeConnectorFcgi);

	//<connector-fcgi> child element <run-directory>
	NodeConnectorFcgi->append_node(XmlDoc.allocate_node(rapidxml::node_element, "run-directory", "C:/witty")); //TODO Gotta understand this first

	//<connector-fcgi> child element <num-threads>
	NodeConnectorFcgi->append_node(XmlDoc.allocate_node(rapidxml::node_element, "num-threads", "5")); //TODO Gotta understand this first

	//<connector-isapi> element
	rapidxml::xml_node<> *NodeConnectorIsapi = XmlDoc.allocate_node(rapidxml::node_element, "connector-isapi");
	NodeAppSett->append_node(NodeConnectorIsapi);

	//<connector-isapi> child element <num-threads>
	NodeConnectorIsapi->append_node(XmlDoc.allocate_node(rapidxml::node_element, "num-threads", "10")); //TODO Gotta understand this first

	//<connector-isapi> child element <max-memory-request-size>
	NodeConnectorIsapi->append_node(XmlDoc.allocate_node(rapidxml::node_element, "max-memory-request-size", "128")); //TODO Gotta understand this first

	//<debug> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "debug", "false"));

	//<log-file> element
	std::string LogFileStr = Configurations->GetStr("LogDirectory", ModulesDatabase::Logging) + "/server.log";
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "log-file", LogFileStr.c_str()));

	//<log-config> element
	std::string LogConfigStr = "*";
	if(!Configurations->GetBool("LogDebugLevel", ModulesDatabase::Logging, false))
	{
		LogConfigStr += " -debug";
	}
	if(!Configurations->GetBool("LogInfoLevel", ModulesDatabase::Logging, false))
	{
		LogConfigStr += " -info";
	}
	if(!Configurations->GetBool("LogWarnLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -warning";
	}
	if(!Configurations->GetBool("LogSecureLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -secure";
	}
	if(!Configurations->GetBool("LogErrorLevel", ModulesDatabase::Logging, true))
	{
		LogConfigStr += " -error";
	}
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "log-config", LogConfigStr.c_str()));

	//<max-request-size> element
	std::string MaxReqSizeStr = boost::lexical_cast<std::string>(Configurations->GetInt("MaxRequestSize", ModulesDatabase::Server, 128));
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "max-request-size", MaxReqSizeStr.c_str()));

	//<ajax-puzzle> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "ajax-puzzle", Configurations->GetBool("DosPuzzle", ModulesDatabase::Server, false) ? "true" : "false"));

	//<strict-event-serialization> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "strict-event-serialization", "false")); //TODO: Gotta understand this first

	//<webgl-detection> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "webgl-detection", "false")); //TODO: Configuration to change detection

	//<redirect-message> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "redirect-message", "Click here if the page does not refreshes."));

	//<behind-reverse-proxy> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "behind-reverse-proxy", Configurations->GetBool("ReverseProxy", ModulesDatabase::Server, false) ? "true" : "false"));

	//<progressive-bootstrap> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "progressive-bootstrap", "false"));

	//<session-id-cookie> element
	NodeAppSett->append_node(XmlDoc.allocate_node(rapidxml::node_element, "session-id-cookie", "true"));

	//<properties> element
	rapidxml::xml_node<> *NodeProperties = XmlDoc.allocate_node(rapidxml::node_element, "properties");
	NodeAppSett->append_node(NodeProperties);

	std::string PropertyBaseUrl = Configurations->GetStr("BaseURL", ModulesDatabase::Server);
	if(!PropertyBaseUrl.empty())
	{
		rapidxml::xml_node<> *NodePropertyBaseURL = XmlDoc.allocate_node(rapidxml::node_element, "property", PropertyBaseUrl.c_str());
		NodePropertyBaseURL->append_attribute(XmlDoc.allocate_attribute("name", "baseURL"));
		NodeProperties->append_node(NodePropertyBaseURL);
	}

	std::string ResourceUrlStr =  Configurations->GetStr("ResourcesURL", ModulesDatabase::Styles, "resources/");
	rapidxml::xml_node<> *NodePropertyResourceURL = XmlDoc.allocate_node(rapidxml::node_element, "property", ResourceUrlStr.c_str());
	NodePropertyResourceURL->append_attribute(XmlDoc.allocate_attribute("name", "resourcesURL"));
	NodeProperties->append_node(NodePropertyResourceURL);

	//Google OAuth
	std::string GoogleClientIdStr = Configurations->GetStr("GoogleClientId", ModulesDatabase::Authentication);
	std::string GoogleClientSecretStr = Configurations->GetStr("GoogleClientSecret", ModulesDatabase::Authentication);
	std::string RedirectStr = PropertyBaseUrl + "oauth2callback";
	if(Configurations->GetBool("GoogleOAuth", ModulesDatabase::Authentication, false) && !GoogleClientIdStr.empty() && !GoogleClientSecretStr.empty())
	{
		rapidxml::xml_node<> *NodeGoogleClientId = XmlDoc.allocate_node(rapidxml::node_element, "property", GoogleClientIdStr.c_str());
		NodeGoogleClientId->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-client-id"));
		NodeProperties->append_node(NodeGoogleClientId);

		rapidxml::xml_node<> *NodeGoogleClientSecret = XmlDoc.allocate_node(rapidxml::node_element, "property", GoogleClientSecretStr.c_str());
		NodeGoogleClientSecret->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-client-secret"));
		NodeProperties->append_node(NodeGoogleClientSecret);

		rapidxml::xml_node<> *NodeGoogleRedirect = XmlDoc.allocate_node(rapidxml::node_element, "property", RedirectStr.c_str());
		NodeGoogleRedirect->append_attribute(XmlDoc.allocate_attribute("name", "google-oauth2-redirect-endpoint"));
		NodeProperties->append_node(NodeGoogleRedirect);
	}

	//Facebook OAuth
	std::string FacebookAppIdStr = Configurations->GetStr("FacebookAppId", ModulesDatabase::Authentication);
	std::string FacebookAppSecretStr = Configurations->GetStr("FacebookAppSecret", ModulesDatabase::Authentication);
	if(Configurations->GetBool("FacebookOAuth", ModulesDatabase::Authentication, false) && !FacebookAppIdStr.empty() && !FacebookAppSecretStr.empty())
	{
		rapidxml::xml_node<> *NodeFacebookAppId = XmlDoc.allocate_node(rapidxml::node_element, "property", FacebookAppIdStr.c_str());
		NodeFacebookAppId->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-app-id"));
		NodeProperties->append_node(NodeFacebookAppId);

		rapidxml::xml_node<> *NodeFacebookAppSecret = XmlDoc.allocate_node(rapidxml::node_element, "property", FacebookAppSecretStr.c_str());
		NodeFacebookAppSecret->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-app-secret"));
		NodeProperties->append_node(NodeFacebookAppSecret);

		rapidxml::xml_node<> *NodeGoogleRedirect = XmlDoc.allocate_node(rapidxml::node_element, "property", RedirectStr.c_str());
		NodeGoogleRedirect->append_attribute(XmlDoc.allocate_attribute("name", "facebook-oauth2-redirect-endpoint"));
		NodeProperties->append_node(NodeGoogleRedirect);
	}

	//Write to file
	std::ofstream XmlFile("wt_config.xml", std::ios::trunc);
	XmlFile << XmlDoc;
	XmlFile.close();
}

WServer *WServer::instance()
{
	return dynamic_cast<WServer*>(Wt::WServer::instance());
}

WServer::~WServer()
{
	//Deallocate
	for(OAuthServiceMap::size_type i = 0; i < OAuthServices.size(); ++i)
	{
		delete OAuthServices[i];
	}

	delete Installer;
	delete AccessPaths;
	delete Pages;
	delete Styles;
	delete Languages;
	delete Configurations;
	delete Modules;
	delete SQLPool; //Also deletes SQLConnection(s)
}

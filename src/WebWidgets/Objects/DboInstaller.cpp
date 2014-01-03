#include "Objects/DboInstaller.h"
#include "Dbo/Module.h"
#include "Dbo/Configuration.h"
#include "Dbo/Language.h"
#include "Dbo/Page.h"
#include "DboDatabase/ModulesDatabase.h"

DboInstaller::DboInstaller(Wt::Dbo::SqlConnectionPool &SQLPool)
	: Installable(SQLPool)
{
	MapClasses();
}

DboInstaller::DboInstaller(Wt::Dbo::SqlConnection &SQLConnection)
	: Installable(SQLConnection)
{
	MapClasses();
}

void DboInstaller::MapClasses()
{
	DboSession.mapClass<Author>(Author::TableName());
	DboSession.mapClass<Module>(Module::TableName());
	DboSession.mapClass<Configuration>(Configuration::TableName());
	DboSession.mapClass<ConfigurationBool>(ConfigurationBool::TableName());
	DboSession.mapClass<ConfigurationEnum>(ConfigurationEnum::TableName());
	DboSession.mapClass<ConfigurationEnumValue>(ConfigurationEnumValue::TableName());
	DboSession.mapClass<ConfigurationDouble>(ConfigurationDouble::TableName());
	DboSession.mapClass<ConfigurationFloat>(ConfigurationFloat::TableName());
	DboSession.mapClass<ConfigurationInt>(ConfigurationInt::TableName());
	DboSession.mapClass<ConfigurationLongInt>(ConfigurationLongInt::TableName());
	DboSession.mapClass<ConfigurationString>(ConfigurationString::TableName());
	DboSession.mapClass<Language>(Language::TableName());
	DboSession.mapClass<LanguageSingle>(LanguageSingle::TableName());
	DboSession.mapClass<LanguagePlural>(LanguagePlural::TableName());
	DboSession.mapClass<Page>(Page::TableName());
	DboSession.mapClass<Template>(Template::TableName());
	DboSession.mapClass<Style>(Style::TableName());
	DboSession.mapClass<StyleTemplate>(StyleTemplate::TableName());
	DboSession.mapClass<StyleCssRule>(StyleCssRule::TableName());
	DboSession.mapClass<TemplateCssRule>(TemplateCssRule::TableName());
	DboSession.mapClass<AccessPath>(AccessPath::TableName());
}

void DboInstaller::CreateTables()
{
	Wt::Dbo::Transaction transaction(DboSession);
	DboSession.createTables();

	//INDEXes
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_accept\" ON \"") + Language::TableName() + "\" (\"LanguageAccept\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_url\" ON \"") + AccessPath::TableName() + "\" (\"HostName\", \"InternalPath\")");
	DboSession.execute(std::string("CREATE INDEX \"idx_installed\" ON \"") + Language::TableName() + "\" (\"Installed\")");
	
	transaction.commit();
}

void DboInstaller::InsertRows()
{
	Wt::Dbo::Transaction transaction(DboSession);

	//Authors
	Wt::Dbo::ptr<Author> SaifAuthor = DboSession.add(new Author(1));
	SaifAuthor.modify()->Name = "Saif Ur Rehman";
	SaifAuthor.modify()->Email = "saifi0102@gmail.com";

	Wt::Dbo::ptr<Author> EmwebAuthor = DboSession.add(new Author(2));
	EmwebAuthor.modify()->Name = "Emweb Bvba";
	EmwebAuthor.modify()->Email = "info@emweb.be";
	EmwebAuthor.modify()->Website = "http://www.emweb.be";

	//Modules
	Wt::Dbo::ptr<Module> WtModule = DboSession.add(new Module(ModulesDatabase::Wt));
	WtModule.modify()->Name = "Wt";
	WtModule.modify()->VersionSeries = 3;
	WtModule.modify()->VersionMajor = 2;
	WtModule.modify()->VersionMinor = 2;
	WtModule.modify()->AuthorPtr = EmwebAuthor;

	Wt::Dbo::ptr<Module> ServerModule = DboSession.add(new Module(ModulesDatabase::Server));
	ServerModule.modify()->Name = "Server";
	ServerModule.modify()->VersionSeries = 1;
	ServerModule.modify()->VersionMajor = 0;
	ServerModule.modify()->VersionMinor = 0;
	ServerModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> LoggingModule = DboSession.add(new Module(ModulesDatabase::Logging));
	LoggingModule.modify()->Name = "Logging";
	LoggingModule.modify()->VersionSeries = 1;
	LoggingModule.modify()->VersionMajor = 0;
	LoggingModule.modify()->VersionMinor = 0;
	LoggingModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> ConfigurationModule = DboSession.add(new Module(ModulesDatabase::Configurations));
	ConfigurationModule.modify()->Name = "Configuration";
	ConfigurationModule.modify()->VersionSeries = 1;
	ConfigurationModule.modify()->VersionMajor = 0;
	ConfigurationModule.modify()->VersionMinor = 0;
	ConfigurationModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> LocalizationModule = DboSession.add(new Module(ModulesDatabase::Localization));
	LocalizationModule.modify()->Name = "Localization";
	LocalizationModule.modify()->VersionSeries = 1;
	LocalizationModule.modify()->VersionMajor = 0;
	LocalizationModule.modify()->VersionMinor = 0;
	LocalizationModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> AuthenticationModule = DboSession.add(new Module(ModulesDatabase::Authentication));
	AuthenticationModule.modify()->Name = "Authentication";
	AuthenticationModule.modify()->VersionSeries = 1;
	AuthenticationModule.modify()->VersionMajor = 0;
	AuthenticationModule.modify()->VersionMinor = 0;
	AuthenticationModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> StylesModule = DboSession.add(new Module(ModulesDatabase::Styles));
	StylesModule.modify()->Name = "Styles";
	StylesModule.modify()->VersionSeries = 1;
	StylesModule.modify()->VersionMajor = 0;
	StylesModule.modify()->VersionMinor = 0;
	StylesModule.modify()->AuthorPtr = SaifAuthor;

	Wt::Dbo::ptr<Module> NavigationModule = DboSession.add(new Module(ModulesDatabase::Navigation));
	NavigationModule.modify()->Name = "Navigation";
	NavigationModule.modify()->VersionSeries = 1;
	NavigationModule.modify()->VersionMajor = 0;
	NavigationModule.modify()->VersionMinor = 0;
	NavigationModule.modify()->AuthorPtr = SaifAuthor;

	//Configurations
	//Hostname
	Wt::Dbo::ptr<Configuration> Hostname = DboSession.add(new Configuration("Hostname", ServerModule, ConfigurationKeys::String));
	Hostname.modify()->Title = "Hostname";
	Hostname.modify()->Details = "This is the host name or domain name used to visit your website. If you have multiple host names, use the one with most priority. Do not enter the 'www.' part.";
	Hostname.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> HostnameVal = DboSession.add(new ConfigurationString());
	HostnameVal.modify()->Value = "";
	HostnameVal.modify()->ExampleValue = "mywebsite.com";
	HostnameVal.modify()->MinLength = 1;
	Hostname.modify()->StringPtr = HostnameVal;

	//BaseURL
	Wt::Dbo::ptr<Configuration> BaseUrl = DboSession.add(new Configuration("BaseURL", ServerModule, ConfigurationKeys::String));
	BaseUrl.modify()->Title = "Base URL";
	BaseUrl.modify()->Details = "This is the base URL that the user can see when browsing. Can be left empty in which case the website will use relative paths.";
	BaseUrl.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> BaseUrlVal = DboSession.add(new ConfigurationString());
	BaseUrlVal.modify()->Value = "";
	BaseUrlVal.modify()->DefaultValue = "";
	BaseUrlVal.modify()->RecommendedValue = "";
	BaseUrlVal.modify()->ExampleValue = "http://www.mywebsite.com/wt";
	BaseUrl.modify()->StringPtr = BaseUrlVal;

	//DosPuzzle
	Wt::Dbo::ptr<Configuration> DosPuzzle = DboSession.add(new Configuration("DosPuzzle", ServerModule, ConfigurationKeys::Bool));
	DosPuzzle.modify()->Title = "Denial-of-Service protection puzzle";
	DosPuzzle.modify()->Details = "When enabled the server adds a puzzle to validate Ajax sessions in the first Ajax request. This is a simple measure which avoids Denial-of-Service attacks on Ajax sessions.";
	DosPuzzle.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> DosPuzzleVal = DboSession.add(new ConfigurationBool());
	DosPuzzleVal.modify()->Value = false;
	DosPuzzleVal.modify()->DefaultValue = false;
	DosPuzzle.modify()->BoolPtr = DosPuzzleVal;

	//JavascriptDebug
	Wt::Dbo::ptr<Configuration> JavascriptDebug = DboSession.add(new Configuration("JavascriptDebug", ServerModule, ConfigurationKeys::Enum));
	JavascriptDebug.modify()->Title = "Debug javascript mode";
	JavascriptDebug.modify()->Details = "Enable this only if you are a developer and to debug javascript.";
	JavascriptDebug.modify()->ExpertWarning = "Only change this if you are a developer and are trying to debug javascript of the website.";
	JavascriptDebug.modify()->RestartRequired = true;

	Wt::Dbo::ptr<ConfigurationEnum> JavascriptDebugVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOff = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOff.modify()->Value = 1;
	JavascriptDebugValOff.modify()->Title = "Off";
	JavascriptDebugValOff.modify()->Details = "JavaScript errors are caught and a simple error message is printed to indicate that something is terribly wrong.";
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn1 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn1.modify()->Value = 2;
	JavascriptDebugValOn1.modify()->Title = "On but caught";
	JavascriptDebugValOn1.modify()->Details = "JavaScript errors are caught but also a stack trace is printed, useful for diagnosing a problem in production.";
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn2 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn2.modify()->Value = 3;
	JavascriptDebugValOn2.modify()->Title = "On";
	JavascriptDebugValOn2.modify()->Details = "JavaScript errors are not caught but the browser built-in debug options are used.";

	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOff);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn1);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn2);

	JavascriptDebugVal.modify()->Value = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->DefaultValue = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->RecommendedValue = JavascriptDebugValOff->Value;

	JavascriptDebug.modify()->EnumPtr = JavascriptDebugVal;

	//MaxRequestSize
	Wt::Dbo::ptr<Configuration> MaxRequestSize = DboSession.add(new Configuration("MaxRequestSize", ServerModule, ConfigurationKeys::Int));
	MaxRequestSize.modify()->Title = "Max request size";
	MaxRequestSize.modify()->Details = "Maximum request size in kilobytes. This must be increased to allow uploading larger sized files.";
	MaxRequestSize.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationInt> MaxRequestSizeVal = DboSession.add(new ConfigurationInt());
	MaxRequestSizeVal.modify()->Value = 128;
	MaxRequestSizeVal.modify()->DefaultValue = 128;
	MaxRequestSizeVal.modify()->MinValue = 128;
	MaxRequestSize.modify()->IntPtr = MaxRequestSizeVal;

	//ReverseProxy
	Wt::Dbo::ptr<Configuration> ReverseProxy = DboSession.add(new Configuration("ReverseProxy", ServerModule, ConfigurationKeys::Bool));
	ReverseProxy.modify()->Title = "Behind reverse proxy";
	ReverseProxy.modify()->Details = "Enable this if the server is behind a reverse proxy. If you do not know what this means then disable it.";
	ReverseProxy.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> ReverseProxyVal = DboSession.add(new ConfigurationBool());
	ReverseProxyVal.modify()->Value = false;
	ReverseProxyVal.modify()->DefaultValue = false;
	ReverseProxy.modify()->BoolPtr = ReverseProxyVal;

	//SessionTimeout
	Wt::Dbo::ptr<Configuration> SessionTimeout = DboSession.add(new Configuration("SessionTimeout", ServerModule, ConfigurationKeys::Int));
	SessionTimeout.modify()->Title = "Session timeout delay";
	SessionTimeout.modify()->Details = "Session timeout delay in seconds. Users inactive for this amount of time will be considered offline.";
	SessionTimeout.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationInt> SessionTimeoutVal = DboSession.add(new ConfigurationInt());
	SessionTimeoutVal.modify()->Value = 600;
	SessionTimeoutVal.modify()->DefaultValue = 600;
	SessionTimeoutVal.modify()->RecommendedValue = 600;
	SessionTimeoutVal.modify()->MinValue = 30;
	SessionTimeout.modify()->IntPtr = SessionTimeoutVal;

	//SessionTracking
	Wt::Dbo::ptr<Configuration> SessionTracking = DboSession.add(new Configuration("SessionTracking", ServerModule, ConfigurationKeys::Enum));
	SessionTracking.modify()->Title = "Session tracking strategy";
	SessionTracking.modify()->RestartRequired = true;

	Wt::Dbo::ptr<ConfigurationEnum> SessionTrackingVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> SessionTrackingValCookies = DboSession.add(new ConfigurationEnumValue());
	SessionTrackingValCookies.modify()->Value = 1;
	SessionTrackingValCookies.modify()->Title = "Cookies";
	SessionTrackingValCookies.modify()->Details = "Cookies will be used if available, otherwise URL rewriting will be for tracking sessions.";
	Wt::Dbo::ptr<ConfigurationEnumValue> SessionTrackingValURL = DboSession.add(new ConfigurationEnumValue());
	SessionTrackingValURL.modify()->Value = 2;
	SessionTrackingValURL.modify()->Title = "Url";
	SessionTrackingValURL.modify()->Details = "Only URL rewriting will be used for tracking sessions.";

	SessionTrackingVal.modify()->EnumValueCollection.insert(SessionTrackingValCookies);
	SessionTrackingVal.modify()->EnumValueCollection.insert(SessionTrackingValURL);

	SessionTrackingVal.modify()->Value = SessionTrackingValCookies->Value;
	SessionTrackingVal.modify()->DefaultValue = SessionTrackingValCookies->Value;
	SessionTrackingVal.modify()->RecommendedValue = SessionTrackingValCookies->Value;

	SessionTracking.modify()->EnumPtr = SessionTrackingVal;

	//LogDebugLevel
	Wt::Dbo::ptr<Configuration> LogDebugLevel = DboSession.add(new Configuration("LogDebugLevel", LoggingModule, ConfigurationKeys::Bool));
	LogDebugLevel.modify()->Title = "Log debugging messages";
	LogDebugLevel.modify()->Details = "When enabled debugging events/messages will be included in the log file. These notices only gives information for developers and might not be meaningful to users.";
	LogDebugLevel.modify()->ExpertWarning = "Only change this if you are a developer and are trying to debug javascript of the website.";
	LogDebugLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogDebugLevelVal = DboSession.add(new ConfigurationBool());
	LogDebugLevelVal.modify()->Value = false;
	LogDebugLevelVal.modify()->DefaultValue = false;
	LogDebugLevelVal.modify()->RecommendedValue = false;
	LogDebugLevel.modify()->BoolPtr = LogDebugLevelVal;

	//LogDirectory
	Wt::Dbo::ptr<Configuration> LogDirectory = DboSession.add(new Configuration("LogDirectory", LoggingModule, ConfigurationKeys::String));
	LogDirectory.modify()->Title = "Logging file directory";
	LogDirectory.modify()->Details = "Location of where the logging file will be created. If this is left empty, no log file will be created but instead it will be stored in HTTP server logs. Disabling file logging is highly not recommended.";
	LogDirectory.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> LogDirectoryVal = DboSession.add(new ConfigurationString());
	LogDirectoryVal.modify()->Value = "./logs";
	LogDirectoryVal.modify()->DefaultValue = "./logs";
	LogDirectoryVal.modify()->ExampleValue = "/etc/logs/website";
	LogDirectory.modify()->StringPtr = LogDirectoryVal;

	//LogErrorLevel
	Wt::Dbo::ptr<Configuration> LogErrorLevel = DboSession.add(new Configuration("LogErrorLevel", LoggingModule, ConfigurationKeys::Bool));
	LogErrorLevel.modify()->Title = "Log error messages";
	LogErrorLevel.modify()->Details = "When enabled errors will be included in the logs.";
	LogErrorLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogErrorLevelVal = DboSession.add(new ConfigurationBool());
	LogErrorLevelVal.modify()->Value = true;
	LogErrorLevelVal.modify()->DefaultValue = true;
	LogErrorLevelVal.modify()->RecommendedValue = true;
	LogErrorLevel.modify()->BoolPtr = LogErrorLevelVal;

	//LogInfoLevel
	Wt::Dbo::ptr<Configuration> LogInfoLevel = DboSession.add(new Configuration("LogInfoLevel", LoggingModule, ConfigurationKeys::Bool));
	LogInfoLevel.modify()->Title = "Log informational notices";
	LogInfoLevel.modify()->Details = "When enabled informational notices will be included in the logs.";
	LogInfoLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogInfoLevelVal = DboSession.add(new ConfigurationBool());
	LogInfoLevelVal.modify()->Value = false;
	LogInfoLevelVal.modify()->DefaultValue = false;
	LogInfoLevel.modify()->BoolPtr = LogInfoLevelVal;

	//LogSecureLevel
	Wt::Dbo::ptr<Configuration> LogSecureLevel = DboSession.add(new Configuration("LogSecureLevel", LoggingModule, ConfigurationKeys::Bool));
	LogSecureLevel.modify()->Title = "Log security events";
	LogSecureLevel.modify()->Details = "When enabled security related messages will be included in the logs.";
	LogSecureLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogSecureLevelVal = DboSession.add(new ConfigurationBool());
	LogSecureLevelVal.modify()->Value = true;
	LogSecureLevelVal.modify()->DefaultValue = true;
	LogSecureLevelVal.modify()->RecommendedValue = true;
	LogSecureLevel.modify()->BoolPtr = LogSecureLevelVal;

	//LogWarnLevel
	Wt::Dbo::ptr<Configuration> LogWarnLevel = DboSession.add(new Configuration("LogWarnLevel", LoggingModule, ConfigurationKeys::Bool));
	LogWarnLevel.modify()->Title = "Log warnings";
	LogWarnLevel.modify()->Details = "When enabled warnings will be included in the log file. These warnings indicate that there was something could have caused an error and should be considered.\n\nRecommended Value: On";
	LogWarnLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogWarnLevelVal = DboSession.add(new ConfigurationBool());
	LogWarnLevelVal.modify()->Value = true;
	LogWarnLevelVal.modify()->DefaultValue = true;
	LogWarnLevelVal.modify()->RecommendedValue = true;
	LogWarnLevel.modify()->BoolPtr = LogWarnLevelVal;

	//DefaultAccessPath
	Wt::Dbo::ptr<Configuration> DefaultAccessPath = DboSession.add(new Configuration("DefaultAccessPath", LocalizationModule, ConfigurationKeys::LongInt));
	DefaultAccessPath.modify()->Title = "Default language access path";
	DefaultAccessPath.modify()->Details = "The access path that links to default language.";
	DefaultAccessPath.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationLongInt> DefaultAccessPathVal = DboSession.add(new ConfigurationLongInt());
	DefaultAccessPathVal.modify()->Value = 1;
	DefaultAccessPath.modify()->LongIntPtr = DefaultAccessPathVal;

	//HostUnspecificLanguage
	Wt::Dbo::ptr<Configuration> HostUnspecificLanguage = DboSession.add(new Configuration("HostUnspecificLanguage", LocalizationModule, ConfigurationKeys::Bool));
	HostUnspecificLanguage.modify()->Title = "Set language from host unspecific access path";
	HostUnspecificLanguage.modify()->Details = "If there are two conflicting access paths linking to different languages for example one with host name \"englishwebsite.com\" and no internal path and the other with internal path \"fr\" and no host name in that case if this is enabled then the access path without the host name would be used to set language of the user.";
	HostUnspecificLanguage.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationBool> HostUnspecificLanguageVal = DboSession.add(new ConfigurationBool());
	HostUnspecificLanguageVal.modify()->Value = false;
	HostUnspecificLanguageVal.modify()->DefaultValue = false;
	HostUnspecificLanguage.modify()->BoolPtr = HostUnspecificLanguageVal;

	//InternalPathMode
	Wt::Dbo::ptr<Configuration> InternalPathMode = DboSession.add(new Configuration("InternalPathMode", LocalizationModule, ConfigurationKeys::Enum));
	InternalPathMode.modify()->Title = "Presentation of language in internal path";
	InternalPathMode.modify()->RestartRequired = false;

	Wt::Dbo::ptr<ConfigurationEnum> InternalPathModeVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVShow = DboSession.add(new ConfigurationEnumValue());
	IPMVShow.modify()->Value = 1;
	IPMVShow.modify()->Title = "Always show language";
	IPMVShow.modify()->Details = "In this mode language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.";
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVShowHideDefault = DboSession.add(new ConfigurationEnumValue());
	IPMVShowHideDefault.modify()->Value = 2;
	IPMVShowHideDefault.modify()->Title = "Always show language but hide default language";
	IPMVShowHideDefault.modify()->Details = "In this mode all languages except for the default language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.";
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVNoRestrictionHideDefault = DboSession.add(new ConfigurationEnumValue());
	IPMVNoRestrictionHideDefault.modify()->Value = 3;
	IPMVNoRestrictionHideDefault.modify()->Title = "No restrictions but hide default language";
	IPMVNoRestrictionHideDefault.modify()->Details = "In this mode language will not be automatically removed or added except the default language will never be shown.";
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVNoRestriction = DboSession.add(new ConfigurationEnumValue());
	IPMVNoRestriction.modify()->Value = 4;
	IPMVNoRestriction.modify()->Title = "No restrictions";
	IPMVNoRestriction.modify()->Details = "In this mode language will not be automatically removed or added in any case.";

	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVShow);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVShowHideDefault);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVNoRestrictionHideDefault);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVNoRestriction);

	InternalPathModeVal.modify()->Value = IPMVNoRestrictionHideDefault->Value;
	InternalPathModeVal.modify()->DefaultValue = IPMVNoRestrictionHideDefault->Value;

	InternalPathMode.modify()->EnumPtr = InternalPathModeVal;

	//EnableTokens
	Wt::Dbo::ptr<Configuration> EnableTokens = DboSession.add(new Configuration("EnableTokens", AuthenticationModule, ConfigurationKeys::Bool));
	EnableTokens.modify()->Title = "Enable Remember-Me Tokens";
	EnableTokens.modify()->Details = "If enabled then remember me feature before login will be enabled. This means that if users agree to keep them self logged in, they will not have to login again even after end of session. Cookies will be set as tokens.";
	EnableTokens.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EnableTokensVal = DboSession.add(new ConfigurationBool());
	EnableTokensVal.modify()->Value = true;
	EnableTokensVal.modify()->DefaultValue = true;
	EnableTokensVal.modify()->RecommendedValue = true;
	EnableTokens.modify()->BoolPtr = EnableTokensVal;

	//LoginCookie
	Wt::Dbo::ptr<Configuration> LoginCookie = DboSession.add(new Configuration("LoginCookie", AuthenticationModule, ConfigurationKeys::String));
	LoginCookie.modify()->Title = "Remember-Me Cookie Name";
	LoginCookie.modify()->Details = "Name of the cookie that will be used to set the remember-me token. Do not change this if you do not know what this means.";
	LoginCookie.modify()->ExpertWarning = "Changing cookie name to an invalid name may prevent the remember-me function to work properly.";
	LoginCookie.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> LoginCookieVal = DboSession.add(new ConfigurationString());
	LoginCookieVal.modify()->Value = "auth";
	LoginCookieVal.modify()->DefaultValue = "auth";
	LoginCookieVal.modify()->ExampleValue = "login";
	LoginCookieVal.modify()->MinLength = 1;
	//LoginCookieVal.modify()->MaxLength = 3; //TODO
	LoginCookie.modify()->StringPtr = LoginCookieVal;

	//EmailVerification
	Wt::Dbo::ptr<Configuration> EmailVerification = DboSession.add(new Configuration("EmailVerification", AuthenticationModule, ConfigurationKeys::Bool));
	EmailVerification.modify()->Title = "Enable Email Verification";
	EmailVerification.modify()->Details = "If enabled then all new users must verify their email address after registration. Verification is done by sending an automatically generated email asking to verify the email.";
	EmailVerification.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EmailVerificationVal = DboSession.add(new ConfigurationBool());
	EmailVerificationVal.modify()->Value = false;
	EmailVerificationVal.modify()->DefaultValue = false;
	EmailVerification.modify()->BoolPtr = EmailVerificationVal;

	//FacebookOAuth
	Wt::Dbo::ptr<Configuration> FacebookOAuth = DboSession.add(new Configuration("FacebookOAuth", AuthenticationModule, ConfigurationKeys::Bool));
	FacebookOAuth.modify()->Title = "Enable Facebook Login";
	FacebookOAuth.modify()->Details = "If enabled then users would be able to login from their facebook account. Note that facebook connect configurations also have to be changed.";
	FacebookOAuth.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> FacebookOAuthVal = DboSession.add(new ConfigurationBool());
	FacebookOAuthVal.modify()->Value = false;
	FacebookOAuthVal.modify()->DefaultValue = false;
	FacebookOAuth.modify()->BoolPtr = FacebookOAuthVal;

	//FacebookAppID
	Wt::Dbo::ptr<Configuration> FacebookAppID = DboSession.add(new Configuration("FacebookAppId", AuthenticationModule, ConfigurationKeys::String));
	FacebookAppID.modify()->Title = "OAuth 2 Facebook App ID";
	FacebookAppID.modify()->Details = "Facebook app ID that you get by registering on OAuth 2.";
	FacebookAppID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppIDVal = DboSession.add(new ConfigurationString());
	FacebookAppIDVal.modify()->ExampleValue = "1234567890123456";
	FacebookAppIDVal.modify()->MinLength = 1;
	FacebookAppID.modify()->StringPtr = FacebookAppIDVal;

	//FacebookAppSecret
	Wt::Dbo::ptr<Configuration> FacebookAppSecret = DboSession.add(new Configuration("FacebookAppSecret", AuthenticationModule, ConfigurationKeys::String));
	FacebookAppSecret.modify()->Title = "OAuth 2 Facebook App Secret";
	FacebookAppSecret.modify()->Details = "Facebook app secret that you get by registering on OAuth 2.";
	FacebookAppSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppSecretVal = DboSession.add(new ConfigurationString());
	FacebookAppSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	FacebookAppSecretVal.modify()->MinLength = 1;
	FacebookAppSecret.modify()->StringPtr = FacebookAppSecretVal;

	//GoogleOAuth
	Wt::Dbo::ptr<Configuration> GoogleOAuth = DboSession.add(new Configuration("GoogleOAuth", AuthenticationModule, ConfigurationKeys::Bool));
	GoogleOAuth.modify()->Title = "Enable Google Login";
	GoogleOAuth.modify()->Details = "If enabled then users would be able to login from their google account. Note that google api configurations also have to be changed.";
	GoogleOAuth.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationBool> GoogleOAuthVal = DboSession.add(new ConfigurationBool());
	GoogleOAuthVal.modify()->Value = false;
	GoogleOAuthVal.modify()->DefaultValue = false;
	GoogleOAuth.modify()->BoolPtr = GoogleOAuthVal;

	//GoogleClientID
	Wt::Dbo::ptr<Configuration> GoogleClientID = DboSession.add(new Configuration("GoogleClientId", AuthenticationModule, ConfigurationKeys::String));
	GoogleClientID.modify()->Title = "OAuth 2 Google Client ID";
	GoogleClientID.modify()->Details = "Google client ID that you get by registering on OAuth 2.";
	GoogleClientID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientIDVal = DboSession.add(new ConfigurationString());
	GoogleClientIDVal.modify()->ExampleValue = "1234567890123456";
	GoogleClientIDVal.modify()->MinLength = 1;
	GoogleClientID.modify()->StringPtr = GoogleClientIDVal;

	//GoogleClientSecret
	Wt::Dbo::ptr<Configuration> GoogleClientSecret = DboSession.add(new Configuration("GoogleClientSecret", AuthenticationModule, ConfigurationKeys::String));
	GoogleClientSecret.modify()->Title = "OAuth 2 Google Client Secret";
	GoogleClientSecret.modify()->Details = "Google client secret that you get by registering on OAuth 2.";
	GoogleClientSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientSecretVal = DboSession.add(new ConfigurationString());
	GoogleClientSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	GoogleClientSecretVal.modify()->MinLength = 1;
	GoogleClientSecret.modify()->StringPtr = GoogleClientSecretVal;

	//HomePageAccessPathId
	Wt::Dbo::ptr<Configuration> HomePageAccessPathId = DboSession.add(new Configuration("HomePageAccessPathId", NavigationModule, ConfigurationKeys::LongInt));
	HomePageAccessPathId.modify()->Title = "Home page access path";
	HomePageAccessPathId.modify()->Details = "This is the page which will be treated as the default or landing page.";
	HomePageAccessPathId.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationLongInt> HomePageAccessPathIdVal = DboSession.add(new ConfigurationLongInt());
	HomePageAccessPathIdVal.modify()->Value = 3;
	HomePageAccessPathId.modify()->LongIntPtr = HomePageAccessPathIdVal;

	//MobileAccessPathId
	Wt::Dbo::ptr<Configuration> MobileAccessPathId = DboSession.add(new Configuration("MobileAccessPathId", NavigationModule, ConfigurationKeys::LongInt));
	MobileAccessPathId.modify()->Title = "Mobile version access path";
	MobileAccessPathId.modify()->Details = "The access path from where mobile version will be accessed.";
	MobileAccessPathId.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationLongInt> MobileAccessPathIdVal = DboSession.add(new ConfigurationLongInt());
	MobileAccessPathIdVal.modify()->Value = 2;
	MobileAccessPathId.modify()->LongIntPtr = MobileAccessPathIdVal;

	//ResourcesURL
	Wt::Dbo::ptr<Configuration> ResourcesURL = DboSession.add(new Configuration("ResourcesURL", StylesModule, ConfigurationKeys::String));
	ResourcesURL.modify()->Title = "Resources folder URL";
	ResourcesURL.modify()->Details = "Path to Wt's resources folder.";
	ResourcesURL.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> ResourcesURLVal = DboSession.add(new ConfigurationString());
	ResourcesURLVal.modify()->Value = "/resources";
	ResourcesURLVal.modify()->DefaultValue = "/resources";
	ResourcesURLVal.modify()->ExampleValue = "/resources";
	ResourcesURLVal.modify()->MinLength = 1;
	ResourcesURL.modify()->StringPtr = ResourcesURLVal;

	//StylesURL
	Wt::Dbo::ptr<Configuration> StylesURL = DboSession.add(new Configuration("StylesURL", StylesModule, ConfigurationKeys::String));
	StylesURL.modify()->Title = "Images folder URL";
	StylesURL.modify()->Details = "Path to the images folder that contain general and style images.";
	StylesURL.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> StylesURLVal = DboSession.add(new ConfigurationString());
	StylesURLVal.modify()->Value = "/styles";
	StylesURLVal.modify()->DefaultValue = "/styles";
	StylesURLVal.modify()->ExampleValue = "/styles";
	StylesURLVal.modify()->MinLength = 1;
	StylesURL.modify()->StringPtr = StylesURLVal;

	//Localization
	//English
	Wt::Dbo::ptr<Language> EnglishLanguagePtr = DboSession.add(new Language());
	EnglishLanguagePtr.modify()->Code = "en";
	EnglishLanguagePtr.modify()->Name = "English";
	EnglishLanguagePtr.modify()->NativeName = "English";
	EnglishLanguagePtr.modify()->LanguageAccept = "en*";
	EnglishLanguagePtr.modify()->PluralExpression = "n > 1 ? 1 : 0"; //Case 0 for Singular string and Case 1 for Plural string
	EnglishLanguagePtr.modify()->PluralCases = 2;

	//Uninstalled languages
	Wt::Dbo::ptr<Language> abLanguagePtr = DboSession.add(new Language("ab", "Abkhaz", "", "ab*", "", -1, false));
	Wt::Dbo::ptr<Language> aaLanguagePtr = DboSession.add(new Language("aa", "Afar", "", "aa*", "", -1, false));
	Wt::Dbo::ptr<Language> afLanguagePtr = DboSession.add(new Language("af", "Afrikaans", "", "af*", "", -1, false));
	Wt::Dbo::ptr<Language> akLanguagePtr = DboSession.add(new Language("ak", "Akan", "", "ak*", "", -1, false));
	Wt::Dbo::ptr<Language> sqLanguagePtr = DboSession.add(new Language("sq", "Albanian", "", "sq*", "", -1, false));
	Wt::Dbo::ptr<Language> amLanguagePtr = DboSession.add(new Language("am", "Amharic", "", "am*", "", -1, false));
	Wt::Dbo::ptr<Language> arLanguagePtr = DboSession.add(new Language("ar", "Arabic", "", "ar*", "", -1, false));
	Wt::Dbo::ptr<Language> anLanguagePtr = DboSession.add(new Language("an", "Aragonese", "", "an*", "", -1, false));
	Wt::Dbo::ptr<Language> hyLanguagePtr = DboSession.add(new Language("hy", "Armenian", "", "hy*", "", -1, false));
	Wt::Dbo::ptr<Language> asLanguagePtr = DboSession.add(new Language("as", "Assamese", "", "as*", "", -1, false));
	Wt::Dbo::ptr<Language> avLanguagePtr = DboSession.add(new Language("av", "Avaric", "", "av*", "", -1, false));
	Wt::Dbo::ptr<Language> aeLanguagePtr = DboSession.add(new Language("ae", "Avestan", "", "ae*", "", -1, false));
	Wt::Dbo::ptr<Language> ayLanguagePtr = DboSession.add(new Language("ay", "Aymara", "", "ay*", "", -1, false));
	Wt::Dbo::ptr<Language> azLanguagePtr = DboSession.add(new Language("az", "Azerbaijani", "", "az*", "", -1, false));
	Wt::Dbo::ptr<Language> bmLanguagePtr = DboSession.add(new Language("bm", "Bambara", "", "bm*", "", -1, false));
	Wt::Dbo::ptr<Language> baLanguagePtr = DboSession.add(new Language("ba", "Bashkir", "", "ba*", "", -1, false));
	Wt::Dbo::ptr<Language> euLanguagePtr = DboSession.add(new Language("eu", "Basque", "", "eu*", "", -1, false));
	Wt::Dbo::ptr<Language> beLanguagePtr = DboSession.add(new Language("be", "Belarusian", "", "be*", "", -1, false));
	Wt::Dbo::ptr<Language> bnLanguagePtr = DboSession.add(new Language("bn", "Bengali; Bangla", "", "bn*", "", -1, false));
	Wt::Dbo::ptr<Language> bhLanguagePtr = DboSession.add(new Language("bh", "Bihari", "", "bh*", "", -1, false));
	Wt::Dbo::ptr<Language> biLanguagePtr = DboSession.add(new Language("bi", "Bislama", "", "bi*", "", -1, false));
	Wt::Dbo::ptr<Language> bsLanguagePtr = DboSession.add(new Language("bs", "Bosnian", "", "bs*", "", -1, false));
	Wt::Dbo::ptr<Language> brLanguagePtr = DboSession.add(new Language("br", "Breton", "", "br*", "", -1, false));
	Wt::Dbo::ptr<Language> bgLanguagePtr = DboSession.add(new Language("bg", "Bulgarian", "", "bg*", "", -1, false));
	Wt::Dbo::ptr<Language> myLanguagePtr = DboSession.add(new Language("my", "Burmese", "", "my*", "", -1, false));
	Wt::Dbo::ptr<Language> caLanguagePtr = DboSession.add(new Language("ca", "Catalan; Valencian", "", "ca*", "", -1, false));
	Wt::Dbo::ptr<Language> chLanguagePtr = DboSession.add(new Language("ch", "Chamorro", "", "ch*", "", -1, false));
	Wt::Dbo::ptr<Language> ceLanguagePtr = DboSession.add(new Language("ce", "Chechen", "", "ce*", "", -1, false));
	Wt::Dbo::ptr<Language> nyLanguagePtr = DboSession.add(new Language("ny", "Chichewa; Chewa; Nyanja", "", "ny*", "", -1, false));
	Wt::Dbo::ptr<Language> zhLanguagePtr = DboSession.add(new Language("zh", "Chinese", "", "zh*", "", -1, false));
	Wt::Dbo::ptr<Language> cvLanguagePtr = DboSession.add(new Language("cv", "Chuvash", "", "cv*", "", -1, false));
	Wt::Dbo::ptr<Language> kwLanguagePtr = DboSession.add(new Language("kw", "Cornish", "", "kw*", "", -1, false));
	Wt::Dbo::ptr<Language> coLanguagePtr = DboSession.add(new Language("co", "Corsican", "", "co*", "", -1, false));
	Wt::Dbo::ptr<Language> crLanguagePtr = DboSession.add(new Language("cr", "Cree", "", "cr*", "", -1, false));
	Wt::Dbo::ptr<Language> hrLanguagePtr = DboSession.add(new Language("hr", "Croatian", "", "hr*", "", -1, false));
	Wt::Dbo::ptr<Language> csLanguagePtr = DboSession.add(new Language("cs", "Czech", "", "cs*", "", -1, false));
	Wt::Dbo::ptr<Language> daLanguagePtr = DboSession.add(new Language("da", "Danish", "", "da*", "", -1, false));
	Wt::Dbo::ptr<Language> dvLanguagePtr = DboSession.add(new Language("dv", "Divehi; Dhivehi; Maldivian;", "", "dv*", "", -1, false));
	Wt::Dbo::ptr<Language> nlLanguagePtr = DboSession.add(new Language("nl", "Dutch", "", "nl*", "", -1, false));
	Wt::Dbo::ptr<Language> dzLanguagePtr = DboSession.add(new Language("dz", "Dzongkha", "", "dz*", "", -1, false));
	Wt::Dbo::ptr<Language> eoLanguagePtr = DboSession.add(new Language("eo", "Esperanto", "", "eo*", "", -1, false));
	Wt::Dbo::ptr<Language> etLanguagePtr = DboSession.add(new Language("et", "Estonian", "", "et*", "", -1, false));
	Wt::Dbo::ptr<Language> eeLanguagePtr = DboSession.add(new Language("ee", "Ewe", "", "ee*", "", -1, false));
	Wt::Dbo::ptr<Language> foLanguagePtr = DboSession.add(new Language("fo", "Faroese", "", "fo*", "", -1, false));
	Wt::Dbo::ptr<Language> fjLanguagePtr = DboSession.add(new Language("fj", "Fijian", "", "fj*", "", -1, false));
	Wt::Dbo::ptr<Language> fiLanguagePtr = DboSession.add(new Language("fi", "Finnish", "", "fi*", "", -1, false));
	Wt::Dbo::ptr<Language> frLanguagePtr = DboSession.add(new Language("fr", "French", "", "fr*", "", -1, false));
	Wt::Dbo::ptr<Language> ffLanguagePtr = DboSession.add(new Language("ff", "Fula; Fulah; Pulaar; Pular", "", "ff*", "", -1, false));
	Wt::Dbo::ptr<Language> glLanguagePtr = DboSession.add(new Language("gl", "Galician", "", "gl*", "", -1, false));
	Wt::Dbo::ptr<Language> kaLanguagePtr = DboSession.add(new Language("ka", "Georgian", "", "ka*", "", -1, false));
	Wt::Dbo::ptr<Language> deLanguagePtr = DboSession.add(new Language("de", "German", "", "de*", "", -1, false));
	Wt::Dbo::ptr<Language> elLanguagePtr = DboSession.add(new Language("el", "Greek, Modern", "", "el*", "", -1, false));
	Wt::Dbo::ptr<Language> gnLanguagePtr = DboSession.add(new Language("gn", "Guaraní", "", "gn*", "", -1, false));
	Wt::Dbo::ptr<Language> guLanguagePtr = DboSession.add(new Language("gu", "Gujarati", "", "gu*", "", -1, false));
	Wt::Dbo::ptr<Language> htLanguagePtr = DboSession.add(new Language("ht", "Haitian; Haitian Creole", "", "ht*", "", -1, false));
	Wt::Dbo::ptr<Language> haLanguagePtr = DboSession.add(new Language("ha", "Hausa", "", "ha*", "", -1, false));
	Wt::Dbo::ptr<Language> heLanguagePtr = DboSession.add(new Language("he", "Hebrew (modern)", "", "he*", "", -1, false));
	Wt::Dbo::ptr<Language> hzLanguagePtr = DboSession.add(new Language("hz", "Herero", "", "hz*", "", -1, false));
	Wt::Dbo::ptr<Language> hiLanguagePtr = DboSession.add(new Language("hi", "Hindi", "", "hi*", "", -1, false));
	Wt::Dbo::ptr<Language> hoLanguagePtr = DboSession.add(new Language("ho", "Hiri Motu", "", "ho*", "", -1, false));
	Wt::Dbo::ptr<Language> huLanguagePtr = DboSession.add(new Language("hu", "Hungarian", "", "hu*", "", -1, false));
	Wt::Dbo::ptr<Language> iaLanguagePtr = DboSession.add(new Language("ia", "Interlingua", "", "ia*", "", -1, false));
	Wt::Dbo::ptr<Language> idLanguagePtr = DboSession.add(new Language("id", "Indonesian", "", "id*", "", -1, false));
	Wt::Dbo::ptr<Language> ieLanguagePtr = DboSession.add(new Language("ie", "Interlingue", "", "ie*", "", -1, false));
	Wt::Dbo::ptr<Language> gaLanguagePtr = DboSession.add(new Language("ga", "Irish", "", "ga*", "", -1, false));
	Wt::Dbo::ptr<Language> igLanguagePtr = DboSession.add(new Language("ig", "Igbo", "", "ig*", "", -1, false));
	Wt::Dbo::ptr<Language> ikLanguagePtr = DboSession.add(new Language("ik", "Inupiaq", "", "ik*", "", -1, false));
	Wt::Dbo::ptr<Language> ioLanguagePtr = DboSession.add(new Language("io", "Ido", "", "io*", "", -1, false));
	Wt::Dbo::ptr<Language> isLanguagePtr = DboSession.add(new Language("is", "Icelandic", "", "is*", "", -1, false));
	Wt::Dbo::ptr<Language> itLanguagePtr = DboSession.add(new Language("it", "Italian", "", "it*", "", -1, false));
	Wt::Dbo::ptr<Language> iuLanguagePtr = DboSession.add(new Language("iu", "Inuktitut", "", "iu*", "", -1, false));
	Wt::Dbo::ptr<Language> jaLanguagePtr = DboSession.add(new Language("ja", "Japanese", "", "ja*", "", -1, false));
	Wt::Dbo::ptr<Language> jvLanguagePtr = DboSession.add(new Language("jv", "Javanese", "", "jv*", "", -1, false));
	Wt::Dbo::ptr<Language> klLanguagePtr = DboSession.add(new Language("kl", "Kalaallisut, Greenlandic", "", "kl*", "", -1, false));
	Wt::Dbo::ptr<Language> knLanguagePtr = DboSession.add(new Language("kn", "Kannada", "", "kn*", "", -1, false));
	Wt::Dbo::ptr<Language> krLanguagePtr = DboSession.add(new Language("kr", "Kanuri", "", "kr*", "", -1, false));
	Wt::Dbo::ptr<Language> ksLanguagePtr = DboSession.add(new Language("ks", "Kashmiri", "", "ks*", "", -1, false));
	Wt::Dbo::ptr<Language> kkLanguagePtr = DboSession.add(new Language("kk", "Kazakh", "", "kk*", "", -1, false));
	Wt::Dbo::ptr<Language> kmLanguagePtr = DboSession.add(new Language("km", "Khmer", "", "km*", "", -1, false));
	Wt::Dbo::ptr<Language> kiLanguagePtr = DboSession.add(new Language("ki", "Kikuyu, Gikuyu", "", "ki*", "", -1, false));
	Wt::Dbo::ptr<Language> rwLanguagePtr = DboSession.add(new Language("rw", "Kinyarwanda", "", "rw*", "", -1, false));
	Wt::Dbo::ptr<Language> kyLanguagePtr = DboSession.add(new Language("ky", "Kyrgyz", "", "ky*", "", -1, false));
	Wt::Dbo::ptr<Language> kvLanguagePtr = DboSession.add(new Language("kv", "Komi", "", "kv*", "", -1, false));
	Wt::Dbo::ptr<Language> kgLanguagePtr = DboSession.add(new Language("kg", "Kongo", "", "kg*", "", -1, false));
	Wt::Dbo::ptr<Language> koLanguagePtr = DboSession.add(new Language("ko", "Korean", "", "ko*", "", -1, false));
	Wt::Dbo::ptr<Language> kuLanguagePtr = DboSession.add(new Language("ku", "Kurdish", "", "ku*", "", -1, false));
	Wt::Dbo::ptr<Language> kjLanguagePtr = DboSession.add(new Language("kj", "Kwanyama, Kuanyama", "", "kj*", "", -1, false));
	Wt::Dbo::ptr<Language> laLanguagePtr = DboSession.add(new Language("la", "Latin", "", "la*", "", -1, false));
	Wt::Dbo::ptr<Language> lbLanguagePtr = DboSession.add(new Language("lb", "Luxembourgish, Letzeburgesch", "", "lb*", "", -1, false));
	Wt::Dbo::ptr<Language> lgLanguagePtr = DboSession.add(new Language("lg", "Ganda", "", "lg*", "", -1, false));
	Wt::Dbo::ptr<Language> liLanguagePtr = DboSession.add(new Language("li", "Limburgish, Limburgan, Limburger", "", "li*", "", -1, false));
	Wt::Dbo::ptr<Language> lnLanguagePtr = DboSession.add(new Language("ln", "Lingala", "", "ln*", "", -1, false));
	Wt::Dbo::ptr<Language> loLanguagePtr = DboSession.add(new Language("lo", "Lao", "", "lo*", "", -1, false));
	Wt::Dbo::ptr<Language> ltLanguagePtr = DboSession.add(new Language("lt", "Lithuanian", "", "lt*", "", -1, false));
	Wt::Dbo::ptr<Language> luLanguagePtr = DboSession.add(new Language("lu", "Luba-Katanga", "", "lu*", "", -1, false));
	Wt::Dbo::ptr<Language> lvLanguagePtr = DboSession.add(new Language("lv", "Latvian", "", "lv*", "", -1, false));
	Wt::Dbo::ptr<Language> gvLanguagePtr = DboSession.add(new Language("gv", "Manx", "", "gv*", "", -1, false));
	Wt::Dbo::ptr<Language> mkLanguagePtr = DboSession.add(new Language("mk", "Macedonian", "", "mk*", "", -1, false));
	Wt::Dbo::ptr<Language> mgLanguagePtr = DboSession.add(new Language("mg", "Malagasy", "", "mg*", "", -1, false));
	Wt::Dbo::ptr<Language> msLanguagePtr = DboSession.add(new Language("ms", "Malay", "", "ms*", "", -1, false));
	Wt::Dbo::ptr<Language> mlLanguagePtr = DboSession.add(new Language("ml", "Malayalam", "", "ml*", "", -1, false));
	Wt::Dbo::ptr<Language> mtLanguagePtr = DboSession.add(new Language("mt", "Maltese", "", "mt*", "", -1, false));
	Wt::Dbo::ptr<Language> miLanguagePtr = DboSession.add(new Language("mi", "Maori", "", "mi*", "", -1, false));
	Wt::Dbo::ptr<Language> mrLanguagePtr = DboSession.add(new Language("mr", "Marathi", "", "mr*", "", -1, false));
	Wt::Dbo::ptr<Language> mhLanguagePtr = DboSession.add(new Language("mh", "Marshallese", "", "mh*", "", -1, false));
	Wt::Dbo::ptr<Language> mnLanguagePtr = DboSession.add(new Language("mn", "Mongolian", "", "mn*", "", -1, false));
	Wt::Dbo::ptr<Language> naLanguagePtr = DboSession.add(new Language("na", "Nauru", "", "na*", "", -1, false));
	Wt::Dbo::ptr<Language> nvLanguagePtr = DboSession.add(new Language("nv", "Navajo, Navaho", "", "nv*", "", -1, false));
	Wt::Dbo::ptr<Language> nbLanguagePtr = DboSession.add(new Language("nb", "Norwegian Bokmål", "", "nb*", "", -1, false));
	Wt::Dbo::ptr<Language> ndLanguagePtr = DboSession.add(new Language("nd", "North Ndebele", "", "nd*", "", -1, false));
	Wt::Dbo::ptr<Language> neLanguagePtr = DboSession.add(new Language("ne", "Nepali", "", "ne*", "", -1, false));
	Wt::Dbo::ptr<Language> ngLanguagePtr = DboSession.add(new Language("ng", "Ndonga", "", "ng*", "", -1, false));
	Wt::Dbo::ptr<Language> nnLanguagePtr = DboSession.add(new Language("nn", "Norwegian Nynorsk", "", "nn*", "", -1, false));
	Wt::Dbo::ptr<Language> noLanguagePtr = DboSession.add(new Language("no", "Norwegian", "", "no*", "", -1, false));
	Wt::Dbo::ptr<Language> iiLanguagePtr = DboSession.add(new Language("ii", "Nuosu", "", "ii*", "", -1, false));
	Wt::Dbo::ptr<Language> nrLanguagePtr = DboSession.add(new Language("nr", "South Ndebele", "", "nr*", "", -1, false));
	Wt::Dbo::ptr<Language> ocLanguagePtr = DboSession.add(new Language("oc", "Occitan", "", "oc*", "", -1, false));
	Wt::Dbo::ptr<Language> ojLanguagePtr = DboSession.add(new Language("oj", "Ojibwe, Ojibwa", "", "oj*", "", -1, false));
	Wt::Dbo::ptr<Language> cuLanguagePtr = DboSession.add(new Language("cu", "Old Church Slavonic, Church Slavic, Church Slavonic, Old Bulgarian, Old Slavonic", "", "cu*", "", -1, false));
	Wt::Dbo::ptr<Language> omLanguagePtr = DboSession.add(new Language("om", "Oromo", "", "om*", "", -1, false));
	Wt::Dbo::ptr<Language> orLanguagePtr = DboSession.add(new Language("or", "Oriya", "", "or*", "", -1, false));
	Wt::Dbo::ptr<Language> osLanguagePtr = DboSession.add(new Language("os", "Ossetian, Ossetic", "", "os*", "", -1, false));
	Wt::Dbo::ptr<Language> paLanguagePtr = DboSession.add(new Language("pa", "Panjabi, Punjabi", "", "pa*", "", -1, false));
	Wt::Dbo::ptr<Language> piLanguagePtr = DboSession.add(new Language("pi", "Pali", "", "pi*", "", -1, false));
	Wt::Dbo::ptr<Language> faLanguagePtr = DboSession.add(new Language("fa", "Persian (Farsi)", "", "fa*", "", -1, false));
	Wt::Dbo::ptr<Language> plLanguagePtr = DboSession.add(new Language("pl", "Polish", "", "pl*", "", -1, false));
	Wt::Dbo::ptr<Language> psLanguagePtr = DboSession.add(new Language("ps", "Pashto, Pushto", "", "ps*", "", -1, false));
	Wt::Dbo::ptr<Language> ptLanguagePtr = DboSession.add(new Language("pt", "Portuguese", "", "pt*", "", -1, false));
	Wt::Dbo::ptr<Language> quLanguagePtr = DboSession.add(new Language("qu", "Quechua", "", "qu*", "", -1, false));
	Wt::Dbo::ptr<Language> rmLanguagePtr = DboSession.add(new Language("rm", "Romansh", "", "rm*", "", -1, false));
	Wt::Dbo::ptr<Language> rnLanguagePtr = DboSession.add(new Language("rn", "Kirundi", "", "rn*", "", -1, false));
	Wt::Dbo::ptr<Language> roLanguagePtr = DboSession.add(new Language("ro", "Romanian, [])", "", "ro*", "", -1, false));
	Wt::Dbo::ptr<Language> ruLanguagePtr = DboSession.add(new Language("ru", "Russian", "", "ru*", "", -1, false));
	Wt::Dbo::ptr<Language> saLanguagePtr = DboSession.add(new Language("sa", "Sanskrit (Samskrta)", "", "sa*", "", -1, false));
	Wt::Dbo::ptr<Language> scLanguagePtr = DboSession.add(new Language("sc", "Sardinian", "", "sc*", "", -1, false));
	Wt::Dbo::ptr<Language> sdLanguagePtr = DboSession.add(new Language("sd", "Sindhi", "", "sd*", "", -1, false));
	Wt::Dbo::ptr<Language> seLanguagePtr = DboSession.add(new Language("se", "Northern Sami", "", "se*", "", -1, false));
	Wt::Dbo::ptr<Language> smLanguagePtr = DboSession.add(new Language("sm", "Samoan", "", "sm*", "", -1, false));
	Wt::Dbo::ptr<Language> sgLanguagePtr = DboSession.add(new Language("sg", "Sango", "", "sg*", "", -1, false));
	Wt::Dbo::ptr<Language> srLanguagePtr = DboSession.add(new Language("sr", "Serbian", "", "sr*", "", -1, false));
	Wt::Dbo::ptr<Language> gdLanguagePtr = DboSession.add(new Language("gd", "Scottish Gaelic; Gaelic", "", "gd*", "", -1, false));
	Wt::Dbo::ptr<Language> snLanguagePtr = DboSession.add(new Language("sn", "Shona", "", "sn*", "", -1, false));
	Wt::Dbo::ptr<Language> siLanguagePtr = DboSession.add(new Language("si", "Sinhala, Sinhalese", "", "si*", "", -1, false));
	Wt::Dbo::ptr<Language> skLanguagePtr = DboSession.add(new Language("sk", "Slovak", "", "sk*", "", -1, false));
	Wt::Dbo::ptr<Language> slLanguagePtr = DboSession.add(new Language("sl", "Slovene", "", "sl*", "", -1, false));
	Wt::Dbo::ptr<Language> soLanguagePtr = DboSession.add(new Language("so", "Somali", "", "so*", "", -1, false));
	Wt::Dbo::ptr<Language> stLanguagePtr = DboSession.add(new Language("st", "Southern Sotho", "", "st*", "", -1, false));
	Wt::Dbo::ptr<Language> esLanguagePtr = DboSession.add(new Language("es", "Spanish; Castilian", "", "es*", "", -1, false));
	Wt::Dbo::ptr<Language> suLanguagePtr = DboSession.add(new Language("su", "Sundanese", "", "su*", "", -1, false));
	Wt::Dbo::ptr<Language> swLanguagePtr = DboSession.add(new Language("sw", "Swahili", "", "sw*", "", -1, false));
	Wt::Dbo::ptr<Language> ssLanguagePtr = DboSession.add(new Language("ss", "Swati", "", "ss*", "", -1, false));
	Wt::Dbo::ptr<Language> svLanguagePtr = DboSession.add(new Language("sv", "Swedish", "", "sv*", "", -1, false));
	Wt::Dbo::ptr<Language> taLanguagePtr = DboSession.add(new Language("ta", "Tamil", "", "ta*", "", -1, false));
	Wt::Dbo::ptr<Language> teLanguagePtr = DboSession.add(new Language("te", "Telugu", "", "te*", "", -1, false));
	Wt::Dbo::ptr<Language> tgLanguagePtr = DboSession.add(new Language("tg", "Tajik", "", "tg*", "", -1, false));
	Wt::Dbo::ptr<Language> thLanguagePtr = DboSession.add(new Language("th", "Thai", "", "th*", "", -1, false));
	Wt::Dbo::ptr<Language> tiLanguagePtr = DboSession.add(new Language("ti", "Tigrinya", "", "ti*", "", -1, false));
	Wt::Dbo::ptr<Language> boLanguagePtr = DboSession.add(new Language("bo", "Tibetan Standard, Tibetan, Central", "", "bo*", "", -1, false));
	Wt::Dbo::ptr<Language> tkLanguagePtr = DboSession.add(new Language("tk", "Turkmen", "", "tk*", "", -1, false));
	Wt::Dbo::ptr<Language> tlLanguagePtr = DboSession.add(new Language("tl", "Tagalog", "", "tl*", "", -1, false));
	Wt::Dbo::ptr<Language> tnLanguagePtr = DboSession.add(new Language("tn", "Tswana", "", "tn*", "", -1, false));
	Wt::Dbo::ptr<Language> toLanguagePtr = DboSession.add(new Language("to", "Tonga (Tonga Islands)", "", "to*", "", -1, false));
	Wt::Dbo::ptr<Language> trLanguagePtr = DboSession.add(new Language("tr", "Turkish", "", "tr*", "", -1, false));
	Wt::Dbo::ptr<Language> tsLanguagePtr = DboSession.add(new Language("ts", "Tsonga", "", "ts*", "", -1, false));
	Wt::Dbo::ptr<Language> ttLanguagePtr = DboSession.add(new Language("tt", "Tatar", "", "tt*", "", -1, false));
	Wt::Dbo::ptr<Language> twLanguagePtr = DboSession.add(new Language("tw", "Twi", "", "tw*", "", -1, false));
	Wt::Dbo::ptr<Language> tyLanguagePtr = DboSession.add(new Language("ty", "Tahitian", "", "ty*", "", -1, false));
	Wt::Dbo::ptr<Language> ugLanguagePtr = DboSession.add(new Language("ug", "Uyghur, Uighur", "", "ug*", "", -1, false));
	Wt::Dbo::ptr<Language> ukLanguagePtr = DboSession.add(new Language("uk", "Ukrainian", "", "uk*", "", -1, false));
	Wt::Dbo::ptr<Language> urLanguagePtr = DboSession.add(new Language("ur", "Urdu", "", "ur*", "", -1, false));
	Wt::Dbo::ptr<Language> uzLanguagePtr = DboSession.add(new Language("uz", "Uzbek", "", "uz*", "", -1, false));
	Wt::Dbo::ptr<Language> veLanguagePtr = DboSession.add(new Language("ve", "Venda", "", "ve*", "", -1, false));
	Wt::Dbo::ptr<Language> viLanguagePtr = DboSession.add(new Language("vi", "Vietnamese", "", "vi*", "", -1, false));
	Wt::Dbo::ptr<Language> voLanguagePtr = DboSession.add(new Language("vo", "Volapük", "", "vo*", "", -1, false));
	Wt::Dbo::ptr<Language> waLanguagePtr = DboSession.add(new Language("wa", "Walloon", "", "wa*", "", -1, false));
	Wt::Dbo::ptr<Language> cyLanguagePtr = DboSession.add(new Language("cy", "Welsh", "", "cy*", "", -1, false));
	Wt::Dbo::ptr<Language> woLanguagePtr = DboSession.add(new Language("wo", "Wolof", "", "wo*", "", -1, false));
	Wt::Dbo::ptr<Language> fyLanguagePtr = DboSession.add(new Language("fy", "Western Frisian", "", "fy*", "", -1, false));
	Wt::Dbo::ptr<Language> xhLanguagePtr = DboSession.add(new Language("xh", "Xhosa", "", "xh*", "", -1, false));
	Wt::Dbo::ptr<Language> yiLanguagePtr = DboSession.add(new Language("yi", "Yiddish", "", "yi*", "", -1, false));
	Wt::Dbo::ptr<Language> yoLanguagePtr = DboSession.add(new Language("yo", "Yoruba", "", "yo*", "", -1, false));
	Wt::Dbo::ptr<Language> zaLanguagePtr = DboSession.add(new Language("za", "Zhuang, Chuang", "", "za*", "", -1, false));
	Wt::Dbo::ptr<Language> zuLanguagePtr = DboSession.add(new Language("zu", "Zulu", "", "zu*", "", -1, false));
	
	//Basic formatting
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("DecimalPointCharacter", ".", LocalizationModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("NumberThousandSeparator", ",", LocalizationModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("DateFormat", "MMMM dd, yyyy", LocalizationModule)));

	//Wt built in internationalization strings
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WAbstractItemView.PageIOfN", "<b>{1}</b> of <b>{2}</b>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WAbstractItemView.PageBar.First", "&#x00ab; First", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WAbstractItemView.PageBar.Previous", "&#x2039; Previous", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WAbstractItemView.PageBar.Next", "Next &#x203a;", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WAbstractItemView.PageBar.Last", "Last &#x00bb;", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WCalendar.PrevMonth", "&#x00ab;", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WCalendar.NextMonth", "&#x00bb;", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Monday", "Monday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Tuesday", "Tuesday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Wednesday", "Wednesday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Thursday", "Thursday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Friday", "Friday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Saturday", "Saturday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.Sunday", "Sunday", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Mon", "Mon", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Tue", "Tue", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Wed", "Wed", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Thu", "Thu", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Fri", "Fri", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Sat", "Sat", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Sun", "Sun", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.January", "January", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.February", "February", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.March", "March", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.April", "April", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.May", "May", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.June", "June", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.July", "July", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.August", "August", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.September", "September", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.October", "October", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.November", "November", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.December", "December", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Jan", "Jan", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Feb", "Feb", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Mar", "Mar", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Apr", "Apr", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.May", "May", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Jun", "Jun", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Jul", "Jul", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Aug", "Aug", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Sep", "Sep", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Oct", "Oct", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Nov", "Nov", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDate.3.Dec", "Dec", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDatePicker.Close", "Close", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.LessThanASecond", "less than a second", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.second", "one second", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.seconds", "{1} seconds", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.minute", "one minute", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.minutes", "{1} minutes", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.hour", "one hour", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.hours", "{1} hours", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.day", "one day", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.days", "{1} days", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.week", "one week", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.weeks", "{1} weeks", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.month", "one month", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.months", "{1} months", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.year", "one year", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.years", "{1} years", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateTime.null", "Null", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateValidator.DateTooEarly", "The date must be after {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateValidator.DateTooLate", "The date must be before {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateValidator.WrongDateRange", "The date must be between {1} and {2}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDateValidator.WrongFormat", "Must be a date in the format '{1}'", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDefaultLoadingIndicator.Loading", "Loading...", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDoubleValidator.NotANumber", "Must be a number", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDoubleValidator.TooSmall", "The number must be larger than {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDoubleValidator.BadRange", "The number must be in the range {1} to {2}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WDoubleValidator.TooLarge", "The number must be smaller than {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WInPlaceEdit.Save", "Save", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WInPlaceEdit.Cancel", "Cancel", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WIntValidator.NotAnInteger", "Must be an integer number", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WIntValidator.TooSmall", "The number must be larger than {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WIntValidator.BadRange", "The number must be in the range {1} to {2}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WIntValidator.TooLarge", "The number must be smaller than {1}", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WLengthValidator.TooShort", "The input must be at least {1} characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WLengthValidator.BadRange", "The input must have a length between {1} and {2} characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WLengthValidator.TooLong", "The input must be no more than {1} characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.play", "Play", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.pause", "Pause", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.stop", "Stop", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.mute", "Mute", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.unmute", "Unmute", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.volume-max", "Maximum volume", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.repeat", "Repeat", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.repeat-off", "Repeat off", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.video-play-icon", "Play", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.full-screen", "Full screen", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMediaPlayer.restore-screen", "Restore screen", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Abort", "Abort", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Cancel", "Cancel", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Ignore", "Ignore", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.No", "No", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.NoToAll", "No to All", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Ok", "Ok", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Retry", "Retry", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.Yes", "Yes", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WMessageBox.YesToAll", "Yes to All", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WOverlayLoadingIndicator.Loading", "Loading...", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WRegExpValidator.Invalid", "Invalid input", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.WValidator.Invalid", "This field cannot be empty", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.true", "true", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.false", "false", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.error-invalid-token", "The operation could not be completed: invalid token.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.error-token-expired", "The operation could not be completed: the token has expired.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.mail-sent", "An email has been sent. Follow the instructions to set a new password.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.info-email-confirmed", "Your email address is now confirmed.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-error", "Check failed", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-same", "The same as the old one", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-similar", "Based on the old one", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-short", "Too short", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-long", "Too long", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-simpleshort", "Not enough different characters or classes for this length", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-simple", "Not enough different characters or classes", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-personal", "Based on personal information", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-word", "Based on a dictionary word and not a passphrase", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwdqc.reason-seq", "Based on a common sequence of characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.GoogleService.badresponse", "<p>Sorry, could not login using your Google account.</p><small><p>Google's identification server generated an unexpected response.</p></small>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.FacebookService.badresponse", "<p>Sorry, could not login using your Facebook account.</p><small><p>Facebook's identification server generated an unexpected response.</p></small>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.login-form-title", "Login", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.user-name", "User name", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.user-name-info", "Enter your user name", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.user-name-invalid", "Invalid", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.email", "Email address", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.email-info", "Enter your email address", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.optional-email-info", "Enter your email address (optional)", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password", "Password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password-info", "Enter your password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.remember-me", "Remember me", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.remember-me-info.days", "Keeps login for {1} days", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.remember-me-info.weeks", "Keeps login for {1} weeks", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lost-password", "Lost password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.login", "Login", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.logout", "Logout", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.throttle-retry", "Retry in {1}s", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.registration-form-title", "Registration", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.registration-form-info", "Please complete the following information to register.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password-or-oauth-registration", "<h3>Register using a user name and password:</h3>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.oauth-registration", "<h3>Register with an identity provider:</h3>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.or-oauth-registration", "<h3>Or, register with an identity provider:</h3>", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.valid", "Valid", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.choose-password", "Password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.choose-password-info", "Choose a password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.repeat-password", "Repeat password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.repeat-password-info", "Re-enter your password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.email-invalid", "Invalid email address", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.email-exists", "Email already registered", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.oauth", "Your account", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.oauth-info", "Choose one of your accounts", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.user-name-exists", "User already exists", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.confirm-is-you", "User exists, is this you?", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.user-name-tooshort", "Min. size {1} characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password-tooshort", "Min. size {1} characters", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.passwords-dont-match", "Passwords don't match", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.registration", "Registration", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.register", "Register", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.error-user-invalid", "Registration failed.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.enter-password", "Enter your password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password-prompt-form-title", "Enter your password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.password-invalid", "Invalid password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.updatepassword", "Update password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.update-password-form-title", "Update password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.update-password-form-info", "Enter a new password below", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.old-password", "Current password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.old-password-info", "Enter your current password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.new-password", "New password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.repeat-new-password", "Repeat new password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.repeat-new-password-info", "Re-enter your new password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lostpassword", "Password forgotten? No sweat!", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lost-password-form-title", "Recover your password", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lost-password-form-info", "Please enter below the email address which you used during registration. A mail will be sent with instructions to enter a new password.", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.send", "Send", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.error", "Error", WtModule)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.notice", "Notice", WtModule)));

	//Emails
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.confirmmail.subject", "User account activation Link", WtModule, Wt::Dbo::ptr<Language>(), true)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.confirmmail.body", "Hello {1},\r\n\r\nThank you for joining!\r\n\r\nTo complete your registration, please finally confirm your account by clicking on the following link or copying the URL into your browser.\r\n \r\nPlease click here to confirm your registration or copy and paste the following URL into your browser: (Note: be sure to copy the entire URL, including any part of it which goes onto a second line.)\r\n\r\n{3}", WtModule, Wt::Dbo::ptr<Language>(), true)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.confirmmail.htmlbody", "<h3>Hello {1},</h3>\r\n\r\n<p>Thank you for joining!</p>\r\n\r\n<p>To complete your registration, please finally confirm your account by clicking on the following link or copying the URL into your browser.</p>\r\n \r\n<p>Please <a href=\"{3}\">click here to confirm</a> your registration or copy and paste the following URL into your browser: <i>(Note: be sure to copy the entire URL, including any part of it which goes onto a second line.)</i></p>\r\n\r\n<b>{3}</b>", WtModule, Wt::Dbo::ptr<Language>(), true)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lostpasswordmail.subject", "Lost password instructions", WtModule, Wt::Dbo::ptr<Language>(), true)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lostpasswordmail.body", "Hello {1},\r\n\r\nThis mail has been sent to you, because someone (presumably you?) indicated that he wishes to choose a new password, because the current password escapes his mind.\r\n\r\nIf you requested this, then choose a new password by clicking on the following link or copying the URL into your browser. If you didn't request this, you can safely ignore and discard this email.\r\n \r\nPlease copy and paste the following URL into your browser: (Note: be sure to copy the entire URL, including any part of it which goes onto a second line.)\r\n \r\n{3}", WtModule, Wt::Dbo::ptr<Language>(), true)));
	EnglishLanguagePtr.modify()->LanguageSingleCollection.insert(DboSession.add(new LanguageSingle("Wt.Auth.lostpasswordmail.htmlbody", "<h3>Hello {1},</h3>\r\n\r\n<p>This mail has been sent to you, because someone (presumably you?)  indicated that he wishes to choose a new password, because the current password escapes his mind.</p>\r\n\r\n<p>If you requested this, then choose a new password by clicking on the following link or copying the URL into your browser. If you didn't request this, you can safely ignore and discard this email.</p>\r\n \r\n<p>Please <a href=\"{3}\">click here to choose a new password</a> or copy and paste the following URL into your browser: <i>(Note: be sure to copy the entire URL, including any part of it which goes onto a second line.)</i></p>\r\n\r\n<b>{3}</b>", WtModule, Wt::Dbo::ptr<Language>(), true)));

	//Pages
	Wt::Dbo::ptr<Page> LandingHomePage = DboSession.add(new Page(1, NavigationModule));
	LandingHomePage.modify()->Title = "Home";
	LandingHomePage.modify()->DefaultInternalPath = "home";

	Wt::Dbo::ptr<Page> SitemapPage = DboSession.add(new Page(2, NavigationModule));
	SitemapPage.modify()->Title = "Site map";
	SitemapPage.modify()->DefaultInternalPath = "sitemap";

	//Access Paths
	//English language
	Wt::Dbo::ptr<AccessPath> DefaultLanguageAccessPath = DboSession.add(new AccessPath());
	DefaultLanguageAccessPath.modify()->HostName = "";
	DefaultLanguageAccessPath.modify()->InternalPath = "en";
	DefaultLanguageAccessPath.modify()->LanguagePtr = EnglishLanguagePtr;

	//Mobile version
	Wt::Dbo::ptr<AccessPath> MobileVersionAccessPath = DboSession.add(new AccessPath());
	MobileVersionAccessPath.modify()->HostName = "";
	MobileVersionAccessPath.modify()->InternalPath = "mobile";

	//Home page
	Wt::Dbo::ptr<AccessPath> HomePageAccessPath = DboSession.add(new AccessPath());
	HomePageAccessPath.modify()->HostName = "";
	HomePageAccessPath.modify()->InternalPath = "home";
	HomePageAccessPath.modify()->PagePtr = LandingHomePage;

	//Sitemap page
	Wt::Dbo::ptr<AccessPath> SitemapPageAccessPath = DboSession.add(new AccessPath());
	SitemapPageAccessPath.modify()->HostName = "";
	SitemapPageAccessPath.modify()->InternalPath = "sitemap";
	SitemapPageAccessPath.modify()->PagePtr = SitemapPage;

	transaction.commit();

	Wt::Dbo::Transaction transaction2(DboSession);
	DefaultAccessPathVal.modify()->Value = DefaultLanguageAccessPath.id();
	HomePageAccessPathIdVal.modify()->Value = HomePageAccessPath.id();
	MobileAccessPathIdVal.modify()->Value = MobileVersionAccessPath.id();
	transaction2.commit();
}

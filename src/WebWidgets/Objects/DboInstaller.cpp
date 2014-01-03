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
	EnglishLanguagePtr.modify()->LanguageAccept = "en*";
	EnglishLanguagePtr.modify()->PluralExpression = "n > 1 ? 1 : 0"; //Case 0 for Singular string and Case 1 for Plural string
	EnglishLanguagePtr.modify()->PluralCases = 2;

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

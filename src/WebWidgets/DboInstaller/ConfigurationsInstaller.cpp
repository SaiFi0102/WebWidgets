#include "DboInstaller/DboInstaller.h"

void DboInstaller::InsertConfigurations()
{
	//Hostname
	Wt::Dbo::ptr<Configuration> Hostname = DboSession.add(new Configuration("Hostname", O.ServerModule, Configuration::String));
	Hostname.modify()->Title = "Hostname";
	Hostname.modify()->Details = "This is the host name or domain name used to visit your website. If you have multiple host names, use the one with most priority. Do not enter the 'www.' part.";
	Hostname.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> HostnameVal = DboSession.add(new ConfigurationString());
	HostnameVal.modify()->Value = "";
	HostnameVal.modify()->ExampleValue = "mywebsite.com";
	HostnameVal.modify()->MinLength = 1;
	Hostname.modify()->StringPtr = HostnameVal;

	//BaseURL
	Wt::Dbo::ptr<Configuration> BaseUrl = DboSession.add(new Configuration("BaseURL", O.ServerModule, Configuration::String));
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
	Wt::Dbo::ptr<Configuration> DosPuzzle = DboSession.add(new Configuration("DosPuzzle", O.ServerModule, Configuration::Bool));
	DosPuzzle.modify()->Title = "Denial-of-Service protection puzzle";
	DosPuzzle.modify()->Details = "When enabled the server adds a puzzle to validate Ajax sessions in the first Ajax request. This is a simple measure which avoids Denial-of-Service attacks on Ajax sessions.";
	DosPuzzle.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> DosPuzzleVal = DboSession.add(new ConfigurationBool());
	DosPuzzleVal.modify()->Value = false;
	DosPuzzleVal.modify()->DefaultValue = false;
	DosPuzzle.modify()->BoolPtr = DosPuzzleVal;

	//JavascriptDebug
	Wt::Dbo::ptr<Configuration> JavascriptDebug = DboSession.add(new Configuration("JavascriptDebug", O.ServerModule, Configuration::Enum));
	JavascriptDebug.modify()->Title = "JavaScript debug mode";
	JavascriptDebug.modify()->Details = "Enable this only if you are a developer and debugging JavaScript.";
	JavascriptDebug.modify()->ExpertWarning = "Only change this if you are a developer and debugging javascript on the website.";
	JavascriptDebug.modify()->RestartRequired = true;

	Wt::Dbo::ptr<ConfigurationEnum> JavascriptDebugVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOff = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOff.modify()->Value = 1;
	JavascriptDebugValOff.modify()->Title = "Off";
	JavascriptDebugValOff.modify()->Details = "JavaScript errors are caught, logged on server-side and a simple error message is printed to indicate that something is terribly wrong.";
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn1 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn1.modify()->Value = 2;
	JavascriptDebugValOn1.modify()->Title = "Log and rethrow";
	JavascriptDebugValOn1.modify()->Details = "JavaScript errors are caught, logged on server-side and rethrown. This is useful for diagnosing a problem and debugging.";
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn2 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn2.modify()->Value = 3;
	JavascriptDebugValOn2.modify()->Title = "Naked";
	JavascriptDebugValOn2.modify()->Details = "JavaScript errors are not caught nor logged and the browser handles the error. This is useful if logging the error server-side is causing problems.";

	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOff);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn1);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn2);

	JavascriptDebugVal.modify()->Value = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->DefaultValue = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->RecommendedValue = JavascriptDebugValOff->Value;

	JavascriptDebug.modify()->EnumPtr = JavascriptDebugVal;

	//MaxRequestSize
	Wt::Dbo::ptr<Configuration> MaxRequestSize = DboSession.add(new Configuration("MaxRequestSize", O.ServerModule, Configuration::Int));
	MaxRequestSize.modify()->Title = "Max request size";
	MaxRequestSize.modify()->Details = "Maximum request size in kilobytes. This must be increased to allow uploading larger sized files.";
	MaxRequestSize.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationInt> MaxRequestSizeVal = DboSession.add(new ConfigurationInt());
	MaxRequestSizeVal.modify()->Value = 128;
	MaxRequestSizeVal.modify()->DefaultValue = 128;
	MaxRequestSizeVal.modify()->MinValue = 128;
	MaxRequestSize.modify()->IntPtr = MaxRequestSizeVal;

	//ReverseProxy
	Wt::Dbo::ptr<Configuration> ReverseProxy = DboSession.add(new Configuration("ReverseProxy", O.ServerModule, Configuration::Bool));
	ReverseProxy.modify()->Title = "Behind reverse proxy";
	ReverseProxy.modify()->Details = "Enable this if the server is behind a reverse proxy.";
	ReverseProxy.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> ReverseProxyVal = DboSession.add(new ConfigurationBool());
	ReverseProxyVal.modify()->Value = false;
	ReverseProxyVal.modify()->DefaultValue = false;
	ReverseProxy.modify()->BoolPtr = ReverseProxyVal;

	//SessionTimeout
	Wt::Dbo::ptr<Configuration> SessionTimeout = DboSession.add(new Configuration("SessionTimeout", O.ServerModule, Configuration::Int));
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
	Wt::Dbo::ptr<Configuration> SessionTracking = DboSession.add(new Configuration("SessionTracking", O.ServerModule, Configuration::Enum));
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
	Wt::Dbo::ptr<Configuration> LogDebugLevel = DboSession.add(new Configuration("LogDebugLevel", O.LoggingModule, Configuration::Bool));
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
	Wt::Dbo::ptr<Configuration> LogDirectory = DboSession.add(new Configuration("LogDirectory", O.LoggingModule, Configuration::String));
	LogDirectory.modify()->Title = "Logging file directory";
	LogDirectory.modify()->Details = "Location of where the logging file will be created. If this is left empty, no log file will be created but instead it will be stored in HTTP server logs. Disabling file logging is highly not recommended.";
	LogDirectory.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> LogDirectoryVal = DboSession.add(new ConfigurationString());
	LogDirectoryVal.modify()->Value = "./logs";
	LogDirectoryVal.modify()->DefaultValue = "./logs";
	LogDirectoryVal.modify()->ExampleValue = "/etc/logs/website";
	LogDirectory.modify()->StringPtr = LogDirectoryVal;

	//LogErrorLevel
	Wt::Dbo::ptr<Configuration> LogErrorLevel = DboSession.add(new Configuration("LogErrorLevel", O.LoggingModule, Configuration::Bool));
	LogErrorLevel.modify()->Title = "Log error messages";
	LogErrorLevel.modify()->Details = "When enabled errors will be included in the logs.";
	LogErrorLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogErrorLevelVal = DboSession.add(new ConfigurationBool());
	LogErrorLevelVal.modify()->Value = true;
	LogErrorLevelVal.modify()->DefaultValue = true;
	LogErrorLevelVal.modify()->RecommendedValue = true;
	LogErrorLevel.modify()->BoolPtr = LogErrorLevelVal;

	//LogInfoLevel
	Wt::Dbo::ptr<Configuration> LogInfoLevel = DboSession.add(new Configuration("LogInfoLevel", O.LoggingModule, Configuration::Bool));
	LogInfoLevel.modify()->Title = "Log informational notices";
	LogInfoLevel.modify()->Details = "When enabled informational notices will be included in the logs.";
	LogInfoLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogInfoLevelVal = DboSession.add(new ConfigurationBool());
	LogInfoLevelVal.modify()->Value = false;
	LogInfoLevelVal.modify()->DefaultValue = false;
	LogInfoLevel.modify()->BoolPtr = LogInfoLevelVal;

	//LogSecureLevel
	Wt::Dbo::ptr<Configuration> LogSecureLevel = DboSession.add(new Configuration("LogSecureLevel", O.LoggingModule, Configuration::Bool));
	LogSecureLevel.modify()->Title = "Log security events";
	LogSecureLevel.modify()->Details = "When enabled security related messages will be included in the logs.";
	LogSecureLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogSecureLevelVal = DboSession.add(new ConfigurationBool());
	LogSecureLevelVal.modify()->Value = true;
	LogSecureLevelVal.modify()->DefaultValue = true;
	LogSecureLevelVal.modify()->RecommendedValue = true;
	LogSecureLevel.modify()->BoolPtr = LogSecureLevelVal;

	//LogWarnLevel
	Wt::Dbo::ptr<Configuration> LogWarnLevel = DboSession.add(new Configuration("LogWarnLevel", O.LoggingModule, Configuration::Bool));
	LogWarnLevel.modify()->Title = "Log warnings";
	LogWarnLevel.modify()->Details = "When enabled warnings will be included in the log file. These warnings indicate that there was something could have caused an error and should be considered.\n\nRecommended Value: On";
	LogWarnLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogWarnLevelVal = DboSession.add(new ConfigurationBool());
	LogWarnLevelVal.modify()->Value = true;
	LogWarnLevelVal.modify()->DefaultValue = true;
	LogWarnLevelVal.modify()->RecommendedValue = true;
	LogWarnLevel.modify()->BoolPtr = LogWarnLevelVal;

	//DefaultAccessPath
	Wt::Dbo::ptr<Configuration> DefaultAccessPath = DboSession.add(new Configuration("DefaultAccessPath", O.LocalizationModule, Configuration::LongInt));
	DefaultAccessPath.modify()->Title = "Default language access path";
	DefaultAccessPath.modify()->Details = "The access path that links to default language.";
	DefaultAccessPath.modify()->RestartRequired = false;
	O.DefaultAccessPathVal = DboSession.add(new ConfigurationLongInt());
	O.DefaultAccessPathVal.modify()->Value = 1;
	DefaultAccessPath.modify()->LongIntPtr = O.DefaultAccessPathVal;

	//HostUnspecificLanguage
	Wt::Dbo::ptr<Configuration> HostUnspecificLanguage = DboSession.add(new Configuration("HostUnspecificLanguage", O.LocalizationModule, Configuration::Bool));
	HostUnspecificLanguage.modify()->Title = "Set language from host unspecific access path";
	HostUnspecificLanguage.modify()->Details = "If there are two conflicting access paths linking to different languages for example one with host name \"englishwebsite.com\" and no internal path and the other with internal path \"fr\" and no host name in that case if this is enabled then the access path without the host name would be used to set language of the user.";
	HostUnspecificLanguage.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationBool> HostUnspecificLanguageVal = DboSession.add(new ConfigurationBool());
	HostUnspecificLanguageVal.modify()->Value = false;
	HostUnspecificLanguageVal.modify()->DefaultValue = false;
	HostUnspecificLanguage.modify()->BoolPtr = HostUnspecificLanguageVal;

	//InternalPathMode
	Wt::Dbo::ptr<Configuration> InternalPathMode = DboSession.add(new Configuration("InternalPathMode", O.LocalizationModule, Configuration::Enum));
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

	InternalPathModeVal.modify()->Value = IPMVShowHideDefault->Value;
	InternalPathModeVal.modify()->DefaultValue = IPMVShowHideDefault->Value;

	InternalPathMode.modify()->EnumPtr = InternalPathModeVal;

	//EnableTokens
	Wt::Dbo::ptr<Configuration> EnableTokens = DboSession.add(new Configuration("EnableTokens", O.AuthenticationModule, Configuration::Bool));
	EnableTokens.modify()->Title = "Enable Remember-Me Tokens";
	EnableTokens.modify()->Details = "If enabled then remember me feature before login will be enabled. This means that if users agree to keep them self logged in, they will not have to login again even after end of session. Cookies will be set as tokens.";
	EnableTokens.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EnableTokensVal = DboSession.add(new ConfigurationBool());
	EnableTokensVal.modify()->Value = true;
	EnableTokensVal.modify()->DefaultValue = true;
	EnableTokensVal.modify()->RecommendedValue = true;
	EnableTokens.modify()->BoolPtr = EnableTokensVal;

	//LoginCookie
	Wt::Dbo::ptr<Configuration> LoginCookie = DboSession.add(new Configuration("LoginCookie", O.AuthenticationModule, Configuration::String));
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
	Wt::Dbo::ptr<Configuration> EmailVerification = DboSession.add(new Configuration("EmailVerification", O.AuthenticationModule, Configuration::Bool));
	EmailVerification.modify()->Title = "Enable Email Verification";
	EmailVerification.modify()->Details = "If enabled then all new users must verify their email address after registration. Verification is done by sending an automatically generated email asking to verify the email.";
	EmailVerification.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EmailVerificationVal = DboSession.add(new ConfigurationBool());
	EmailVerificationVal.modify()->Value = false;
	EmailVerificationVal.modify()->DefaultValue = false;
	EmailVerification.modify()->BoolPtr = EmailVerificationVal;

	//FacebookOAuth
	Wt::Dbo::ptr<Configuration> FacebookOAuth = DboSession.add(new Configuration("FacebookOAuth", O.AuthenticationModule, Configuration::Bool));
	FacebookOAuth.modify()->Title = "Enable Facebook Login";
	FacebookOAuth.modify()->Details = "If enabled then users would be able to login from their facebook account. Note that facebook connect configurations also have to be changed.";
	FacebookOAuth.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> FacebookOAuthVal = DboSession.add(new ConfigurationBool());
	FacebookOAuthVal.modify()->Value = false;
	FacebookOAuthVal.modify()->DefaultValue = false;
	FacebookOAuth.modify()->BoolPtr = FacebookOAuthVal;

	//FacebookAppID
	Wt::Dbo::ptr<Configuration> FacebookAppID = DboSession.add(new Configuration("FacebookAppId", O.AuthenticationModule, Configuration::String));
	FacebookAppID.modify()->Title = "OAuth 2 Facebook App ID";
	FacebookAppID.modify()->Details = "Facebook app ID that you get by registering on OAuth 2.";
	FacebookAppID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppIDVal = DboSession.add(new ConfigurationString());
	FacebookAppIDVal.modify()->ExampleValue = "1234567890123456";
	FacebookAppIDVal.modify()->MinLength = 1;
	FacebookAppID.modify()->StringPtr = FacebookAppIDVal;

	//FacebookAppSecret
	Wt::Dbo::ptr<Configuration> FacebookAppSecret = DboSession.add(new Configuration("FacebookAppSecret", O.AuthenticationModule, Configuration::String));
	FacebookAppSecret.modify()->Title = "OAuth 2 Facebook App Secret";
	FacebookAppSecret.modify()->Details = "Facebook app secret that you get by registering on OAuth 2.";
	FacebookAppSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppSecretVal = DboSession.add(new ConfigurationString());
	FacebookAppSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	FacebookAppSecretVal.modify()->MinLength = 1;
	FacebookAppSecret.modify()->StringPtr = FacebookAppSecretVal;

	//GoogleOAuth
	Wt::Dbo::ptr<Configuration> GoogleOAuth = DboSession.add(new Configuration("GoogleOAuth", O.AuthenticationModule, Configuration::Bool));
	GoogleOAuth.modify()->Title = "Enable Google Login";
	GoogleOAuth.modify()->Details = "If enabled then users would be able to login from their google account. Note that google api configurations also have to be changed.";
	GoogleOAuth.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationBool> GoogleOAuthVal = DboSession.add(new ConfigurationBool());
	GoogleOAuthVal.modify()->Value = false;
	GoogleOAuthVal.modify()->DefaultValue = false;
	GoogleOAuth.modify()->BoolPtr = GoogleOAuthVal;

	//GoogleClientID
	Wt::Dbo::ptr<Configuration> GoogleClientID = DboSession.add(new Configuration("GoogleClientId", O.AuthenticationModule, Configuration::String));
	GoogleClientID.modify()->Title = "OAuth 2 Google Client ID";
	GoogleClientID.modify()->Details = "Google client ID that you get by registering on OAuth 2.";
	GoogleClientID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientIDVal = DboSession.add(new ConfigurationString());
	GoogleClientIDVal.modify()->ExampleValue = "1234567890123456";
	GoogleClientIDVal.modify()->MinLength = 1;
	GoogleClientID.modify()->StringPtr = GoogleClientIDVal;

	//GoogleClientSecret
	Wt::Dbo::ptr<Configuration> GoogleClientSecret = DboSession.add(new Configuration("GoogleClientSecret", O.AuthenticationModule, Configuration::String));
	GoogleClientSecret.modify()->Title = "OAuth 2 Google Client Secret";
	GoogleClientSecret.modify()->Details = "Google client secret that you get by registering on OAuth 2.";
	GoogleClientSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientSecretVal = DboSession.add(new ConfigurationString());
	GoogleClientSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	GoogleClientSecretVal.modify()->MinLength = 1;
	GoogleClientSecret.modify()->StringPtr = GoogleClientSecretVal;

	//HomePageAccessPathId
	Wt::Dbo::ptr<Configuration> HomePageAccessPathId = DboSession.add(new Configuration("HomePageAccessPathId", O.NavigationModule, Configuration::LongInt));
	HomePageAccessPathId.modify()->Title = "Home page access path";
	HomePageAccessPathId.modify()->Details = "This is the page which will be treated as the default or landing page.";
	HomePageAccessPathId.modify()->RestartRequired = false;
	O.HomePageAccessPathIdVal = DboSession.add(new ConfigurationLongInt());
	O.HomePageAccessPathIdVal.modify()->Value = 3;
	HomePageAccessPathId.modify()->LongIntPtr = O.HomePageAccessPathIdVal;

	//MobileAccessPathId
	Wt::Dbo::ptr<Configuration> MobileAccessPathId = DboSession.add(new Configuration("MobileAccessPathId", O.NavigationModule, Configuration::LongInt));
	MobileAccessPathId.modify()->Title = "Mobile version access path";
	MobileAccessPathId.modify()->Details = "The access path from where mobile version will be accessed.";
	MobileAccessPathId.modify()->RestartRequired = false;
	O.MobileAccessPathIdVal = DboSession.add(new ConfigurationLongInt());
	O.MobileAccessPathIdVal.modify()->Value = 2;
	MobileAccessPathId.modify()->LongIntPtr = O.MobileAccessPathIdVal;

	//ResourcesURL
	Wt::Dbo::ptr<Configuration> ResourcesURL = DboSession.add(new Configuration("ResourcesURL", O.StylesModule, Configuration::String));
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
	Wt::Dbo::ptr<Configuration> StylesURL = DboSession.add(new Configuration("StylesURL", O.StylesModule, Configuration::String));
	StylesURL.modify()->Title = "Images folder URL";
	StylesURL.modify()->Details = "Path to the images folder that contain general and style images.";
	StylesURL.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> StylesURLVal = DboSession.add(new ConfigurationString());
	StylesURLVal.modify()->Value = "/styles";
	StylesURLVal.modify()->DefaultValue = "/styles";
	StylesURLVal.modify()->ExampleValue = "/styles";
	StylesURLVal.modify()->MinLength = 1;
	StylesURL.modify()->StringPtr = StylesURLVal;

	//DefaultStyleName
	Wt::Dbo::ptr<Configuration> DefaultStyleName = DboSession.add(new Configuration("DefaultStyleName", O.StylesModule, Configuration::String));
	DefaultStyleName.modify()->Title = "Default style's name";
	DefaultStyleName.modify()->Details = "Name of the style which would be used to show as default.";
	DefaultStyleName.modify()->RestartRequired = false;
	O.DefaultStyleNameVal = DboSession.add(new ConfigurationString());
	DefaultStyleName.modify()->StringPtr = O.DefaultStyleNameVal;

	//DefaultStyleAuthor
	Wt::Dbo::ptr<Configuration> DefaultStyleAuthor = DboSession.add(new Configuration("DefaultStyleAuthor", O.StylesModule, Configuration::LongInt));
	DefaultStyleAuthor.modify()->Title = "Default style's author id";
	DefaultStyleAuthor.modify()->Details = "Author ID of the style which would be used to show as default.";
	DefaultStyleAuthor.modify()->RestartRequired = false;
	O.DefaultStyleAuthorVal = DboSession.add(new ConfigurationLongInt());
	O.DefaultStyleAuthorVal.modify()->Value = -1;
	DefaultStyleAuthor.modify()->LongIntPtr = O.DefaultStyleAuthorVal;
}
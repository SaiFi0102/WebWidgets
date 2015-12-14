#include "DboInstaller.h"

namespace WW
{
	namespace Installer
	{

void DboInstaller::insertConfigurations()
{
	//Hostname
	auto Hostname = new Dbo::Configuration("Hostname", O.ServerModule, Dbo::Configuration::String);
	Hostname->titleKey = addSingularKeyString(O.EnglishLanguage, "HostnameTitle", O.ConfigurationModule, "Hostname");
	Hostname->detailsKey = addSingularKeyString(O.EnglishLanguage, "HostnameDetails", O.ConfigurationModule, "This is the host name or domain name used to visit your website. If you have multiple host names, use the one with most priority. Do not enter the 'www.' part.");
	Hostname->restartRequired = false;

	auto HostnameVal = new Dbo::ConfigurationString(dboSession.add(Hostname));
	HostnameVal->value = "";
	HostnameVal->exampleValue = "mywebsite.com";
	HostnameVal->minLength = 1;
	dboSession.add(HostnameVal);

	//BaseURL
	auto BaseUrl = new Dbo::Configuration("BaseURL", O.ServerModule, Dbo::Configuration::String);
	BaseUrl->titleKey = addSingularKeyString(O.EnglishLanguage, "BaseUrlTitle", O.ConfigurationModule, "Base URL");
	BaseUrl->detailsKey = addSingularKeyString(O.EnglishLanguage, "BaseUrlDetails", O.ConfigurationModule, "This is the base URL that the user can see when browsing. Can be left empty in which case the website will use relative paths.");
	BaseUrl->restartRequired = true;

	auto BaseUrlVal = new Dbo::ConfigurationString(dboSession.add(BaseUrl));
	BaseUrlVal->value = "";
	BaseUrlVal->defaultValue = "";
	BaseUrlVal->recommendedValue = "";
	BaseUrlVal->exampleValue = "http://www.mywebsite.com/wt";
	dboSession.add(BaseUrlVal);

	//DosPuzzle
	auto DosPuzzle = new Dbo::Configuration("DosPuzzle", O.ServerModule, Dbo::Configuration::Bool);
	DosPuzzle->titleKey = addSingularKeyString(O.EnglishLanguage, "DosPuzzleTitle", O.ConfigurationModule, "Denial-of-Service protection puzzle");
	DosPuzzle->detailsKey = addSingularKeyString(O.EnglishLanguage, "DosPuzzleDetails", O.ConfigurationModule, "When enabled the server adds a puzzle to validate Ajax sessions in the first Ajax request. This is a simple measure which avoids Denial-of-Service attacks on Ajax sessions.");
	DosPuzzle->restartRequired = true;

	auto DosPuzzleVal = new Dbo::ConfigurationBool(dboSession.add(DosPuzzle));
	DosPuzzleVal->value = false;
	DosPuzzleVal->defaultValue = false;
	dboSession.add(DosPuzzleVal);

	//JavascriptDebug
	auto JavascriptDebug = new Dbo::Configuration("JavascriptDebug", O.ServerModule, Dbo::Configuration::Enum);
	JavascriptDebug->titleKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugTitle", O.ConfigurationModule, "JavaScript debug mode");
	JavascriptDebug->detailsKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugDetails", O.ConfigurationModule, "Enable this only if you are a developer and debugging JavaScript.");
	JavascriptDebug->expertWarningKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugExpertWarning", O.ConfigurationModule, "Only change this if you are a developer and debugging javascript on the website.");
	JavascriptDebug->restartRequired = true;

	auto JavascriptDebugValPtr = dboSession.add(new Dbo::ConfigurationEnum(dboSession.add(JavascriptDebug)));
	
	auto JavascriptDebugValOff = new Dbo::ConfigurationEnumValue(JavascriptDebugValPtr);
	JavascriptDebugValOff->value = 1;
	JavascriptDebugValOff->titleKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOffTitle", O.ConfigurationModule, "Off");
	JavascriptDebugValOff->detailsKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOffDetails", O.ConfigurationModule, "JavaScript errors are caught, logged on server-side and a simple error message is printed to indicate that something is terribly wrong.");
	dboSession.add(JavascriptDebugValOff);
	auto JavascriptDebugValOn1 = new Dbo::ConfigurationEnumValue(JavascriptDebugValPtr);
	JavascriptDebugValOn1->value = 2;
	JavascriptDebugValOn1->titleKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOn1Title", O.ConfigurationModule, "Log and rethrow");
	JavascriptDebugValOn1->detailsKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOn1Details", O.ConfigurationModule, "JavaScript errors are caught, logged on server-side and rethrown. This is useful for diagnosing a problem and debugging.");
	dboSession.add(JavascriptDebugValOn1);
	auto JavascriptDebugValOn2 = new Dbo::ConfigurationEnumValue(JavascriptDebugValPtr);
	JavascriptDebugValOn2->value = 3;
	JavascriptDebugValOn2->titleKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOn2Title", O.ConfigurationModule, "Naked");
	JavascriptDebugValOn2->detailsKey = addSingularKeyString(O.EnglishLanguage, "JavascriptDebugValOn2Details", O.ConfigurationModule, "JavaScript errors are not caught nor logged and the browser handles the error. This is useful if logging the error server-side is causing problems.");
	dboSession.add(JavascriptDebugValOn2);

	JavascriptDebugValPtr.modify()->value = JavascriptDebugValOff->value;
	JavascriptDebugValPtr.modify()->defaultValue = JavascriptDebugValOff->value;
	JavascriptDebugValPtr.modify()->recommendedValue = JavascriptDebugValOff->value;

	//MaxRequestSize
	auto MaxRequestSize = new Dbo::Configuration("MaxRequestSize", O.ServerModule, Dbo::Configuration::Int);
	MaxRequestSize->titleKey = addSingularKeyString(O.EnglishLanguage, "MaxRequestSizeTitle", O.ConfigurationModule, "Max request size");
	MaxRequestSize->detailsKey = addSingularKeyString(O.EnglishLanguage, "MaxRequestSizeDetails", O.ConfigurationModule, "Maximum request size in kilobytes. This must be increased to allow uploading larger sized files.");
	MaxRequestSize->restartRequired = true;

	auto MaxRequestSizeVal = new Dbo::ConfigurationInt(dboSession.add(MaxRequestSize));
	MaxRequestSizeVal->value = 128;
	MaxRequestSizeVal->defaultValue = 128;
	MaxRequestSizeVal->minValue = 128;
	dboSession.add(MaxRequestSizeVal);

	//ReverseProxy
	auto ReverseProxy = new Dbo::Configuration("ReverseProxy", O.ServerModule, Dbo::Configuration::Bool);
	ReverseProxy->titleKey = addSingularKeyString(O.EnglishLanguage, "ReverseProxyTitle", O.ConfigurationModule, "Behind reverse proxy");
	ReverseProxy->detailsKey = addSingularKeyString(O.EnglishLanguage, "ReverseProxyDetails", O.ConfigurationModule, "Enable this if the server is behind a reverse proxy.");
	ReverseProxy->restartRequired = true;

	auto ReverseProxyVal = new Dbo::ConfigurationBool(dboSession.add(ReverseProxy));
	ReverseProxyVal->value = false;
	ReverseProxyVal->defaultValue = false;
	dboSession.add(ReverseProxyVal);

	//SessionTimeout
	auto SessionTimeout = new Dbo::Configuration("SessionTimeout", O.ServerModule, Dbo::Configuration::Int);
	SessionTimeout->titleKey = addSingularKeyString(O.EnglishLanguage, "SessionTimeoutTitle", O.ConfigurationModule, "Session timeout delay");
	SessionTimeout->detailsKey = addSingularKeyString(O.EnglishLanguage, "SessionTimeoutDetails", O.ConfigurationModule, "Session timeout delay in seconds. Users inactive for this amount of time will be considered offline.");
	SessionTimeout->restartRequired = true;

	auto SessionTimeoutVal = new Dbo::ConfigurationInt(dboSession.add(SessionTimeout));
	SessionTimeoutVal->value = 600;
	SessionTimeoutVal->defaultValue = 600;
	SessionTimeoutVal->recommendedValue = 600;
	SessionTimeoutVal->minValue = 30;
	dboSession.add(SessionTimeoutVal);

	//SessionTracking
	auto SessionTracking = new Dbo::Configuration("SessionTracking", O.ServerModule, Dbo::Configuration::Enum);
	SessionTracking->titleKey = addSingularKeyString(O.EnglishLanguage, "SessionTrackingTitle", O.ConfigurationModule, "Session tracking strategy");
	SessionTracking->restartRequired = true;

	auto SessionTrackingValPtr = dboSession.add(new Dbo::ConfigurationEnum(dboSession.add(SessionTracking)));

	auto SessionTrackingValCookies = new Dbo::ConfigurationEnumValue(SessionTrackingValPtr);
	SessionTrackingValCookies->value = 1;
	SessionTrackingValCookies->titleKey = addSingularKeyString(O.EnglishLanguage, "SessionTrackingValCookiesTitle", O.ConfigurationModule, "Cookies");
	SessionTrackingValCookies->detailsKey = addSingularKeyString(O.EnglishLanguage, "SessionTrackingValCookiesDetails", O.ConfigurationModule, "Cookies will be used if available, otherwise URL rewriting will be for tracking sessions.");
	dboSession.add(SessionTrackingValCookies);
	auto SessionTrackingValURL = new Dbo::ConfigurationEnumValue(SessionTrackingValPtr);
	SessionTrackingValURL->value = 2;
	SessionTrackingValURL->titleKey = addSingularKeyString(O.EnglishLanguage, "SessionTrackingValURLTitle", O.ConfigurationModule, "Url");
	SessionTrackingValURL->detailsKey = addSingularKeyString(O.EnglishLanguage, "SessionTrackingValURLDetails", O.ConfigurationModule, "Only URL rewriting will be used for tracking sessions.");
	dboSession.add(SessionTrackingValURL);

	SessionTrackingValPtr.modify()->value = SessionTrackingValURL->value; //TODO!
	SessionTrackingValPtr.modify()->defaultValue = SessionTrackingValCookies->value;
	SessionTrackingValPtr.modify()->recommendedValue = SessionTrackingValCookies->value;

	//LogDebugLevel
	auto LogDebugLevel = new Dbo::Configuration("LogDebugLevel", O.LoggingModule, Dbo::Configuration::Bool);
	LogDebugLevel->titleKey = addSingularKeyString(O.EnglishLanguage, "LogDebugLevelTitle", O.ConfigurationModule, "Log debugging messages");
	LogDebugLevel->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogDebugLevelDetails", O.ConfigurationModule, "When enabled debugging events/messages will be included in the log file. These notices only gives information for developers and might not be meaningful to users.");
	LogDebugLevel->expertWarningKey = addSingularKeyString(O.EnglishLanguage, "LogDebugLevelExpertWarning", O.ConfigurationModule, "Only change this if you are a developer and are trying to debug javascript of the website.");
	LogDebugLevel->restartRequired = true;

	auto LogDebugLevelVal = new Dbo::ConfigurationBool(dboSession.add(LogDebugLevel));
	LogDebugLevelVal->value = false;
	LogDebugLevelVal->defaultValue = false;
	LogDebugLevelVal->recommendedValue = false;
	dboSession.add(LogDebugLevelVal);

	//LogDirectory
	auto LogDirectory = new Dbo::Configuration("LogDirectory", O.LoggingModule, Dbo::Configuration::String);
	LogDirectory->titleKey = addSingularKeyString(O.EnglishLanguage, "LogDirectoryTitle", O.ConfigurationModule, "Logging file directory");
	LogDirectory->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogDirectoryDetails", O.ConfigurationModule, "Location of where the logging file will be created. If this is left empty, no log file will be created but instead it will be stored in HTTP server logs. Disabling file logging is highly not recommended.");
	LogDirectory->restartRequired = true;

	auto LogDirectoryVal = new Dbo::ConfigurationString(dboSession.add(LogDirectory));
	LogDirectoryVal->value = "./logs";
	LogDirectoryVal->defaultValue = "./logs";
	LogDirectoryVal->exampleValue = "/etc/logs/website";
	dboSession.add(LogDirectoryVal);

	//LogErrorLevel
	auto LogErrorLevel = new Dbo::Configuration("LogErrorLevel", O.LoggingModule, Dbo::Configuration::Bool);
	LogErrorLevel->titleKey = addSingularKeyString(O.EnglishLanguage, "LogErrorLevelTitle", O.ConfigurationModule, "Log error messages");
	LogErrorLevel->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogErrorLevelDetails", O.ConfigurationModule, "When enabled errors will be included in the logs.");
	LogErrorLevel->restartRequired = true;

	auto LogErrorLevelVal = new Dbo::ConfigurationBool(dboSession.add(LogErrorLevel));
	LogErrorLevelVal->value = true;
	LogErrorLevelVal->defaultValue = true;
	LogErrorLevelVal->recommendedValue = true;
	dboSession.add(LogErrorLevelVal);

	//LogInfoLevel
	auto LogInfoLevel = new Dbo::Configuration("LogInfoLevel", O.LoggingModule, Dbo::Configuration::Bool);
	LogInfoLevel->titleKey = addSingularKeyString(O.EnglishLanguage, "LogInfoLevelTitle", O.ConfigurationModule, "Log informational notices");
	LogInfoLevel->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogInfoLevelDetails", O.ConfigurationModule, "When enabled informational notices will be included in the logs.");
	LogInfoLevel->restartRequired = true;

	auto LogInfoLevelVal = new Dbo::ConfigurationBool(dboSession.add(LogInfoLevel));
	LogInfoLevelVal->value = false;
	LogInfoLevelVal->defaultValue = false;
	dboSession.add(LogInfoLevelVal);

	//LogSecureLevel
	auto LogSecureLevel = new Dbo::Configuration("LogSecureLevel", O.LoggingModule, Dbo::Configuration::Bool);
	LogSecureLevel->titleKey = addSingularKeyString(O.EnglishLanguage, "LogSecureLevelTitle", O.ConfigurationModule, "Log security events");
	LogSecureLevel->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogSecureLevelDetails", O.ConfigurationModule, "When enabled security related messages will be included in the logs.");
	LogSecureLevel->restartRequired = true;

	auto LogSecureLevelVal = new Dbo::ConfigurationBool(dboSession.add(LogSecureLevel));
	LogSecureLevelVal->value = true;
	LogSecureLevelVal->defaultValue = true;
	LogSecureLevelVal->recommendedValue = true;
	dboSession.add(LogSecureLevelVal);

	//LogWarnLevel
	auto LogWarnLevel = new Dbo::Configuration("LogWarnLevel", O.LoggingModule, Dbo::Configuration::Bool);
	LogWarnLevel->titleKey = addSingularKeyString(O.EnglishLanguage, "LogWarnLevelTitle", O.ConfigurationModule, "Log warnings");
	LogWarnLevel->detailsKey = addSingularKeyString(O.EnglishLanguage, "LogWarnLevelDetails", O.ConfigurationModule, "When enabled warnings will be included in the log file. These warnings indicate that there was something could have caused an error and should be considered.\n\nRecommended Value: On");
	LogWarnLevel->restartRequired = true;

	auto LogWarnLevelVal = new Dbo::ConfigurationBool(dboSession.add(LogWarnLevel));
	LogWarnLevelVal->value = true;
	LogWarnLevelVal->defaultValue = true;
	LogWarnLevelVal->recommendedValue = true;
	dboSession.add(LogWarnLevelVal);

	//InternalPathMode
	auto InternalPathMode = new Dbo::Configuration("InternalPathMode", O.LocalizationModule, Dbo::Configuration::Enum);
	InternalPathMode->titleKey = addSingularKeyString(O.EnglishLanguage, "InternalPathModeTitle", O.ConfigurationModule, "Presentation of language in internal path");
	InternalPathMode->restartRequired = false;

	auto InternalPathModeValPtr = dboSession.add(new Dbo::ConfigurationEnum(dboSession.add(InternalPathMode)));

	auto IPMVShow = new Dbo::ConfigurationEnumValue(InternalPathModeValPtr);
	IPMVShow->value = 1;
	IPMVShow->titleKey = addSingularKeyString(O.EnglishLanguage, "IPMVShowTitle", O.ConfigurationModule, "Always show language");
	IPMVShow->detailsKey = addSingularKeyString(O.EnglishLanguage, "IPMVShowDetails", O.ConfigurationModule, "In this mode language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.");
	dboSession.add(IPMVShow);
	auto IPMVShowHideDefault = new Dbo::ConfigurationEnumValue(InternalPathModeValPtr);
	IPMVShowHideDefault->value = 2;
	IPMVShowHideDefault->titleKey = addSingularKeyString(O.EnglishLanguage, "IPMVShowHideDefaultTitle", O.ConfigurationModule, "Always show language but hide default language");
	IPMVShowHideDefault->detailsKey = addSingularKeyString(O.EnglishLanguage, "IPMVShowHideDefaultDetails", O.ConfigurationModule, "In this mode all languages except for the default language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.");
	dboSession.add(IPMVShowHideDefault);
	auto IPMVNoRestrictionHideDefault = new Dbo::ConfigurationEnumValue(InternalPathModeValPtr);
	IPMVNoRestrictionHideDefault->value = 3;
	IPMVNoRestrictionHideDefault->titleKey = addSingularKeyString(O.EnglishLanguage, "IPMVNoRestrictionHideDefaultTitle", O.ConfigurationModule, "No restrictions but hide default language");
	IPMVNoRestrictionHideDefault->detailsKey = addSingularKeyString(O.EnglishLanguage, "IPMVNoRestrictionHideDefaultDetails", O.ConfigurationModule, "In this mode language will not be automatically removed or added except the default language will never be shown.");
	dboSession.add(IPMVNoRestrictionHideDefault);
	auto IPMVNoRestriction = new Dbo::ConfigurationEnumValue(InternalPathModeValPtr);
	IPMVNoRestriction->value = 4;
	IPMVNoRestriction->titleKey = addSingularKeyString(O.EnglishLanguage, "IPMVNoRestrictionTitle", O.ConfigurationModule, "No restrictions");
	IPMVNoRestriction->detailsKey = addSingularKeyString(O.EnglishLanguage, "IPMVNoRestrictionDetails", O.ConfigurationModule, "In this mode language will not be automatically removed or added in any case.");
	dboSession.add(IPMVNoRestriction);

	InternalPathModeValPtr.modify()->value = IPMVShowHideDefault->value;
	InternalPathModeValPtr.modify()->defaultValue = IPMVShowHideDefault->value;

	//EnableTokens
	auto EnableTokens = new Dbo::Configuration("EnableTokens", O.AuthenticationModule, Dbo::Configuration::Bool);
	EnableTokens->titleKey = addSingularKeyString(O.EnglishLanguage, "EnableTokensTitle", O.ConfigurationModule, "Enable Remember-Me Tokens");
	EnableTokens->detailsKey = addSingularKeyString(O.EnglishLanguage, "EnableTokensDetails", O.ConfigurationModule, "If enabled then remember me feature before login will be enabled. This means that if users agree to keep them self logged in, they will not have to login again even after end of session. Cookies will be set as tokens.");
	EnableTokens->restartRequired = true;

	auto EnableTokensVal = new Dbo::ConfigurationBool(dboSession.add(EnableTokens));
	EnableTokensVal->value = true;
	EnableTokensVal->defaultValue = true;
	EnableTokensVal->recommendedValue = true;
	dboSession.add(EnableTokensVal);

	//LoginCookie
	auto LoginCookie = new Dbo::Configuration("LoginCookie", O.AuthenticationModule, Dbo::Configuration::String);
	LoginCookie->titleKey = addSingularKeyString(O.EnglishLanguage, "LoginCookieTitle", O.ConfigurationModule, "Remember-Me Cookie Name");
	LoginCookie->detailsKey = addSingularKeyString(O.EnglishLanguage, "LoginCookieDetails", O.ConfigurationModule, "Name of the cookie that will be used to set the remember-me token. Do not change this if you do not know what this means.");
	LoginCookie->expertWarningKey = addSingularKeyString(O.EnglishLanguage, "LoginCookieExpertWarning", O.ConfigurationModule, "Changing cookie name to an invalid name may prevent the remember-me function to work properly.");
	LoginCookie->restartRequired = true;

	auto LoginCookieVal = new Dbo::ConfigurationString(dboSession.add(LoginCookie));
	LoginCookieVal->value = "auth";
	LoginCookieVal->defaultValue = "auth";
	LoginCookieVal->exampleValue = "login";
	LoginCookieVal->minLength = 1;
	//LoginCookieVal->MaxLength = 3; //TODO
	dboSession.add(LoginCookieVal);

	//EmailVerification
	auto EmailVerification = new Dbo::Configuration("EmailVerification", O.AuthenticationModule, Dbo::Configuration::Bool);
	EmailVerification->titleKey = addSingularKeyString(O.EnglishLanguage, "EmailVerificationTitle", O.ConfigurationModule, "Enable Email Verification");
	EmailVerification->detailsKey = addSingularKeyString(O.EnglishLanguage, "EmailVerificationDetails", O.ConfigurationModule, "If enabled then all new users must verify their email address after registration. Verification is done by sending an automatically generated email asking to verify the email.");
	EmailVerification->restartRequired = true;

	auto EmailVerificationVal = new Dbo::ConfigurationBool(dboSession.add(EmailVerification));
	EmailVerificationVal->value = false;
	EmailVerificationVal->defaultValue = false;
	dboSession.add(EmailVerificationVal);

	//FacebookOAuth
	auto FacebookOAuth = new Dbo::Configuration("FacebookOAuth", O.AuthenticationModule, Dbo::Configuration::Bool);
	FacebookOAuth->titleKey = addSingularKeyString(O.EnglishLanguage, "FacebookOAuthTitle", O.ConfigurationModule, "Enable Facebook Login");
	FacebookOAuth->detailsKey = addSingularKeyString(O.EnglishLanguage, "FacebookOAuthDetails", O.ConfigurationModule, "If enabled then users would be able to login from their facebook account. Note that facebook connect configurations also have to be changed.");
	FacebookOAuth->restartRequired = true;

	auto FacebookOAuthVal = new Dbo::ConfigurationBool(dboSession.add(FacebookOAuth));
	FacebookOAuthVal->value = false;
	FacebookOAuthVal->defaultValue = false;
	dboSession.add(FacebookOAuthVal);

	//FacebookAppID
	auto FacebookAppID = new Dbo::Configuration("FacebookAppId", O.AuthenticationModule, Dbo::Configuration::String);
	FacebookAppID->titleKey = addSingularKeyString(O.EnglishLanguage, "FacebookAppIDTitle", O.ConfigurationModule, "OAuth 2 Facebook App ID");
	FacebookAppID->detailsKey = addSingularKeyString(O.EnglishLanguage, "FacebookAppIDDetails", O.ConfigurationModule, "Facebook app ID that you get by registering on OAuth 2.");
	FacebookAppID->restartRequired = true;

	auto FacebookAppIDVal = new Dbo::ConfigurationString(dboSession.add(FacebookAppID));
	FacebookAppIDVal->exampleValue = "1234567890123456";
	FacebookAppIDVal->minLength = 1;
	dboSession.add(FacebookAppIDVal);

	//FacebookAppSecret
	auto FacebookAppSecret = new Dbo::Configuration("FacebookAppSecret", O.AuthenticationModule, Dbo::Configuration::String);
	FacebookAppSecret->titleKey = addSingularKeyString(O.EnglishLanguage, "FacebookAppSecretTitle", O.ConfigurationModule, "OAuth 2 Facebook App Secret");
	FacebookAppSecret->detailsKey = addSingularKeyString(O.EnglishLanguage, "FacebookAppSecretDetails", O.ConfigurationModule, "Facebook app secret that you get by registering on OAuth 2.");
	FacebookAppSecret->restartRequired = true;

	auto FacebookAppSecretVal = new Dbo::ConfigurationString(dboSession.add(FacebookAppSecret));
	FacebookAppSecretVal->exampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	FacebookAppSecretVal->minLength = 1;
	dboSession.add(FacebookAppSecretVal);

	//GoogleOAuth
	auto GoogleOAuth = new Dbo::Configuration("GoogleOAuth", O.AuthenticationModule, Dbo::Configuration::Bool);
	GoogleOAuth->titleKey = addSingularKeyString(O.EnglishLanguage, "GoogleOAuthTitle", O.ConfigurationModule, "Enable Google Login");
	GoogleOAuth->detailsKey = addSingularKeyString(O.EnglishLanguage, "GoogleOAuthDetails", O.ConfigurationModule, "If enabled then users would be able to login from their google account. Note that google api configurations also have to be changed.");
	GoogleOAuth->restartRequired = false;

	auto GoogleOAuthVal = new Dbo::ConfigurationBool(dboSession.add(GoogleOAuth));
	GoogleOAuthVal->value = false;
	GoogleOAuthVal->defaultValue = false;
	dboSession.add(GoogleOAuthVal);

	//GoogleClientID
	auto GoogleClientID = new Dbo::Configuration("GoogleClientId", O.AuthenticationModule, Dbo::Configuration::String);
	GoogleClientID->titleKey = addSingularKeyString(O.EnglishLanguage, "GoogleClientIDTitle", O.ConfigurationModule, "OAuth 2 Google Client ID");
	GoogleClientID->detailsKey = addSingularKeyString(O.EnglishLanguage, "GoogleClientIDDetails", O.ConfigurationModule, "Google client ID that you get by registering on OAuth 2.");
	GoogleClientID->restartRequired = true;

	auto GoogleClientIDVal = new Dbo::ConfigurationString(dboSession.add(GoogleClientID));
	GoogleClientIDVal->exampleValue = "1234567890123456";
	GoogleClientIDVal->minLength = 1;
	dboSession.add(GoogleClientIDVal);

	//GoogleClientSecret
	auto GoogleClientSecret = new Dbo::Configuration("GoogleClientSecret", O.AuthenticationModule, Dbo::Configuration::String);
	GoogleClientSecret->titleKey = addSingularKeyString(O.EnglishLanguage, "GoogleClientSecretTitle", O.ConfigurationModule, "OAuth 2 Google Client Secret");
	GoogleClientSecret->detailsKey = addSingularKeyString(O.EnglishLanguage, "GoogleClientSecretDetails", O.ConfigurationModule, "Google client secret that you get by registering on OAuth 2.");
	GoogleClientSecret->restartRequired = true;

	auto GoogleClientSecretVal = new Dbo::ConfigurationString(dboSession.add(GoogleClientSecret));
	GoogleClientSecretVal->exampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	GoogleClientSecretVal->minLength = 1;
	dboSession.add(GoogleClientSecretVal);

	//ResourcesURL
	auto ResourcesURL = new Dbo::Configuration("ResourcesURL", O.StylingModule, Dbo::Configuration::String);
	ResourcesURL->titleKey = addSingularKeyString(O.EnglishLanguage, "ResourcesURLTitle", O.ConfigurationModule, "Resources folder URL");
	ResourcesURL->detailsKey = addSingularKeyString(O.EnglishLanguage, "ResourcesURLDetails", O.ConfigurationModule, "Path to Wt's resources folder.");
	ResourcesURL->restartRequired = true;

	auto ResourcesURLVal = new Dbo::ConfigurationString(dboSession.add(ResourcesURL));
	ResourcesURLVal->value = "/resources";
	ResourcesURLVal->defaultValue = "/resources";
	ResourcesURLVal->exampleValue = "/resources";
	ResourcesURLVal->minLength = 1;
	dboSession.add(ResourcesURLVal);

	//StylesURL
	auto StylesURL = new Dbo::Configuration("StylesURL", O.StylingModule, Dbo::Configuration::String);
	StylesURL->titleKey = addSingularKeyString(O.EnglishLanguage, "StylesURLTitle", O.ConfigurationModule, "Images folder URL");
	StylesURL->detailsKey = addSingularKeyString(O.EnglishLanguage, "StylesURLDetails", O.ConfigurationModule, "Path to the images folder that contain general and style images.");
	StylesURL->restartRequired = false;

	auto StylesURLVal = new Dbo::ConfigurationString(dboSession.add(StylesURL));
	StylesURLVal->value = "/styles";
	StylesURLVal->defaultValue = "/styles";
	StylesURLVal->exampleValue = "/styles";
	StylesURLVal->minLength = 1;
	dboSession.add(StylesURLVal);
}

	}
}

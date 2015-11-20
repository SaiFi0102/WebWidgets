#include "DboInstaller/DboInstaller.h"

void DboInstaller::InsertConfigurations()
{
	//Hostname
	Wt::Dbo::ptr<Configuration> Hostname = DboSession.add(new Configuration("Hostname", O.ServerModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> HostnameTitle = DboSession.add(new SingularKey("HostnameTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(HostnameTitle, "Hostname")));
	Hostname.modify()->TitleKey = HostnameTitle;
	Wt::Dbo::ptr<SingularKey> HostnameDetails = DboSession.add(new SingularKey("HostnameDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(HostnameDetails, "This is the host name or domain name used to visit your website. If you have multiple host names, use the one with most priority. Do not enter the 'www.' part.")));
	Hostname.modify()->DetailsKey = HostnameDetails;
	Hostname.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> HostnameVal = DboSession.add(new ConfigurationString());
	HostnameVal.modify()->Value = "";
	HostnameVal.modify()->ExampleValue = "mywebsite.com";
	HostnameVal.modify()->MinLength = 1;
	Hostname.modify()->StringPtr = HostnameVal;

	//BaseURL
	Wt::Dbo::ptr<Configuration> BaseUrl = DboSession.add(new Configuration("BaseURL", O.ServerModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> BaseUrlTitle = DboSession.add(new SingularKey("BaseUrlTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(BaseUrlTitle, "Base URL")));
	BaseUrl.modify()->TitleKey = BaseUrlTitle;
	Wt::Dbo::ptr<SingularKey> BaseUrlDetails = DboSession.add(new SingularKey("BaseUrlDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(BaseUrlDetails, "This is the base URL that the user can see when browsing. Can be left empty in which case the website will use relative paths.")));
	BaseUrl.modify()->DetailsKey = BaseUrlDetails;
	BaseUrl.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> BaseUrlVal = DboSession.add(new ConfigurationString());
	BaseUrlVal.modify()->Value = "";
	BaseUrlVal.modify()->DefaultValue = "";
	BaseUrlVal.modify()->RecommendedValue = "";
	BaseUrlVal.modify()->ExampleValue = "http://www.mywebsite.com/wt";
	BaseUrl.modify()->StringPtr = BaseUrlVal;

	//DosPuzzle
	Wt::Dbo::ptr<Configuration> DosPuzzle = DboSession.add(new Configuration("DosPuzzle", O.ServerModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> DosPuzzleTitle = DboSession.add(new SingularKey("DosPuzzleTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(DosPuzzleTitle, "Denial-of-Service protection puzzle")));
	DosPuzzle.modify()->TitleKey = DosPuzzleTitle;
	Wt::Dbo::ptr<SingularKey> DosPuzzleDetails = DboSession.add(new SingularKey("DosPuzzleDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(DosPuzzleDetails, "When enabled the server adds a puzzle to validate Ajax sessions in the first Ajax request. This is a simple measure which avoids Denial-of-Service attacks on Ajax sessions.")));
	DosPuzzle.modify()->DetailsKey = DosPuzzleDetails;
	DosPuzzle.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> DosPuzzleVal = DboSession.add(new ConfigurationBool());
	DosPuzzleVal.modify()->Value = false;
	DosPuzzleVal.modify()->DefaultValue = false;
	DosPuzzle.modify()->BoolPtr = DosPuzzleVal;

	//JavascriptDebug
	Wt::Dbo::ptr<Configuration> JavascriptDebug = DboSession.add(new Configuration("JavascriptDebug", O.ServerModule, Configuration::Enum));
	Wt::Dbo::ptr<SingularKey> JavascriptDebugTitle = DboSession.add(new SingularKey("JavascriptDebugTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugTitle, "JavaScript debug mode")));
	JavascriptDebug.modify()->TitleKey = JavascriptDebugTitle;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugDetails = DboSession.add(new SingularKey("JavascriptDebugDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugDetails, "Enable this only if you are a developer and debugging JavaScript.")));
	JavascriptDebug.modify()->DetailsKey = JavascriptDebugDetails;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugExpertWarning = DboSession.add(new SingularKey("JavascriptDebugExpertWarning", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugExpertWarning, "Only change this if you are a developer and debugging javascript on the website.")));
	JavascriptDebug.modify()->ExpertWarningKey = JavascriptDebugExpertWarning;
	JavascriptDebug.modify()->RestartRequired = true;

	Wt::Dbo::ptr<ConfigurationEnum> JavascriptDebugVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOff = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOff.modify()->Value = 1;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOffTitle = DboSession.add(new SingularKey("JavascriptDebugValOffTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOffTitle, "Off")));
	JavascriptDebugValOff.modify()->TitleKey = JavascriptDebugValOffTitle;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOffDetails = DboSession.add(new SingularKey("JavascriptDebugValOffDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOffDetails, "JavaScript errors are caught, logged on server-side and a simple error message is printed to indicate that something is terribly wrong.")));
	JavascriptDebugValOff.modify()->DetailsKey = JavascriptDebugValOffDetails;
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn1 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn1.modify()->Value = 2;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOn1Title = DboSession.add(new SingularKey("JavascriptDebugValOn1Title", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOn1Title, "Log and rethrow")));
	JavascriptDebugValOn1.modify()->TitleKey = JavascriptDebugValOn1Title;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOn1Details = DboSession.add(new SingularKey("JavascriptDebugValOn1Details", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOn1Details, "JavaScript errors are caught, logged on server-side and rethrown. This is useful for diagnosing a problem and debugging.")));
	JavascriptDebugValOn1.modify()->DetailsKey = JavascriptDebugValOn1Details;
	Wt::Dbo::ptr<ConfigurationEnumValue> JavascriptDebugValOn2 = DboSession.add(new ConfigurationEnumValue());
	JavascriptDebugValOn2.modify()->Value = 3;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOn2Title = DboSession.add(new SingularKey("JavascriptDebugValOn2Title", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOn2Title, "Naked")));
	JavascriptDebugValOn2.modify()->TitleKey = JavascriptDebugValOn2Title;
	Wt::Dbo::ptr<SingularKey> JavascriptDebugValOn2Details = DboSession.add(new SingularKey("JavascriptDebugValOn2Details", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(JavascriptDebugValOn2Details, "JavaScript errors are not caught nor logged and the browser handles the error. This is useful if logging the error server-side is causing problems.")));
	JavascriptDebugValOn2.modify()->DetailsKey = JavascriptDebugValOn2Details;

	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOff);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn1);
	JavascriptDebugVal.modify()->EnumValueCollection.insert(JavascriptDebugValOn2);

	JavascriptDebugVal.modify()->Value = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->DefaultValue = JavascriptDebugValOff->Value;
	JavascriptDebugVal.modify()->RecommendedValue = JavascriptDebugValOff->Value;

	JavascriptDebug.modify()->EnumPtr = JavascriptDebugVal;

	//MaxRequestSize
	Wt::Dbo::ptr<Configuration> MaxRequestSize = DboSession.add(new Configuration("MaxRequestSize", O.ServerModule, Configuration::Int));
	Wt::Dbo::ptr<SingularKey> MaxRequestSizeTitle = DboSession.add(new SingularKey("MaxRequestSizeTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(MaxRequestSizeTitle, "Max request size")));
	MaxRequestSize.modify()->TitleKey = MaxRequestSizeTitle;
	Wt::Dbo::ptr<SingularKey> MaxRequestSizeDetails = DboSession.add(new SingularKey("MaxRequestSizeDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(MaxRequestSizeDetails, "Maximum request size in kilobytes. This must be increased to allow uploading larger sized files.")));
	MaxRequestSize.modify()->DetailsKey = MaxRequestSizeDetails;
	MaxRequestSize.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationInt> MaxRequestSizeVal = DboSession.add(new ConfigurationInt());
	MaxRequestSizeVal.modify()->Value = 128;
	MaxRequestSizeVal.modify()->DefaultValue = 128;
	MaxRequestSizeVal.modify()->MinValue = 128;
	MaxRequestSize.modify()->IntPtr = MaxRequestSizeVal;

	//ReverseProxy
	Wt::Dbo::ptr<Configuration> ReverseProxy = DboSession.add(new Configuration("ReverseProxy", O.ServerModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> ReverseProxyTitle = DboSession.add(new SingularKey("ReverseProxyTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(ReverseProxyTitle, "Behind reverse proxy")));
	ReverseProxy.modify()->TitleKey = ReverseProxyTitle;
	Wt::Dbo::ptr<SingularKey> ReverseProxyDetails = DboSession.add(new SingularKey("ReverseProxyDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(ReverseProxyDetails, "Enable this if the server is behind a reverse proxy.")));
	ReverseProxy.modify()->DetailsKey = ReverseProxyDetails;
	ReverseProxy.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> ReverseProxyVal = DboSession.add(new ConfigurationBool());
	ReverseProxyVal.modify()->Value = false;
	ReverseProxyVal.modify()->DefaultValue = false;
	ReverseProxy.modify()->BoolPtr = ReverseProxyVal;

	//SessionTimeout
	Wt::Dbo::ptr<Configuration> SessionTimeout = DboSession.add(new Configuration("SessionTimeout", O.ServerModule, Configuration::Int));
	Wt::Dbo::ptr<SingularKey> SessionTimeoutTitle = DboSession.add(new SingularKey("SessionTimeoutTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTimeoutTitle, "Session timeout delay")));
	SessionTimeout.modify()->TitleKey = SessionTimeoutTitle;
	Wt::Dbo::ptr<SingularKey> SessionTimeoutDetails = DboSession.add(new SingularKey("SessionTimeoutDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTimeoutDetails, "Session timeout delay in seconds. Users inactive for this amount of time will be considered offline.")));
	SessionTimeout.modify()->DetailsKey = SessionTimeoutDetails;
	SessionTimeout.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationInt> SessionTimeoutVal = DboSession.add(new ConfigurationInt());
	SessionTimeoutVal.modify()->Value = 600;
	SessionTimeoutVal.modify()->DefaultValue = 600;
	SessionTimeoutVal.modify()->RecommendedValue = 600;
	SessionTimeoutVal.modify()->MinValue = 30;
	SessionTimeout.modify()->IntPtr = SessionTimeoutVal;

	//SessionTracking
	Wt::Dbo::ptr<Configuration> SessionTracking = DboSession.add(new Configuration("SessionTracking", O.ServerModule, Configuration::Enum));
	Wt::Dbo::ptr<SingularKey> SessionTrackingTitle = DboSession.add(new SingularKey("SessionTrackingTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTrackingTitle, "Session tracking strategy")));
	SessionTracking.modify()->TitleKey = SessionTrackingTitle;
	SessionTracking.modify()->RestartRequired = true;

	Wt::Dbo::ptr<ConfigurationEnum> SessionTrackingVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> SessionTrackingValCookies = DboSession.add(new ConfigurationEnumValue());
	SessionTrackingValCookies.modify()->Value = 1;
	Wt::Dbo::ptr<SingularKey> SessionTrackingValCookiesTitle = DboSession.add(new SingularKey("SessionTrackingValCookiesTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTrackingValCookiesTitle, "Cookies")));
	SessionTrackingValCookies.modify()->TitleKey = SessionTrackingValCookiesTitle;
	Wt::Dbo::ptr<SingularKey> SessionTrackingValCookiesDetails = DboSession.add(new SingularKey("SessionTrackingValCookiesDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTrackingValCookiesDetails, "Cookies will be used if available, otherwise URL rewriting will be for tracking sessions.")));
	SessionTrackingValCookies.modify()->DetailsKey = SessionTrackingValCookiesDetails;
	Wt::Dbo::ptr<ConfigurationEnumValue> SessionTrackingValURL = DboSession.add(new ConfigurationEnumValue());
	SessionTrackingValURL.modify()->Value = 2;
	Wt::Dbo::ptr<SingularKey> SessionTrackingValURLTitle = DboSession.add(new SingularKey("SessionTrackingValURLTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTrackingValURLTitle, "Url")));
	SessionTrackingValURL.modify()->TitleKey = SessionTrackingValURLTitle;
	Wt::Dbo::ptr<SingularKey> SessionTrackingValURLDetails = DboSession.add(new SingularKey("SessionTrackingValURLDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(SessionTrackingValURLDetails, "Only URL rewriting will be used for tracking sessions.")));
	SessionTrackingValURL.modify()->DetailsKey = SessionTrackingValURLDetails;

	SessionTrackingVal.modify()->EnumValueCollection.insert(SessionTrackingValCookies);
	SessionTrackingVal.modify()->EnumValueCollection.insert(SessionTrackingValURL);

	SessionTrackingVal.modify()->Value = SessionTrackingValCookies->Value;
	SessionTrackingVal.modify()->DefaultValue = SessionTrackingValCookies->Value;
	SessionTrackingVal.modify()->RecommendedValue = SessionTrackingValCookies->Value;

	SessionTracking.modify()->EnumPtr = SessionTrackingVal;

	//LogDebugLevel
	Wt::Dbo::ptr<Configuration> LogDebugLevel = DboSession.add(new Configuration("LogDebugLevel", O.LoggingModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> LogDebugLevelTitle = DboSession.add(new SingularKey("LogDebugLevelTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogDebugLevelTitle, "Log debugging messages")));
	LogDebugLevel.modify()->TitleKey = LogDebugLevelTitle;
	Wt::Dbo::ptr<SingularKey> LogDebugLevelDetails = DboSession.add(new SingularKey("LogDebugLevelDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogDebugLevelDetails, "When enabled debugging events/messages will be included in the log file. These notices only gives information for developers and might not be meaningful to users.")));
	LogDebugLevel.modify()->DetailsKey = LogDebugLevelDetails;
	Wt::Dbo::ptr<SingularKey> LogDebugLevelExpertWarning = DboSession.add(new SingularKey("LogDebugLevelExpertWarning", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogDebugLevelExpertWarning, "Only change this if you are a developer and are trying to debug javascript of the website.")));
	LogDebugLevel.modify()->ExpertWarningKey = LogDebugLevelExpertWarning;
	LogDebugLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogDebugLevelVal = DboSession.add(new ConfigurationBool());
	LogDebugLevelVal.modify()->Value = false;
	LogDebugLevelVal.modify()->DefaultValue = false;
	LogDebugLevelVal.modify()->RecommendedValue = false;
	LogDebugLevel.modify()->BoolPtr = LogDebugLevelVal;

	//LogDirectory
	Wt::Dbo::ptr<Configuration> LogDirectory = DboSession.add(new Configuration("LogDirectory", O.LoggingModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> LogDirectoryTitle = DboSession.add(new SingularKey("LogDirectoryTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogDirectoryTitle, "Logging file directory")));
	LogDirectory.modify()->TitleKey = LogDirectoryTitle;
	Wt::Dbo::ptr<SingularKey> LogDirectoryDetails = DboSession.add(new SingularKey("LogDirectoryDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogDirectoryDetails, "Location of where the logging file will be created. If this is left empty, no log file will be created but instead it will be stored in HTTP server logs. Disabling file logging is highly not recommended.")));
	LogDirectory.modify()->DetailsKey = LogDirectoryDetails;
	LogDirectory.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> LogDirectoryVal = DboSession.add(new ConfigurationString());
	LogDirectoryVal.modify()->Value = "./logs";
	LogDirectoryVal.modify()->DefaultValue = "./logs";
	LogDirectoryVal.modify()->ExampleValue = "/etc/logs/website";
	LogDirectory.modify()->StringPtr = LogDirectoryVal;

	//LogErrorLevel
	Wt::Dbo::ptr<Configuration> LogErrorLevel = DboSession.add(new Configuration("LogErrorLevel", O.LoggingModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> LogErrorLevelTitle = DboSession.add(new SingularKey("LogErrorLevelTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogErrorLevelTitle, "Log error messages")));
	LogErrorLevel.modify()->TitleKey = LogErrorLevelTitle;
	Wt::Dbo::ptr<SingularKey> LogErrorLevelDetails = DboSession.add(new SingularKey("LogErrorLevelDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogErrorLevelDetails, "When enabled errors will be included in the logs.")));
	LogErrorLevel.modify()->DetailsKey = LogErrorLevelDetails;
	LogErrorLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogErrorLevelVal = DboSession.add(new ConfigurationBool());
	LogErrorLevelVal.modify()->Value = true;
	LogErrorLevelVal.modify()->DefaultValue = true;
	LogErrorLevelVal.modify()->RecommendedValue = true;
	LogErrorLevel.modify()->BoolPtr = LogErrorLevelVal;

	//LogInfoLevel
	Wt::Dbo::ptr<Configuration> LogInfoLevel = DboSession.add(new Configuration("LogInfoLevel", O.LoggingModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> LogInfoLevelTitle = DboSession.add(new SingularKey("LogInfoLevelTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogInfoLevelTitle, "Log informational notices")));
	LogInfoLevel.modify()->TitleKey = LogInfoLevelTitle;
	Wt::Dbo::ptr<SingularKey> LogInfoLevelDetails = DboSession.add(new SingularKey("LogInfoLevelDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogInfoLevelDetails, "When enabled informational notices will be included in the logs.")));
	LogInfoLevel.modify()->DetailsKey = LogInfoLevelDetails;
	LogInfoLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogInfoLevelVal = DboSession.add(new ConfigurationBool());
	LogInfoLevelVal.modify()->Value = false;
	LogInfoLevelVal.modify()->DefaultValue = false;
	LogInfoLevel.modify()->BoolPtr = LogInfoLevelVal;

	//LogSecureLevel
	Wt::Dbo::ptr<Configuration> LogSecureLevel = DboSession.add(new Configuration("LogSecureLevel", O.LoggingModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> LogSecureLevelTitle = DboSession.add(new SingularKey("LogSecureLevelTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogSecureLevelTitle, "Log security events")));
	LogSecureLevel.modify()->TitleKey = LogSecureLevelTitle;
	Wt::Dbo::ptr<SingularKey> LogSecureLevelDetails = DboSession.add(new SingularKey("LogSecureLevelDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogSecureLevelDetails, "When enabled security related messages will be included in the logs.")));
	LogSecureLevel.modify()->DetailsKey = LogSecureLevelDetails;
	LogSecureLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogSecureLevelVal = DboSession.add(new ConfigurationBool());
	LogSecureLevelVal.modify()->Value = true;
	LogSecureLevelVal.modify()->DefaultValue = true;
	LogSecureLevelVal.modify()->RecommendedValue = true;
	LogSecureLevel.modify()->BoolPtr = LogSecureLevelVal;

	//LogWarnLevel
	Wt::Dbo::ptr<Configuration> LogWarnLevel = DboSession.add(new Configuration("LogWarnLevel", O.LoggingModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> LogWarnLevelTitle = DboSession.add(new SingularKey("LogWarnLevelTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogWarnLevelTitle, "Log warnings")));
	LogWarnLevel.modify()->TitleKey = LogWarnLevelTitle;
	Wt::Dbo::ptr<SingularKey> LogWarnLevelDetails = DboSession.add(new SingularKey("LogWarnLevelDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LogWarnLevelDetails, "When enabled warnings will be included in the log file. These warnings indicate that there was something could have caused an error and should be considered.\n\nRecommended Value: On")));
	LogWarnLevel.modify()->DetailsKey = LogWarnLevelDetails;
	LogWarnLevel.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> LogWarnLevelVal = DboSession.add(new ConfigurationBool());
	LogWarnLevelVal.modify()->Value = true;
	LogWarnLevelVal.modify()->DefaultValue = true;
	LogWarnLevelVal.modify()->RecommendedValue = true;
	LogWarnLevel.modify()->BoolPtr = LogWarnLevelVal;

	//InternalPathMode
	Wt::Dbo::ptr<Configuration> InternalPathMode = DboSession.add(new Configuration("InternalPathMode", O.LocalizationModule, Configuration::Enum));
	Wt::Dbo::ptr<SingularKey> InternalPathModeTitle = DboSession.add(new SingularKey("InternalPathModeTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(InternalPathModeTitle, "Presentation of language in internal path")));
	InternalPathMode.modify()->TitleKey = InternalPathModeTitle;
	InternalPathMode.modify()->RestartRequired = false;

	Wt::Dbo::ptr<ConfigurationEnum> InternalPathModeVal = DboSession.add(new ConfigurationEnum());
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVShow = DboSession.add(new ConfigurationEnumValue());
	IPMVShow.modify()->Value = 1;
	Wt::Dbo::ptr<SingularKey> IPMVShowTitle = DboSession.add(new SingularKey("IPMVShowTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVShowTitle, "Always show language")));
	IPMVShow.modify()->TitleKey = IPMVShowTitle;
	Wt::Dbo::ptr<SingularKey> IPMVShowDetails = DboSession.add(new SingularKey("IPMVShowDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVShowDetails, "In this mode language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.")));
	IPMVShow.modify()->DetailsKey = IPMVShowDetails;
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVShowHideDefault = DboSession.add(new ConfigurationEnumValue());
	IPMVShowHideDefault.modify()->Value = 2;
	Wt::Dbo::ptr<SingularKey> IPMVShowHideDefaultTitle = DboSession.add(new SingularKey("IPMVShowHideDefaultTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVShowHideDefaultTitle, "Always show language but hide default language")));
	IPMVShowHideDefault.modify()->TitleKey = IPMVShowHideDefaultTitle;
	Wt::Dbo::ptr<SingularKey> IPMVShowHideDefaultDetails = DboSession.add(new SingularKey("IPMVShowHideDefaultDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVShowHideDefaultDetails, "In this mode all languages except for the default language is always shown in the internal path. If the website is accessed without language in the internal path then language will automatically be added.")));
	IPMVShowHideDefault.modify()->DetailsKey = IPMVShowHideDefaultDetails;
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVNoRestrictionHideDefault = DboSession.add(new ConfigurationEnumValue());
	IPMVNoRestrictionHideDefault.modify()->Value = 3;
	Wt::Dbo::ptr<SingularKey> IPMVNoRestrictionHideDefaultTitle = DboSession.add(new SingularKey("IPMVNoRestrictionHideDefaultTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVNoRestrictionHideDefaultTitle, "No restrictions but hide default language")));
	IPMVNoRestrictionHideDefault.modify()->TitleKey = IPMVNoRestrictionHideDefaultTitle;
	Wt::Dbo::ptr<SingularKey> IPMVNoRestrictionHideDefaultDetails = DboSession.add(new SingularKey("IPMVNoRestrictionHideDefaultDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVNoRestrictionHideDefaultDetails, "In this mode language will not be automatically removed or added except the default language will never be shown.")));
	IPMVNoRestrictionHideDefault.modify()->DetailsKey = IPMVNoRestrictionHideDefaultDetails;
	Wt::Dbo::ptr<ConfigurationEnumValue> IPMVNoRestriction = DboSession.add(new ConfigurationEnumValue());
	IPMVNoRestriction.modify()->Value = 4;
	Wt::Dbo::ptr<SingularKey> IPMVNoRestrictionTitle = DboSession.add(new SingularKey("IPMVNoRestrictionTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVNoRestrictionTitle, "No restrictions")));
	IPMVNoRestriction.modify()->TitleKey = IPMVNoRestrictionTitle;
	Wt::Dbo::ptr<SingularKey> IPMVNoRestrictionDetails = DboSession.add(new SingularKey("IPMVNoRestrictionDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(IPMVNoRestrictionDetails, "In this mode language will not be automatically removed or added in any case.")));
	IPMVNoRestriction.modify()->DetailsKey = IPMVNoRestrictionDetails;

	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVShow);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVShowHideDefault);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVNoRestrictionHideDefault);
	InternalPathModeVal.modify()->EnumValueCollection.insert(IPMVNoRestriction);

	InternalPathModeVal.modify()->Value = IPMVShowHideDefault->Value;
	InternalPathModeVal.modify()->DefaultValue = IPMVShowHideDefault->Value;

	InternalPathMode.modify()->EnumPtr = InternalPathModeVal;

	//EnableTokens
	Wt::Dbo::ptr<Configuration> EnableTokens = DboSession.add(new Configuration("EnableTokens", O.AuthenticationModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> EnableTokensTitle = DboSession.add(new SingularKey("EnableTokensTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(EnableTokensTitle, "Enable Remember-Me Tokens")));
	EnableTokens.modify()->TitleKey = EnableTokensTitle;
	Wt::Dbo::ptr<SingularKey> EnableTokensDetails = DboSession.add(new SingularKey("EnableTokensDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(EnableTokensDetails, "If enabled then remember me feature before login will be enabled. This means that if users agree to keep them self logged in, they will not have to login again even after end of session. Cookies will be set as tokens.")));
	EnableTokens.modify()->DetailsKey = EnableTokensDetails;
	EnableTokens.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EnableTokensVal = DboSession.add(new ConfigurationBool());
	EnableTokensVal.modify()->Value = true;
	EnableTokensVal.modify()->DefaultValue = true;
	EnableTokensVal.modify()->RecommendedValue = true;
	EnableTokens.modify()->BoolPtr = EnableTokensVal;

	//LoginCookie
	Wt::Dbo::ptr<Configuration> LoginCookie = DboSession.add(new Configuration("LoginCookie", O.AuthenticationModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> LoginCookieTitle = DboSession.add(new SingularKey("LoginCookieTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LoginCookieTitle, "Remember-Me Cookie Name")));
	LoginCookie.modify()->TitleKey = LoginCookieTitle;
	Wt::Dbo::ptr<SingularKey> LoginCookieDetails = DboSession.add(new SingularKey("LoginCookieDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LoginCookieDetails, "Name of the cookie that will be used to set the remember-me token. Do not change this if you do not know what this means.")));
	LoginCookie.modify()->DetailsKey = LoginCookieDetails;
	Wt::Dbo::ptr<SingularKey> LoginCookieExpertWarning = DboSession.add(new SingularKey("LoginCookieExpertWarning", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(LoginCookieExpertWarning, "Changing cookie name to an invalid name may prevent the remember-me function to work properly.")));
	LoginCookie.modify()->ExpertWarningKey = LoginCookieExpertWarning;
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
	Wt::Dbo::ptr<SingularKey> EmailVerificationTitle = DboSession.add(new SingularKey("EmailVerificationTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(EmailVerificationTitle, "Enable Email Verification")));
	EmailVerification.modify()->TitleKey = EmailVerificationTitle;
	Wt::Dbo::ptr<SingularKey> EmailVerificationDetails = DboSession.add(new SingularKey("EmailVerificationDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(EmailVerificationDetails, "If enabled then all new users must verify their email address after registration. Verification is done by sending an automatically generated email asking to verify the email.")));
	EmailVerification.modify()->DetailsKey = EmailVerificationDetails;
	EmailVerification.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> EmailVerificationVal = DboSession.add(new ConfigurationBool());
	EmailVerificationVal.modify()->Value = false;
	EmailVerificationVal.modify()->DefaultValue = false;
	EmailVerification.modify()->BoolPtr = EmailVerificationVal;

	//FacebookOAuth
	Wt::Dbo::ptr<Configuration> FacebookOAuth = DboSession.add(new Configuration("FacebookOAuth", O.AuthenticationModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> FacebookOAuthTitle = DboSession.add(new SingularKey("FacebookOAuthTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookOAuthTitle, "Enable Facebook Login")));
	FacebookOAuth.modify()->TitleKey = FacebookOAuthTitle;
	Wt::Dbo::ptr<SingularKey> FacebookOAuthDetails = DboSession.add(new SingularKey("FacebookOAuthDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookOAuthDetails, "If enabled then users would be able to login from their facebook account. Note that facebook connect configurations also have to be changed.")));
	FacebookOAuth.modify()->DetailsKey = FacebookOAuthDetails;
	FacebookOAuth.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationBool> FacebookOAuthVal = DboSession.add(new ConfigurationBool());
	FacebookOAuthVal.modify()->Value = false;
	FacebookOAuthVal.modify()->DefaultValue = false;
	FacebookOAuth.modify()->BoolPtr = FacebookOAuthVal;

	//FacebookAppID
	Wt::Dbo::ptr<Configuration> FacebookAppID = DboSession.add(new Configuration("FacebookAppId", O.AuthenticationModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> FacebookAppIDTitle = DboSession.add(new SingularKey("FacebookAppIDTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookAppIDTitle, "OAuth 2 Facebook App ID")));
	FacebookAppID.modify()->TitleKey = FacebookAppIDTitle;
	Wt::Dbo::ptr<SingularKey> FacebookAppIDDetails = DboSession.add(new SingularKey("FacebookAppIDDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookAppIDDetails, "Facebook app ID that you get by registering on OAuth 2.")));
	FacebookAppID.modify()->DetailsKey = FacebookAppIDDetails;
	FacebookAppID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppIDVal = DboSession.add(new ConfigurationString());
	FacebookAppIDVal.modify()->ExampleValue = "1234567890123456";
	FacebookAppIDVal.modify()->MinLength = 1;
	FacebookAppID.modify()->StringPtr = FacebookAppIDVal;

	//FacebookAppSecret
	Wt::Dbo::ptr<Configuration> FacebookAppSecret = DboSession.add(new Configuration("FacebookAppSecret", O.AuthenticationModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> FacebookAppSecretTitle = DboSession.add(new SingularKey("FacebookAppSecretTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookAppSecretTitle, "OAuth 2 Facebook App Secret")));
	FacebookAppSecret.modify()->TitleKey = FacebookAppSecretTitle;
	Wt::Dbo::ptr<SingularKey> FacebookAppSecretDetails = DboSession.add(new SingularKey("FacebookAppSecretDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(FacebookAppSecretDetails, "Facebook app secret that you get by registering on OAuth 2.")));
	FacebookAppSecret.modify()->DetailsKey = FacebookAppSecretDetails;
	FacebookAppSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> FacebookAppSecretVal = DboSession.add(new ConfigurationString());
	FacebookAppSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	FacebookAppSecretVal.modify()->MinLength = 1;
	FacebookAppSecret.modify()->StringPtr = FacebookAppSecretVal;

	//GoogleOAuth
	Wt::Dbo::ptr<Configuration> GoogleOAuth = DboSession.add(new Configuration("GoogleOAuth", O.AuthenticationModule, Configuration::Bool));
	Wt::Dbo::ptr<SingularKey> GoogleOAuthTitle = DboSession.add(new SingularKey("GoogleOAuthTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleOAuthTitle, "Enable Google Login")));
	GoogleOAuth.modify()->TitleKey = GoogleOAuthTitle;
	Wt::Dbo::ptr<SingularKey> GoogleOAuthDetails = DboSession.add(new SingularKey("GoogleOAuthDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleOAuthDetails, "If enabled then users would be able to login from their google account. Note that google api configurations also have to be changed.")));
	GoogleOAuth.modify()->DetailsKey = GoogleOAuthDetails;
	GoogleOAuth.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationBool> GoogleOAuthVal = DboSession.add(new ConfigurationBool());
	GoogleOAuthVal.modify()->Value = false;
	GoogleOAuthVal.modify()->DefaultValue = false;
	GoogleOAuth.modify()->BoolPtr = GoogleOAuthVal;

	//GoogleClientID
	Wt::Dbo::ptr<Configuration> GoogleClientID = DboSession.add(new Configuration("GoogleClientId", O.AuthenticationModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> GoogleClientIDTitle = DboSession.add(new SingularKey("GoogleClientIDTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleClientIDTitle, "OAuth 2 Google Client ID")));
	GoogleClientID.modify()->TitleKey = GoogleClientIDTitle;
	Wt::Dbo::ptr<SingularKey> GoogleClientIDDetails = DboSession.add(new SingularKey("GoogleClientIDDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleClientIDDetails, "Google client ID that you get by registering on OAuth 2.")));
	GoogleClientID.modify()->DetailsKey = GoogleClientIDDetails;
	GoogleClientID.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientIDVal = DboSession.add(new ConfigurationString());
	GoogleClientIDVal.modify()->ExampleValue = "1234567890123456";
	GoogleClientIDVal.modify()->MinLength = 1;
	GoogleClientID.modify()->StringPtr = GoogleClientIDVal;

	//GoogleClientSecret
	Wt::Dbo::ptr<Configuration> GoogleClientSecret = DboSession.add(new Configuration("GoogleClientSecret", O.AuthenticationModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> GoogleClientSecretTitle = DboSession.add(new SingularKey("GoogleClientSecretTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleClientSecretTitle, "OAuth 2 Google Client Secret")));
	GoogleClientSecret.modify()->TitleKey = GoogleClientSecretTitle;
	Wt::Dbo::ptr<SingularKey> GoogleClientSecretDetails = DboSession.add(new SingularKey("GoogleClientSecretDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(GoogleClientSecretDetails, "Google client secret that you get by registering on OAuth 2.")));
	GoogleClientSecret.modify()->DetailsKey = GoogleClientSecretDetails;
	GoogleClientSecret.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> GoogleClientSecretVal = DboSession.add(new ConfigurationString());
	GoogleClientSecretVal.modify()->ExampleValue = "a3cf1630b1ae415c7260d849efdf444d";
	GoogleClientSecretVal.modify()->MinLength = 1;
	GoogleClientSecret.modify()->StringPtr = GoogleClientSecretVal;

	//ResourcesURL
	Wt::Dbo::ptr<Configuration> ResourcesURL = DboSession.add(new Configuration("ResourcesURL", O.StylesModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> ResourcesURLTitle = DboSession.add(new SingularKey("ResourcesURLTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(ResourcesURLTitle, "Resources folder URL")));
	ResourcesURL.modify()->TitleKey = ResourcesURLTitle;
	Wt::Dbo::ptr<SingularKey> ResourcesURLDetails = DboSession.add(new SingularKey("ResourcesURLDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(ResourcesURLDetails, "Path to Wt's resources folder.")));
	ResourcesURL.modify()->DetailsKey = ResourcesURLDetails;
	ResourcesURL.modify()->RestartRequired = true;
	Wt::Dbo::ptr<ConfigurationString> ResourcesURLVal = DboSession.add(new ConfigurationString());
	ResourcesURLVal.modify()->Value = "/resources";
	ResourcesURLVal.modify()->DefaultValue = "/resources";
	ResourcesURLVal.modify()->ExampleValue = "/resources";
	ResourcesURLVal.modify()->MinLength = 1;
	ResourcesURL.modify()->StringPtr = ResourcesURLVal;

	//StylesURL
	Wt::Dbo::ptr<Configuration> StylesURL = DboSession.add(new Configuration("StylesURL", O.StylesModule, Configuration::String));
	Wt::Dbo::ptr<SingularKey> StylesURLTitle = DboSession.add(new SingularKey("StylesURLTitle", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(StylesURLTitle, "Images folder URL")));
	StylesURL.modify()->TitleKey = StylesURLTitle;
	Wt::Dbo::ptr<SingularKey> StylesURLDetails = DboSession.add(new SingularKey("StylesURLDetails", O.ConfigurationModule));
	O.EnglishLanguagePtr.modify()->SingularStringCollection.insert(DboSession.add(new SingularString(StylesURLDetails, "Path to the images folder that contain general and style images.")));
	StylesURL.modify()->DetailsKey = StylesURLDetails;
	StylesURL.modify()->RestartRequired = false;
	Wt::Dbo::ptr<ConfigurationString> StylesURLVal = DboSession.add(new ConfigurationString());
	StylesURLVal.modify()->Value = "/styles";
	StylesURLVal.modify()->DefaultValue = "/styles";
	StylesURLVal.modify()->ExampleValue = "/styles";
	StylesURLVal.modify()->MinLength = 1;
	StylesURL.modify()->StringPtr = StylesURLVal;
}
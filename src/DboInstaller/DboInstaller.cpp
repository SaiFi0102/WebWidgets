#include "DboInstaller.h"
#include <Dbo/UserTreeDbos.h>

namespace WW
{
	namespace Installer
	{

DboInstaller::DboInstaller(Wt::Dbo::SqlConnectionPool &SQLPool)
{
	dboSession.setConnectionPool(SQLPool);
	mapClasses();
}

DboInstaller::DboInstaller(Wt::Dbo::SqlConnection &SQLConnection)
{
	dboSession.setConnection(SQLConnection);
	mapClasses();
}

void DboInstaller::mapClasses()
{
	MAP_MODULE_DBO_TREE(dboSession)
 	MAP_USER_DBO_TREE(dboSession)
}

void DboInstaller::createTables()
{
	Wt::Dbo::Transaction transaction(dboSession);
	dboSession.createTables();

	//UNIQUE
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_accept\" ON \"") + Dbo::Language::tableName() + "\" (\"LanguageAccept\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_page_host_path\" ON \"") + Dbo::PageAccessPath::tableName() + "\" (\"Access_HostName\", \"InternalPath\", \"Parent_AccessPath_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_host_path\" ON \"") + Dbo::LanguageAccessPath::tableName() + "\" (\"Access_HostName\", \"InternalPath\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_configuration\" ON \"") + Dbo::Configuration::tableName() + "\" (\"Name\", \"Module_id\", \"Type\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_singular_key\" ON \"") + Dbo::SingularKey::tableName() + "\" (\"Key\", \"Module_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_singular_string\" ON \"") + Dbo::SingularString::tableName() + "\" (\"Language_Code\", \"Key_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_plural_key\" ON \"") + Dbo::PluralKey::tableName() + "\" (\"Key\", \"Module_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_plural_string\" ON \"") + Dbo::PluralString::tableName() + "\" (\"Language_Code\", \"Key_id\", \"Case\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_page\" ON \"") + Dbo::Page::tableName() + "\" (\"Name\", \"Module_id\")");
// 	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_order\" ON \"") + Dbo::NavigationMenuItem::TableName() + "\" (\"Order\", \"Parent_Item_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_style\" ON \"") + Dbo::Style::tableName() + "\" (\"StyleName\", \"Author_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_template\" ON \"") + Dbo::Template::tableName() + "\" (\"TemplateName\", \"Module_id\")");
	dboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_style_template\" ON \"") + Dbo::StyleTemplate::tableName() + "\" (\"Style_id\", \"Template_id\")");

	//INDEX
	dboSession.execute(std::string("CREATE INDEX \"idx_installed\" ON \"") + Dbo::Language::tableName() + "\" (\"Installed\")");
	
	transaction.commit();
}

void DboInstaller::insertRows()
{
	Wt::Dbo::Transaction transaction(dboSession);

	//Authors and Modules
	insertAuthorModules();

	//Localization
	insertLanguages();

	//Configurations
	insertConfigurations();

	//Default style
	auto defaultStyle = new Dbo::Style("Default", O.SaifAuthor);
	defaultStyle->description = "Default style";
	defaultStyle->compatibilityVersionSeries = 0;
	defaultStyle->compatibilityVersionMajor = 0;
	O.DefaultStyle = dboSession.add(defaultStyle);

	//Pages
	auto homePage = new Dbo::Page("home", O.NavigationModule);
	homePage->titleKey = addSingularKeyString(O.EnglishLanguage, "HomePageTitle", O.NavigationModule, "Home");
	homePage->defaultInternalPath = "home";
	auto homePagePtr = dboSession.add(homePage);

	auto sitemapPage = new Dbo::Page("sitemap", O.NavigationModule);
	sitemapPage->titleKey = addSingularKeyString(O.EnglishLanguage, "SitemapPageTitle", O.NavigationModule, "Site Map");
	sitemapPage->defaultInternalPath = "sitemap";
	auto sitemapPagePtr = dboSession.add(sitemapPage);

	auto errorPage = new Dbo::Page("error", O.NavigationModule);
	errorPage->titleKey = addSingularKeyString(O.EnglishLanguage, "ErrorPageTitle", O.NavigationModule, "An error occurred");
	auto errorPagePtr = dboSession.add(errorPage);

	auto notFoundPage = new Dbo::Page("notfound", O.NavigationModule);
	notFoundPage->titleKey = addSingularKeyString(O.EnglishLanguage, "NotFoundPageTitle", O.NavigationModule, "Page not found");
	auto notFoundPagePtr = dboSession.add(notFoundPage);

	auto loginPage = new Dbo::Page("login", O.AuthenticationModule);
	loginPage->titleKey = addSingularKeyString(O.EnglishLanguage, "LoginPageTitle", O.AuthenticationModule, "Login");
	loginPage->defaultInternalPath = "login";
	loginPage->allowOnDisabledLogin = true;
	loginPage->authRestriction = Dbo::Page::AuthUnrestricted;
	auto loginPagePtr = dboSession.add(loginPage);

	auto registerPage = new Dbo::Page("register", O.AuthenticationModule);
	registerPage->titleKey = addSingularKeyString(O.EnglishLanguage, "RegisterPageTitle", O.AuthenticationModule, "Register");
	registerPage->defaultInternalPath = "register";
	registerPage->authRestriction = Dbo::Page::AuthLoggedOut;
	auto registerPagePtr = dboSession.add(registerPage);

	auto disabledPage = new Dbo::Page("disabled", O.AuthenticationModule);
	disabledPage->titleKey = addSingularKeyString(O.EnglishLanguage, "DisabledPageTitle", O.AuthenticationModule, "Disabled");
	auto disabledPagePtr = dboSession.add(disabledPage);

	//Global Access HostName
	auto globalAccessHost = new Dbo::AccessHostName("");
	globalAccessHost->languagePtr = O.EnglishLanguage;
	globalAccessHost->defaultPagePtr = homePagePtr;
	globalAccessHost->stylePtr = O.DefaultStyle;
	globalAccessHost->mobileInternalPath = "mobile";
	O.GlobalAccessHost = dboSession.add(globalAccessHost);

	//English Access Path
	auto englishAccessPath = new Dbo::LanguageAccessPath(O.GlobalAccessHost, "en");
	englishAccessPath->languagePtr = O.EnglishLanguage;
	O.EnglishAccessPath = dboSession.add(englishAccessPath);

	//Page Access Paths
	auto homeAccessPath = new Dbo::PageAccessPath(O.GlobalAccessHost, "home");
	homeAccessPath->pagePtr = homePagePtr;
	O.HomePageAccessPath = dboSession.add(homeAccessPath);

	auto sitemapAccessPath = new Dbo::PageAccessPath(O.GlobalAccessHost, "sitemap");
	sitemapAccessPath->pagePtr = sitemapPagePtr;
	dboSession.add(sitemapAccessPath);

	auto loginAccessPath = new Dbo::PageAccessPath(O.GlobalAccessHost, "login");
	loginAccessPath->pagePtr = loginPagePtr;
	auto loginAccessPathPtr = dboSession.add(loginAccessPath);

	auto registerAccessPath = new Dbo::PageAccessPath(O.GlobalAccessHost, "register");
	registerAccessPath->pagePtr = registerPagePtr;
	auto registerAccessPathPtr = dboSession.add(registerAccessPath);

	//Menus
	O.MainMenu = dboSession.add(new Dbo::NavigationMenu());

	auto homeMenuItem = new Dbo::NavigationMenuItem(O.MainMenu);
	homeMenuItem->labelKey = addSingularKeyString(O.EnglishLanguage, "HomeMenuItemLabel", O.NavigationModule, "Home");
	homeMenuItem->order = 0;
	homeMenuItem->accessPathPtr = O.HomePageAccessPath;
	dboSession.add(homeMenuItem);

	auto loginMenuItem = new Dbo::NavigationMenuItem(O.MainMenu);
	loginMenuItem->labelKey = addSingularKeyString(O.EnglishLanguage, "LoginMenuItemLabel", O.AuthenticationModule, "Login");
	loginMenuItem->order = 1;
	loginMenuItem->accessPathPtr = loginAccessPathPtr;
	loginMenuItem->loggedInVisibility = Dbo::NavigationMenuItem::LoggedOutOnly;
	dboSession.add(loginMenuItem);

	auto registerMenuItem = new Dbo::NavigationMenuItem(O.MainMenu);
	registerMenuItem->labelKey = addSingularKeyString(O.EnglishLanguage, "RegisterMenuItemLabel", O.AuthenticationModule, "Register");
	registerMenuItem->order = 2;
	registerMenuItem->accessPathPtr = registerAccessPathPtr;
	registerMenuItem->loggedInVisibility = Dbo::NavigationMenuItem::LoggedOutOnly;
	dboSession.add(registerMenuItem);

	//Templates
	auto mainTemplate = new Dbo::Template("main", O.StylingModule);
	mainTemplate->description = "Main container template";
	mainTemplate->templateStr = "${navigation}${page-content}${test-page}";
	O.MainTemplate = dboSession.add(mainTemplate);

	auto registerTemplate = new Dbo::Template("registration", O.AuthenticationModule);
	mainTemplate->description = "Registration widget template";
	registerTemplate->templateStr = "<div class=\"Wt-form Wt-auth-registration\">\r\n  <h2>${tr:Wt.Auth.registration-form-title}</h2>\r\n  <p>${tr:Wt.Auth.registration-form-info}</p>\r\n\r\n  ${password-description}\r\n\r\n  <div class=\"Wt-fields\">\r\n	${<if:user-name>}\r\n  <label for=\"${id:user-name}\">${user-name-label}\r\n	${user-name-info class=\"Wt-info\"}\r\n	${confirm-is-you class=\"Wt-info\"}\r\n  </label>\r\n  ${user-name}\r\n${</if:user-name>}\r\n\r\n${block:Wt.Auth.field choose-password}\r\n${block:Wt.Auth.field repeat-password}\r\n${block:Wt.Auth.field email}\r\n  </div>\r\n\r\n  ${<if:oauth>}\r\n	<br clear=\"both\" />\r\n${oauth-description}\r\n<label>${tr:Wt.Auth.oauth}\r\n  <span class=\"Wt-info\">${tr:Wt.Auth.oauth-info}</span>\r\n</label>\r\n${icons}\r\n  ${</if:oauth>}\r\n\r\n  <div class=\"Wt-buttons\">${ok-button}</div>\r\n</div>";
	dboSession.add(registerTemplate);

	insertTestData();
	transaction.commit();
}

void DboInstaller::insertTestData()
{
	auto testStyle = new Dbo::Style("Test", O.SaifAuthor);
	testStyle->description = "Test style";
	testStyle->compatibilityVersionSeries = 0;
	testStyle->compatibilityVersionMajor = 0;
	auto testStylePtr = dboSession.add(testStyle);

	auto templateTplTemplate = new Dbo::Template("templatetpl", O.StylingModule);
	templateTplTemplate->templateStr = "templatetpl";
	auto templateTplTemplatePtr = dboSession.add(templateTplTemplate);

	auto styleTplTemplate = new Dbo::Template("styletpl", O.StylingModule);
	styleTplTemplate->templateStr = "styletpl(no style)";
	auto styleTplTemplatePtr = dboSession.add(styleTplTemplate);

	auto defaultStyleTpl = new Dbo::StyleTemplate(O.DefaultStyle, styleTplTemplatePtr);
	defaultStyleTpl->templateStr = "styletpl(Default style)";
	dboSession.add(defaultStyleTpl);

	auto testStyleTpl = new Dbo::StyleTemplate(testStylePtr, styleTplTemplatePtr);
	testStyleTpl->templateStr = "styletpl(Test style)";
	dboSession.add(testStyleTpl);

	auto defaultStyleCssRule = new Dbo::StyleCssRule(O.DefaultStyle);
	defaultStyleCssRule->selector = "body";
	defaultStyleCssRule->declarations = "background:#ffffff;color:#000000;";
	dboSession.add(defaultStyleCssRule);

	auto testStyleCssRule = new Dbo::StyleCssRule(testStylePtr);
	testStyleCssRule->selector = "body";
	testStyleCssRule->declarations = "background:#000000;color:#ffffff;";
	dboSession.add(testStyleCssRule);

	O.FrenchLanguage.modify()->installed = true;
	auto frenchAccessPath = new Dbo::LanguageAccessPath(O.GlobalAccessHost, "fr");
	frenchAccessPath->languagePtr = O.FrenchLanguage;
	dboSession.add(frenchAccessPath);

	addSingularKeyString(O.FrenchLanguage, "HomeMenuItemLabel", O.NavigationModule, "Accueil");
	addSingularKeyString(O.FrenchLanguage, "LoginMenuItemLabel", O.AuthenticationModule, "S'identifier");
	addSingularKeyString(O.FrenchLanguage, "RegisterMenuItemLabel", O.AuthenticationModule, "Enregistrez");
}

void DboInstaller::dropTables()
{
	dboSession.dropTables();
}

void DboInstaller::install()
{
	createTables();
	insertRows();
}

void DboInstaller::reinstall()
{
	dropTables();
	install();
}

	}
}

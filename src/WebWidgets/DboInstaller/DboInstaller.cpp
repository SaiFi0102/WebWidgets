#include "DboInstaller/DboInstaller.h"

DboInstaller::DboInstaller(Wt::Dbo::SqlConnectionPool &SQLPool)
{
	DboSession.setConnectionPool(SQLPool);
	MapClasses();
}

DboInstaller::DboInstaller(Wt::Dbo::SqlConnection &SQLConnection)
{
	DboSession.setConnection(SQLConnection);
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

	//UNIQUE
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_accept\" ON \"") + Language::TableName() + "\" (\"LanguageAccept\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_url\" ON \"") + AccessPath::TableName() + "\" (\"HostName\", \"InternalPath\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_configuration\" ON \"") + Configuration::TableName() + "\" (\"Name\", \"Module_id\", \"Type\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_singular\" ON \"") + LanguageSingle::TableName() + "\" (\"Language_Code\", \"Key\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_plural\" ON \"") + LanguagePlural::TableName() + "\" (\"Language_Code\", \"Key\", \"Case\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_page\" ON \"") + Page::TableName() + "\" (\"Name\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_style\" ON \"") + Style::TableName() + "\" (\"StyleName\", \"Author_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_template\" ON \"") + Template::TableName() + "\" (\"TemplateName\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_style_template\" ON \"") + StyleTemplate::TableName() + "\" (\"Style_id\", \"Template_id\")");

	//INDEX
	DboSession.execute(std::string("CREATE INDEX \"idx_installed\" ON \"") + Language::TableName() + "\" (\"Installed\")");
	
	transaction.commit();
}

void DboInstaller::InsertRows()
{
	Wt::Dbo::Transaction transaction(DboSession);

	//Authors and Modules
	InsertAuthorModules();

	//Configurations
	InsertConfigurations();

	//Localization
	InsertLanguages();

	//Pages
	O.LandingHomePage = DboSession.add(new Page("home", O.NavigationModule));
	O.LandingHomePage.modify()->Title = "Home";
	O.LandingHomePage.modify()->DefaultInternalPath = "home";

	O.SitemapPage = DboSession.add(new Page("sitemap", O.NavigationModule));
	O.SitemapPage.modify()->Title = "Site map";
	O.SitemapPage.modify()->DefaultInternalPath = "sitemap";

	//Access Paths
	//English language
	O.DefaultLanguageAccessPath = DboSession.add(new AccessPath());
	O.DefaultLanguageAccessPath.modify()->HostName = "";
	O.DefaultLanguageAccessPath.modify()->InternalPath = "en";
	O.DefaultLanguageAccessPath.modify()->LanguagePtr = O.EnglishLanguagePtr;

	//Mobile version
	O.MobileVersionAccessPath = DboSession.add(new AccessPath());
	O.MobileVersionAccessPath.modify()->HostName = "";
	O.MobileVersionAccessPath.modify()->InternalPath = "mobile";

	//Home page
	O.HomePageAccessPath = DboSession.add(new AccessPath());
	O.HomePageAccessPath.modify()->HostName = "";
	O.HomePageAccessPath.modify()->InternalPath = "home";
	O.HomePageAccessPath.modify()->PagePtr = O.LandingHomePage;

	//Sitemap page
	Wt::Dbo::ptr<AccessPath> SitemapPageAccessPath = DboSession.add(new AccessPath());
	SitemapPageAccessPath.modify()->HostName = "";
	SitemapPageAccessPath.modify()->InternalPath = "sitemap";
	SitemapPageAccessPath.modify()->PagePtr = O.SitemapPage;

	//Default style
	O.DefaultStyle = DboSession.add(new Style("Default", O.SaifAuthor));
	O.DefaultStyle.modify()->Description = "Default style";
	O.DefaultStyle.modify()->CompatibilityVersionSeries = 1;
	O.DefaultStyle.modify()->CompatibilityVersionMajor = 0;

	transaction.commit();

	//Set IDs after they have been assigned in the database
	Wt::Dbo::Transaction transaction2(DboSession);

	O.DefaultAccessPathVal.modify()->Value = O.DefaultLanguageAccessPath.id();
	O.HomePageAccessPathIdVal.modify()->Value = O.HomePageAccessPath.id();
	O.MobileAccessPathIdVal.modify()->Value = O.MobileVersionAccessPath.id();

	O.DefaultStyleNameVal.modify()->Value = O.DefaultStyle->Name();
	O.DefaultStyleNameVal.modify()->DefaultValue = O.DefaultStyle->Name();
	O.DefaultStyleAuthorVal.modify()->Value = O.DefaultStyle->AuthorPtr().id();
	O.DefaultStyleAuthorVal.modify()->DefaultValue = O.DefaultStyle->AuthorPtr().id();

	transaction2.commit();
}

void DboInstaller::DropTables()
{
	DboSession.dropTables();
}

void DboInstaller::Install()
{
	CreateTables();
	InsertRows();
}

void DboInstaller::Reinstall()
{
	DropTables();
	Install();
}

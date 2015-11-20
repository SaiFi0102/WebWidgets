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
	MAPDBOCASSES(DboSession)
}

void DboInstaller::CreateTables()
{
	Wt::Dbo::Transaction transaction(DboSession);
	DboSession.createTables();

	//UNIQUE
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_accept\" ON \"") + Language::TableName() + "\" (\"LanguageAccept\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_page_host_path\" ON \"") + PageAccessPath::TableName() + "\" (\"Access_HostName\", \"InternalPath\", \"Parent_AccessPath_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_language_host_path\" ON \"") + LanguageAccessPath::TableName() + "\" (\"Access_HostName\", \"InternalPath\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_configuration\" ON \"") + Configuration::TableName() + "\" (\"Name\", \"Module_id\", \"Type\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_singular_key\" ON \"") + SingularKey::TableName() + "\" (\"Key\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_singular_string\" ON \"") + SingularString::TableName() + "\" (\"Language_Code\", \"Key_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_plural_key\" ON \"") + PluralKey::TableName() + "\" (\"Key\", \"Module_id\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_plural_string\" ON \"") + PluralString::TableName() + "\" (\"Language_Code\", \"Key_id\", \"Case\")");
	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_page\" ON \"") + Page::TableName() + "\" (\"Name\", \"Module_id\")");
// 	DboSession.execute(std::string("CREATE UNIQUE INDEX \"unique_order\" ON \"") + NavigationMenuItem::TableName() + "\" (\"Order\", \"Parent_Item_id\")");
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

	//Localization
	InsertLanguages();

	//Configurations
	InsertConfigurations();

	//Pages
	O.LandingHomePage = DboSession.add(new Page("home", O.NavigationModule));
	O.LandingHomePage.modify()->TitleKey = O.HomePageTitle;
	O.LandingHomePage.modify()->DefaultInternalPath = "home";

	O.SitemapPage = DboSession.add(new Page("sitemap", O.NavigationModule));
	O.SitemapPage.modify()->TitleKey = O.SiteMapPageTitle;
	O.SitemapPage.modify()->DefaultInternalPath = "sitemap";

	//Default style
	O.DefaultStyle = DboSession.add(new Style("Default", O.SaifAuthor));
	O.DefaultStyle.modify()->Description = "Default style";
	O.DefaultStyle.modify()->CompatibilityVersionSeries = 1;
	O.DefaultStyle.modify()->CompatibilityVersionMajor = 0;

	//Global Access HostName
	O.GlobalAccessHost = DboSession.add(new AccessHostName(""));
	O.GlobalAccessHost.modify()->LanguagePtr = O.EnglishLanguagePtr;
	O.GlobalAccessHost.modify()->DefaultPagePtr = O.LandingHomePage;
	O.GlobalAccessHost.modify()->StylePtr = O.DefaultStyle;
	O.GlobalAccessHost.modify()->MobileInternalPath = "mobile";

	//English Access Path
	O.EnglishAccessPath = DboSession.add(new LanguageAccessPath(O.GlobalAccessHost, "en"));
	O.EnglishAccessPath.modify()->LanguagePtr = O.EnglishLanguagePtr;

	//Home Page Access Path
	O.HomePageAccessPath = DboSession.add(new PageAccessPath(O.GlobalAccessHost, "home"));
	O.HomePageAccessPath.modify()->PagePtr = O.LandingHomePage;

	//Sitemap Page Access Path
	Wt::Dbo::ptr<PageAccessPath> SitemapPageAccessPath = DboSession.add(new PageAccessPath(O.GlobalAccessHost, "sitemap"));
	SitemapPageAccessPath.modify()->PagePtr = O.SitemapPage;

	transaction.commit();
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

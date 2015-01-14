#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "Application/WServer.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::unique_lock<boost::shared_mutex> lock(mutex)

PagesDatabase::PagesDatabase(DboDatabaseManager *Manager)
	: AbstractDboDatabase(Manager)
{ }

void PagesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	PageContainers pagecontainer;
	pagecontainer.swap(PageContainer);

	//Strong transaction like exception safety
	try
	{
		Wt::Dbo::Transaction transaction(DboSession);
		PageCollections PageCollection = DboSession.find<Page>();

		//All Modules
		for(PageCollections::const_iterator itr = PageCollection.begin();
			itr != PageCollection.end();
			++itr)
		{
			PageContainer.insert(MetaPage(boost::shared_ptr<PageData>(new PageData(*itr))));
		}

		transaction.commit();
	}
	catch(...)
	{
		PageContainer.swap(pagecontainer);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	lock.unlock();
	Wt::log("info") << Name() << ": " << CountPages() << " Page entires successfully loaded in " << GetLoadDurationinMS() << " ms";
}

boost::shared_ptr<const PageData> PagesDatabase::GetPtr(long long PageId) const
{
	READ_LOCK;
	PageById::const_iterator itr = PageContainer.get<ById>().find(PageId);
	if(itr == PageContainer.get<ById>().end())
	{
		return boost::shared_ptr<const PageData>();
	}
	return itr->PagePtr;
}
boost::shared_ptr<const PageData> PagesDatabase::GetPtr(const std::string &PageName, long long ModuleId) const
{
	READ_LOCK;
	PageByKey::const_iterator itr = PageContainer.get<ByKey>().find(boost::make_tuple(PageName, ModuleId));
	if(itr == PageContainer.get<ByKey>().end())
	{
		return boost::shared_ptr<const PageData>();
	}
	return itr->PagePtr;
}

boost::shared_ptr<const PageData> PagesDatabase::HomePagePtr(const std::string &HostName) const
{
	boost::shared_ptr<const AccessHostNameData> AccessHostNamePtr = Server()->AccessPaths()->AccessHostOrGlobalPtr(HostName);
	if(AccessHostNamePtr && AccessHostNamePtr->DefaultPageId != -1)
	{
		return GetPtr(AccessHostNamePtr->DefaultPageId);
	}
	return boost::shared_ptr<const PageData>();
}


std::size_t PagesDatabase::CountPages() const
{
	READ_LOCK;
	return PageContainer.size();
}
long long PagesDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}

void PagesDatabase::Load(Wt::Dbo::Session &DboSession)
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
	DboSession.mapClass<AccessHostName>(AccessHostName::TableName());
	DboSession.mapClass<PageAccessPath>(PageAccessPath::TableName());
	DboSession.mapClass<LanguageAccessPath>(LanguageAccessPath::TableName());

	FetchAll(DboSession);
}
void PagesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
	Server()->RefreshPageStrings();
}

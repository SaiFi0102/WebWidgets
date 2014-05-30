#include "DboDatabase/PagesDatabase.h"
#include "Application/WServer.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

PagesDatabase::PagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnectionPool(SQLPool);
	MapClasses();
}

PagesDatabase::PagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnection(SQLConnection);
	MapClasses();
}

void PagesDatabase::MapClasses()
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

void PagesDatabase::FetchAll()
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	PageContainers pagecontainer;
	boost::multi_index::swap(pagecontainer, PageContainer);

	//Strong transaction like exception safety
	try
	{
		DboSession.disconnectAll();

		Wt::Dbo::Transaction transaction(DboSession);
		PageCollections PageCollection = DboSession.find<Page>();

		//All Modules
		for(PageCollections::const_iterator itr = PageCollection.begin();
			itr != PageCollection.end();
			++itr)
		{
			PageContainer.get<0>().insert(*itr);
		}

		transaction.commit();
	}
	catch(...)
	{
		boost::multi_index::swap(PageContainer, pagecontainer);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;
}

Wt::Dbo::ptr<Page> PagesDatabase::GetPtr(long long PageId, long long ModuleId) const
{
	READ_LOCK;
	PageByCompositeKey::iterator itr = PageContainer.get<ByCompositeKey>().find(boost::make_tuple(PageId, ModuleId));
	if(itr == PageContainer.get<ByCompositeKey>().end())
	{
		return Wt::Dbo::ptr<Page>();
	}
	return *itr;
}
/*Wt::Dbo::ptr<Page> PagesDatabase::GetPtr(const std::string &InternalPath) const
{
	READ_LOCK;
	PageByInternalPath::iterator itr = PageContainer.get<ByInternalPath>().find(InternalPath);
	if(itr == PageContainer.get<ByInternalPath>().end())
	{
		return Wt::Dbo::ptr<Page>();
	}
	return *itr;
}*/

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

void PagesDatabase::Load()
{
	FetchAll();
}

void PagesDatabase::Reload()
{
	FetchAll();
	_Server.RefreshPageStrings();
}

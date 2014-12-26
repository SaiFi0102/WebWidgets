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
	PageMaps pagemap;
	pagemap.swap(PageMap);

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
			PageMap[std::make_pair((*itr)->id(), (*itr)->ModulePtr().id())] = MetaPage(boost::shared_ptr<PageData>(new PageData(*itr)));
		}

		transaction.commit();
	}
	catch(...)
	{
		PageMap.swap(pagemap);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	lock.unlock();
	Wt::log("info") << Name() << ": " << CountPages() << " Page entires successfully loaded in " << GetLoadDurationinMS() << " ms";
}

boost::shared_ptr<PageData> PagesDatabase::GetPtr(long long PageId, long long ModuleId) const
{
	READ_LOCK;
	PageMaps::const_iterator itr = PageMap.find(std::make_pair(PageId, ModuleId));
	if(itr == PageMap.end())
	{
		return boost::shared_ptr<PageData>();
	}
	return itr->second.PagePtr;
}

boost::shared_ptr<PageData> PagesDatabase::HomePagePtr() const
{
	boost::shared_ptr<AccessPathData> HomePageAccessPath = Server()->AccessPaths()->HomePageAccessPathPtr();
	if(!HomePageAccessPath)
	{
		return boost::shared_ptr<PageData>();
	}
	return GetPtr(HomePageAccessPath->PageId, HomePageAccessPath->PageModuleId);
}

/*boost::shared_ptr<PageData> PagesDatabase::GetPtr(const std::string &InternalPath) const
{
	ReadLock lock(Manager);
	PageByInternalPath::iterator itr = PageContainer.get<ByInternalPath>().find(InternalPath);
	if(itr == PageContainer.get<ByInternalPath>().end())
	{
		return boost::shared_ptr<PageData>();
	}
	return *itr;
}*/

void PagesDatabase::RegisterPageHandler(long long PageId, long long ModuleId, boost::function<void()> Handler)
{
	WRITE_LOCK;
	PageMap[std::make_pair(PageId, ModuleId)].HandlerFunction = Handler;
}

bool PagesDatabase::CallPageHandler(long long PageId, long long ModuleId)
{
	READ_LOCK;
	PageMaps::const_iterator itr = PageMap.find(std::make_pair(PageId, ModuleId));
	if(itr == PageMap.end())
	{
		return false;
	}
	if(itr->second.HandlerFunction.empty())
	{
		return false;
	}

	itr->second.HandlerFunction();
	return true;
}

std::size_t PagesDatabase::CountPages() const
{
	READ_LOCK;
	return PageMap.size();
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
	DboSession.mapClass<AccessPath>(AccessPath::TableName());

	FetchAll(DboSession);
}
void PagesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
	Server()->RefreshPageStrings();
}

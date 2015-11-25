#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"
#include "Pages/AbstractPage.h"

PagesDatabase::PagesDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager), Page404Handler(0)
{ }

PagesDatabase::~PagesDatabase()
{
	for(const auto &page : PageContainer)
	{
		delete page.PageHandler;
	}
	delete Page404Handler;
}

void PagesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	PageContainers pagecontainer;

	Wt::Dbo::Transaction transaction(DboSession);
	PageCollections PageCollection = DboSession.find<Page>();

	for(PageCollections::const_iterator itr = PageCollection.begin();
		itr != PageCollection.end();
		++itr)
	{
		PageById::iterator i = PageContainer.find(itr->id());
		if(i != PageContainer.end())
		{
			pagecontainer.insert(MetaPage(std::shared_ptr<PageData>(new PageData(*itr)), i->PageHandler));
		}
		else
		{
			pagecontainer.insert(MetaPage(std::shared_ptr<PageData>(new PageData(*itr))));
		}
	}

	//Delete if any AbstractPage left
	for(PageById::iterator itr = PageContainer.begin();
		itr != PageContainer.end();
		++itr)
	{
		PageById::iterator i = pagecontainer.find(itr->PagePtr->id());
		if(i == pagecontainer.end())
		{
			delete itr->PageHandler;
		}
	}

	transaction.commit();
	PageContainer.swap(pagecontainer);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << PageContainer.size() << " Page entires successfully loaded in "
		<< LoadDuration.total_milliseconds() << " ms";
}

std::shared_ptr<const PageData> PagesDatabase::GetPtr(long long PageId) const
{
	ReadLock lock(Manager());
	PageById::const_iterator itr = PageContainer.get<ById>().find(PageId);
	if(itr == PageContainer.get<ById>().end())
	{
		return std::shared_ptr<const PageData>();
	}
	return itr->PagePtr;
}
std::shared_ptr<const PageData> PagesDatabase::GetPtr(const std::string &PageName, long long ModuleId) const
{
	ReadLock lock(Manager());
	PageByKey::const_iterator itr = PageContainer.get<ByKey>().find(boost::make_tuple(PageName, ModuleId));
	if(itr == PageContainer.get<ByKey>().end())
	{
		return std::shared_ptr<const PageData>();
	}
	return itr->PagePtr;
}

AbstractPage *PagesDatabase::GetPage(long long PageId) const
{
	ReadLock lock(Manager());
	PageById::const_iterator itr = PageContainer.get<ById>().find(PageId);
	if(itr == PageContainer.get<ById>().end())
	{
		return 0;
	}
	return itr->PageHandler;
}

AbstractPage *PagesDatabase::GetPage(const std::string &PageName, long long ModuleId) const
{
	ReadLock lock(Manager());
	PageByKey::const_iterator itr = PageContainer.get<ByKey>().find(boost::make_tuple(PageName, ModuleId));
	if(itr == PageContainer.get<ByKey>().end())
	{
		return 0;
	}
	return itr->PageHandler;
}

AbstractPage * PagesDatabase::Get404Page() const
{
	ReadLock lock(Manager());
	return Page404Handler;
}

std::shared_ptr<const PageData> PagesDatabase::HomePagePtr(const std::string &HostName) const
{
	ReadLock lock(Manager());
	std::shared_ptr<const AccessHostNameData> AccessHostNamePtr = Server()->AccessPaths()->AccessHostOrGlobalPtr(HostName);
	if(AccessHostNamePtr && AccessHostNamePtr->DefaultPageId != -1)
	{
		return GetPtr(AccessHostNamePtr->DefaultPageId);
	}
	return std::shared_ptr<const PageData>();
}

void PagesDatabase::RegisterPageHandler(long long PageId, AbstractPage *handler)
{
	WriteLock lock(Manager());
	PageById::iterator itr = PageContainer.get<ById>().find(PageId);
	if(itr == PageContainer.get<ById>().end())
		return;

	if(itr->PageHandler)
		return;

	PageContainer.get<ById>().modify(itr, [handler](MetaPage &page) {
		page.PageHandler = handler;
	});
}

void PagesDatabase::RegisterPageHandler(const std::string &PageName, long long ModuleId, AbstractPage *handler)
{
	WriteLock lock(Manager());
	PageByKey::iterator itr = PageContainer.get<ByKey>().find(boost::make_tuple(PageName, ModuleId));
	if(itr == PageContainer.get<ByKey>().end())
		return;

	if(itr->PageHandler)
		return;

	PageContainer.get<ByKey>().modify(itr, [handler](MetaPage &page) {
		page.PageHandler = handler;
	});
}

void PagesDatabase::Register404PageHandler(AbstractPage *handler)
{
	if(!handler)
		return;

	WriteLock lock(Manager());
	if(Page404Handler)
		delete Page404Handler;

	Page404Handler = handler;
}

std::size_t PagesDatabase::CountPages() const
{
	ReadLock lock(Manager());
	return PageContainer.size();
}
long long PagesDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}

void PagesDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}
void PagesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
}

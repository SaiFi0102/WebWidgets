#include "DboDatabase/PageDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"
#include "Pages/PageHandler.h"

namespace WW
{

PageDatabase::PageDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

PageDatabase::~PageDatabase()
{
	for(const auto &page : _pageContainer)
		delete page.pageHandler;
}

void PageDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	PageContainer pagecontainer;

	Wt::Dbo::Transaction transaction(dboSession);
	Dbo::PageCollection pageCollection = dboSession.find<Dbo::Page>();

	//Create new entries in container, copy page handler if found from previous state
	for(Dbo::ptr<Dbo::Page> &ptr : pageCollection)
	{
		PageById::iterator i = _pageContainer.find(ptr.id());
		if(i != _pageContainer.end())
			pagecontainer.insert(MetaPage(Ddo::ptr<Ddo::Page>(new Ddo::Page(ptr)), i->pageHandler));
		else
			pagecontainer.insert(MetaPage(Ddo::ptr<Ddo::Page>(new Ddo::Page(ptr))));
	}

	//Set standard values for managed pages
	//Error page
	PageByKey::iterator errorItr = pagecontainer.get<ByKey>().find(boost::make_tuple(ERRORPAGENAME, ModuleDatabase::Navigation));
	if(errorItr != pagecontainer.get<ByKey>().end())
	{
		errorItr->pagePtr->authRestriction = Dbo::Page::AuthUnrestricted;
		errorItr->pagePtr->allowOnDisabledLogin = true;
	}
	else
		Wt::log("warn") << name() << ": " << "Page (\"" ERRORPAGENAME "\", ModulesDatabase::Navigation) not found";

	//404 page
	PageByKey::iterator notFoundItr = pagecontainer.get<ByKey>().find(boost::make_tuple(NOTFOUNDPAGENAME, ModuleDatabase::Navigation));
	if(notFoundItr != pagecontainer.get<ByKey>().end())
	{
		notFoundItr->pagePtr->authRestriction = Dbo::Page::AuthUnrestricted;
		notFoundItr->pagePtr->allowOnDisabledLogin = true;
	}
	else
		Wt::log("warn") << name() << ": " << "Page (\"" NOTFOUNDPAGENAME "\", ModulesDatabase::Navigation) not found";

	//Login page
	PageByKey::iterator loginItr = pagecontainer.get<ByKey>().find(boost::make_tuple("login", ModuleDatabase::Authentication));
	if(loginItr != pagecontainer.get<ByKey>().end())
		loginItr->pagePtr->authRestriction = Dbo::Page::AuthUnrestricted;


	//Register page
	PageByKey::iterator registerItr = pagecontainer.get<ByKey>().find(boost::make_tuple("register", ModuleDatabase::Authentication));
	if(registerItr != pagecontainer.get<ByKey>().end())
	{
		if(registerItr->pagePtr->authRestriction == Dbo::Page::AuthLoggedIn || registerItr->pagePtr->authRestriction == Dbo::Page::AuthLoggedInStrong)
			registerItr->pagePtr->authRestriction = Dbo::Page::AuthUnrestricted;
	}

	transaction.commit();
	_pageContainer.swap(pagecontainer);

	//Delete if any page handlers left in previous state
	//Happens if a page was removed from the backend database then reloaded
	for(const MetaPage &oldPage : pagecontainer)
	{
		PageById::iterator newPageItr = _pageContainer.find(oldPage.pagePtr->id());
		if(newPageItr == _pageContainer.end())
			delete oldPage.pageHandler;
	}

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = ptEnd - ptStart;

	Wt::log("info") << name() << ": " << _pageContainer.size() << " Page entires successfully loaded in "
		<< _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::Page> PageDatabase::getPtr(long long pageId) const
{
	ReadLock lock(manager());
	PageById::const_iterator itr = _pageContainer.get<ById>().find(pageId);
	if(itr == _pageContainer.get<ById>().end())
		return Ddo::cPtr<Ddo::Page>();

	return itr->pagePtr;
}
Ddo::cPtr<Ddo::Page> PageDatabase::getPtr(const std::string &pageName, long long moduleId) const
{
	ReadLock lock(manager());
	PageByKey::const_iterator itr = _pageContainer.get<ByKey>().find(boost::make_tuple(pageName, moduleId));
	if(itr == _pageContainer.get<ByKey>().end())
		return Ddo::cPtr<Ddo::Page>();

	return itr->pagePtr;
}

PageHandler *PageDatabase::getPage(long long pageId) const
{
	ReadLock lock(manager());
	PageById::const_iterator itr = _pageContainer.get<ById>().find(pageId);
	if(itr == _pageContainer.get<ById>().end())
		return nullptr;

	return itr->pageHandler;
}

PageHandler *PageDatabase::getPage(const std::string &pageName, long long moduleId) const
{
	ReadLock lock(manager());
	PageByKey::const_iterator itr = _pageContainer.get<ByKey>().find(boost::make_tuple(pageName, moduleId));
	if(itr == _pageContainer.get<ByKey>().end())
		return nullptr;

	return itr->pageHandler;
}

Ddo::cPtr<Ddo::Page> PageDatabase::homePagePtr(const std::string &hostName) const
{
	ReadLock lock(manager());
	Ddo::cPtr<Ddo::AccessHostName> AccessHostNamePtr = server()->accessPaths()->accessHostOrGlobalPtr(hostName);
	if(AccessHostNamePtr && AccessHostNamePtr->defaultPageId != -1)
		return getPtr(AccessHostNamePtr->defaultPageId);

	return Ddo::cPtr<Ddo::Page>();
}

void PageDatabase::visitPreloadPages(std::function<void(Ddo::cPtr<Ddo::Page>)> visitor) const
{
	ReadLock lock(manager());
	auto range = _pageContainer.get<ByPreload>().equal_range(true);
	for(auto itr = range.first;
		itr != range.second;
		++itr)
	{
		visitor(itr->pagePtr);
	}
}

void PageDatabase::registerPageHandler(long long pageId, PageHandler *handler)
{
	WriteLock lock(manager());
	PageById::iterator itr = _pageContainer.get<ById>().find(pageId);
	if(itr == _pageContainer.get<ById>().end())
		return;

	if(itr->pageHandler)
		return;

	_pageContainer.get<ById>().modify(itr, [handler](MetaPage &page) {
		page.pageHandler = handler;
	});
}

void PageDatabase::registerPageHandler(const std::string &pageName, long long moduleId, PageHandler *handler)
{
	if(!handler)
		return;

	WriteLock lock(manager());
	PageByKey::iterator itr = _pageContainer.get<ByKey>().find(boost::make_tuple(pageName, moduleId));
	if(itr == _pageContainer.get<ByKey>().end())
		return;

	if(itr->pageHandler)
		return;

	_pageContainer.get<ByKey>().modify(itr, [handler](MetaPage &page) {
		page.pageHandler = handler;
	});
}

void PageDatabase::registerReplacablePageHandler(const std::string &pageName, long long moduleId, PageHandler *handler)
{
	if(!handler)
		return;

	WriteLock lock(manager());
	PageByKey::iterator itr = _pageContainer.get<ByKey>().find(boost::make_tuple(pageName, moduleId));
	if(itr == _pageContainer.get<ByKey>().end())
		return;
	if(itr->pageHandler == handler)
		return;

	_pageContainer.get<ByKey>().modify(itr, [&handler](MetaPage &page) {
		std::swap(page.pageHandler, handler);
	});

	//Delete old handler
	delete handler;
}

std::size_t PageDatabase::pagesCount() const
{
	ReadLock lock(manager());
	return _pageContainer.size();
}
long long PageDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}

void PageDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	fetchAll(dboSession);
}
void PageDatabase::reload(Wt::Dbo::Session &dboSession)
{
	fetchAll(dboSession);
}

}

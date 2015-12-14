#include "Widgets/PageStackWidget.h"
#include "Application/WApplication.h"
#include "Application/WServer.h"
#include "DboDatabase/PageDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/ModuleDatabase.h"
#include "Pages/PageHandler.h"

#include <Wt/WEnvironment>

namespace WW
{

PageStackWidget::PageStackWidget(Wt::WContainerWidget *parent)
: Wt::WStackedWidget(parent)
{
	WApplication *app = WApplication::instance();
	WServer *server = WServer::instance();
	if(!app || !server)
		throw std::runtime_error("PagesStackWidget constructed outside of Wt loop");

	server->pages()->visitPreloadPages(std::bind(&PageStackWidget::lazyLoadPageVisitor, this, std::placeholders::_1));
	app->pageChanged().connect(std::bind(&PageStackWidget::handlePageChanged, this));
	app->session().login().changed().connect(std::bind(&PageStackWidget::checkAuthorization, this, nullptr));
}

void PageStackWidget::handlePageChanged()
{
	WApplication *app = WApplication::instance();
	WServer *server = WServer::instance();
	if(!app || !server)
		return;

	auto pagePtr = app->currentPage();

	//404 page not found
	if(!pagePtr)
	{
		PageHandler *page404Handler;
		auto validParentPagePtr = app->validParentPage();
		long long pageId = -1;

		if(validParentPagePtr)
		{
			page404Handler = server->pages()->getPage(validParentPagePtr->id());
			if(page404Handler && page404Handler->get404Page())
			{
				page404Handler = page404Handler->get404Page();
				pageId = validParentPagePtr->id();
			}
			else
				page404Handler = server->pages()->get404Page();
		}
		else
			page404Handler = server->pages()->get404Page();

		//Lazy load 404 page content
		if(page404Handler)
		{
			auto itr = _index404Map.find(pageId);
			if(itr == _index404Map.end())
			{
				Wt::WWidget *content = page404Handler->createContent();
				addWidget(content);
				itr = _index404Map.insert(std::make_pair(pageId, indexOf(content))).first;
			}
			setCurrentIndex(itr->second);
		}
		else
			setCurrentIndex(-1);

		return;
	}

	//Page found, check authorization
	if(!checkAuthorization(pagePtr))
		return;

	//Lazy load page content
	int index = lazyLoadPage(pagePtr->id(), _indexMap);
	setCurrentIndex(index);
}

int PageStackWidget::lazyLoadPage(long long pageId, PageIdIndexMap &indexMap)
{
	WServer *server = WServer::instance();
	auto itr = indexMap.find(pageId);
	if(itr == indexMap.end() || itr->second < children().size())
	{
		PageHandler *PageHandler = server->pages()->getPage(pageId);
		if(!PageHandler)
			return -1; //Valid page entry, no widget associated

		Wt::WWidget *content = PageHandler->createContent();
		addWidget(content);
		itr = indexMap.insert(std::make_pair(pageId, indexOf(content))).first;
	}
	return itr->second;
}

int PageStackWidget::lazyLoadPage(const std::string &pageName, long long moduleId)
{
	auto ptr = WServer::instance()->pages()->getPtr(pageName, moduleId);
	if(!ptr)
		return -1;

	return lazyLoadPage(ptr->id());
}

Wt::WWidget *PageStackWidget::getPageContent(long long pageId) const
{
	auto itr = _indexMap.find(pageId);
	if(itr == _indexMap.end())
		return nullptr;

	return widget(itr->second);
}

Wt::WWidget *PageStackWidget::getPageContent(const std::string &pageName, long long moduleId) const
{
	auto ptr = WServer::instance()->pages()->getPtr(pageName, moduleId);
	if(!ptr)
		return nullptr;

	return getPageContent(ptr->id());
}

void PageStackWidget::changePage(long long pageId)
{
	WApplication *app = WApplication::instance();
	if(!app)
		return;

	std::string path = WServer::instance()->accessPaths()->firstPagePath(pageId, app->environment().hostName());
	if(path == "/")
		return;

	app->setInternalPathAfterReserved(path, true);
}

void PageStackWidget::changePage(const std::string &pageName, long long moduleId)
{
	auto Ptr = WServer::instance()->pages()->getPtr(pageName, moduleId);
	if(!Ptr)
		return;

	changePage(Ptr->id());
}

bool PageStackWidget::checkAuthorization(Ddo::cPtr<Ddo::Page> pagePtr)
{
	WApplication *app = WApplication::instance();
	if(!pagePtr)
		pagePtr = app->currentPage();
	if(!pagePtr)
		return true;

	if(pagePtr->authRestriction == Dbo::Page::AuthUnrestricted)
		return true;

	if(app->session().login().loggedIn())
	{
		if(pagePtr->authRestriction == Dbo::Page::AuthLoggedOut)
		{
			//Not allowed when logged in
			app->setInternalPathAfterReserved("/", true);
			return false;
		}
		if(pagePtr->authRestriction == Dbo::Page::AuthLoggedInStrong && app->session().login().state() != Wt::Auth::StrongLogin)
		{
			//Require re-login
			changePage("login", ModuleDatabase::Authentication);
			return false;
		}
	}
	else //Logged out or disabled login
	{
		if(app->session().login().state() == Wt::Auth::DisabledLogin)
		{
			//if(!pagePtr->allowOnDisabledLogin)
			//Show login disabled page
		}
		else //Logged out
		{
			if(pagePtr->authRestriction == Dbo::Page::AuthLoggedIn || pagePtr->authRestriction == Dbo::Page::AuthLoggedInStrong)
			{
				//Require login
				changePage("login", ModuleDatabase::Authentication);
				return false;
			}
		}
	}
	return true;
}

}

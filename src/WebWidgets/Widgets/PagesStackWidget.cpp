#include "Widgets/PagesStackWidget.h"
#include "Application/Application.h"
#include "Application/WServer.h"
#include "DboDatabase/PagesDatabase.h"
#include "Pages/AbstractPage.h"

PagesStackWidget::PagesStackWidget(Wt::WContainerWidget *parent)
: Wt::WStackedWidget(parent)
{
	Application *app = Application::instance();
	WServer *Server = WServer::instance();
	if(!app || !Server)
		throw std::runtime_error("PagesStackWidget constructed outside of Wt loop");

	app->PageChanged().connect(std::bind(&PagesStackWidget::HandlePageChanged, this));
}

void PagesStackWidget::HandlePageChanged()
{
	Application *app = Application::instance();
	WServer *server = WServer::instance();
	if(!app || !server)
		return;

	auto PagePtr = app->CurrentPage();

	//404 page not found
	if(!PagePtr)
	{
		AbstractPage *Page404Handler;
		auto ValidParentPagePtr = app->ValidParentPage();
		long long PageId = -1;

		if(ValidParentPagePtr)
		{
			Page404Handler = server->Pages()->GetPage(ValidParentPagePtr->id());
			if(Page404Handler && Page404Handler->Get404Page())
			{
				Page404Handler = Page404Handler->Get404Page();
				PageId = ValidParentPagePtr->id();
			}
			else
				Page404Handler = server->Pages()->Get404Page();
		}
		else
			Page404Handler = server->Pages()->Get404Page();

		//Lazy load 404 page content
		if(Page404Handler)
		{
			auto itr = index404Map.find(PageId);
			if(itr == index404Map.end())
			{
				Wt::WWidget *content = Page404Handler->CreateContent();
				addWidget(content);
				itr = index404Map.insert(std::make_pair(PageId, indexOf(content))).first;
			}
			setCurrentIndex(itr->second);
		}
		else
			setCurrentIndex(-1);

		return;
	}

	//Page found, lazy load page content
	auto itr = indexMap.find(PagePtr->id());
	if(itr == indexMap.end())
	{
		AbstractPage *PageHandler = server->Pages()->GetPage(PagePtr->id());
		if(!PageHandler)
		{
			setCurrentIndex(-1);
			return; //Valid page entry, no widget associated
		}

		Wt::WWidget *content = PageHandler->CreateContent();
		addWidget(content);
		itr = indexMap.insert(std::make_pair(PagePtr->id(), indexOf(content))).first;
	}
	setCurrentIndex(itr->second);
}

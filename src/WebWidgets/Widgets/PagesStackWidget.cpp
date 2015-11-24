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
	if(!PagePtr)
		return; //404

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

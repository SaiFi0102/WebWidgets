#include "Pages/PageHandler.h"
#include <Wt/WText>
#include <Wt/WContainerWidget>

namespace WW
{

// void PageSections::Add(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO)
// {
// 	if(Widget)
// 	{
// 		_SectionMap[std::make_pair(HO, VO)] = Widget;
// 	}
// }
// 
// bool PageSections::Remove(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO)
// {
// 	if(Widget)
// 	{
// 		return _SectionMap.erase(std::make_pair(HO, VO)) != 0;
// 	}
// 	return false;
// }

Wt::WWidget *Default404Page::createContent()
{
	Wt::WContainerWidget *widget = new Wt::WContainerWidget();
	new Wt::WText("!!!404!!!", widget);
	return widget;
}

void PageHandler::set404Page(PageHandler *page)
{
	std::swap(_page404, page);
	delete page;
}

}
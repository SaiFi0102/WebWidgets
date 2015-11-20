#ifndef WWMENU_WIDGET_H
#define WWMENU_WIDGET_H

#include <Wt/WMenu>
#include "Dbo/Page.h"

class PagesMenuItem : public Wt::WMenuItem
{
	public:
		PagesMenuItem(boost::shared_ptr<const NavigationMenuItemData> ItemPtr, Wt::WMenuItem::LoadPolicy policy = Wt::WMenuItem::LazyLoading);

	protected:
		boost::shared_ptr<const NavigationMenuItemData> ItemPtr;
};

class PagesMenu : public Wt::WMenu
{
	public:
		PagesMenu(Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent = 0);

	protected:
		virtual void PageChanged();
		virtual void select(int index, bool changePath);
};

#endif
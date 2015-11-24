#ifndef WWMENU_WIDGET_H
#define WWMENU_WIDGET_H

#include <Wt/WMenu>
#include "Dbo/Page.h"

class PagesMenuItem : public Wt::WMenuItem
{
	public:
		PagesMenuItem(std::shared_ptr<const NavigationMenuItemData> ItemPtr);
		long long ItemId() const { return ItemPtr->Id; };

	protected:
		std::shared_ptr<const NavigationMenuItemData> ItemPtr;
};

class PagesMenu : public Wt::WMenu
{
	public:
		PagesMenu(long long MenuId, Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent = 0);
		PagesMenu(long long MenuId, Wt::WContainerWidget *parent = 0);
		PagesMenu(long long MenuId, bool doNotLoad);

		virtual void setInternalPathEnabled(const std::string& basePathAfterReserved = "");
		std::string BasePathAfterReserved() const { return _BasePathAfterReserved; }
		void SetBasePathAfterReserved(const std::string& basePathAfterReserved = "") { setInternalPathEnabled(basePathAfterReserved); }

		long long MenuId() const { return _MenuId; };

	private:
		virtual void HandleReservedInternalPathChanged();
		long long _MenuId;
		std::string _BasePathAfterReserved;
};

#endif
#ifndef WW_WIDGET_PAGEMENU_H
#define WW_WIDGET_PAGEMENU_H

#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class DboMenuItem : public Wt::WMenuItem
	{
	public:
		DboMenuItem(Ddo::cPtr<Ddo::NavigationMenuItem> itemPtr);
		long long itemId() const { return _itemPtr->id; };

		virtual std::string pathComponent() const override;

	private:
		enum HiddenFlags
		{
			NotHidden = 0x00,
			HiddenFromLocale = 0x01,
			HiddenFromAccessPath = 0x02,
			HiddenFromAccessHost = 0x04,
			HiddenFromStyle = 0x08,
			HiddenFromPage = 0x10,
			HiddenFromLoginState = 0x20
		};

		void handleInternalPathChanged();
		void handleLocaleChanged();
		void handlePageChanged();
		void handleStyleChanged();
		void handleAuthEvent();

		char _hiddenFlags;
		Ddo::cPtr<Ddo::NavigationMenuItem> _itemPtr;
		Ddo::cPtr<Ddo::PageAccessPath> _accessPathPtr;
	};

	class DboMenu : public Wt::WMenu
	{
	public:
		DboMenu(long long menuId, Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent = nullptr);
		DboMenu(long long menuId, Wt::WContainerWidget *parent = nullptr);
		DboMenu(long long menuId, bool doNotLoad);

		virtual void setInternalPathEnabled(const std::string& basePathAfterReserved = "") override;
		std::string BasePathAfterReserved() const { return _basePathAfterReserved; }
		void SetBasePathAfterReserved(const std::string& basePathAfterReserved = "") { setInternalPathEnabled(basePathAfterReserved); }

		long long menuId() const { return _menuId; };

	private:
		void handleReservedInternalPathChanged();

		long long _menuId;
		std::string _basePathAfterReserved;
	};
}

#endif
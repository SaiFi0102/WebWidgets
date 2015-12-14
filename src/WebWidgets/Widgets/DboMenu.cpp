#include "Widgets/DboMenu.h"
#include "Application/WApplication.h"
#include "Application/WServer.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/NavigationMenuDatabase.h"
#include "DboDatabase/PageDatabase.h"
#include <Wt/WebUtils.h>

namespace WW
{

DboMenu::DboMenu(long long menuId, Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent)
: Wt::WMenu(contentsStack, parent), _menuId(menuId)
{
	WServer *server = WServer::instance();
	if(!server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	server->navigationMenus()->populateMenuItems(this);
}

DboMenu::DboMenu(long long menuId, Wt::WContainerWidget *parent)
: Wt::WMenu(parent), _menuId(menuId)
{
	WServer *server = WServer::instance();
	if(!server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	server->navigationMenus()->populateMenuItems(this);
}

DboMenu::DboMenu(long long menuId, bool doNotLoad)
: _menuId(menuId)
{
	WServer *server = WServer::instance();
	if(!server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	if(!doNotLoad)
		server->navigationMenus()->populateMenuItems(this);
}

void DboMenu::handleReservedInternalPathChanged()
{
	WApplication *app = WApplication::instance();
	if(!app)
		return;

	setInternalBasePath(app->reservedInternalPath() + _basePathAfterReserved);
}

void DboMenu::setInternalPathEnabled(const std::string& basePath /*= ""*/)
{
	WApplication *app = WApplication::instance();
	if(!app)
		return;

	_basePathAfterReserved = Wt::Utils::append(basePath, '/');
	if(_basePathAfterReserved[0] == '/')
		_basePathAfterReserved.erase(0);

	if(!internalPathEnabled())
		app->reservedInternalPathChanged().connect(std::bind(&DboMenu::handleReservedInternalPathChanged, this));

	Wt::WMenu::setInternalPathEnabled(app->reservedInternalPath() + _basePathAfterReserved);
}

DboMenuItem::DboMenuItem(Ddo::cPtr<Ddo::NavigationMenuItem> itemPtr)
: Wt::WMenuItem(Wt::WString::tr(itemPtr->labelKey, itemPtr->labelModuleId), 0, itemPtr->preload ? Wt::WMenuItem::PreLoading : Wt::WMenuItem::LazyLoading),
	_itemPtr(itemPtr), _hiddenFlags(NotHidden)
{
	WServer *server = WServer::instance();
	WApplication *app = WApplication::instance();

	if(itemPtr->accessPathId != -1)
		_accessPathPtr = server->accessPaths()->pageAccessPathPtr(itemPtr->accessPathId);

	app->pageChanged().connect(std::bind(&DboMenuItem::handlePageChanged, this));
	app->localeChanged().connect(std::bind(&DboMenuItem::handleLocaleChanged, this));
	app->styleChanged().connect(std::bind(&DboMenuItem::handleStyleChanged, this));
	app->internalPathChanged().connect(std::bind(&DboMenuItem::handlePageChanged, this));
	app->session().login().changed().connect(std::bind(&DboMenuItem::handleAuthEvent, this));
}

std::string DboMenuItem::pathComponent() const
{
	if(_accessPathPtr && parentMenu())
	{
		std::string newPath = _accessPathPtr->fullPath;
		std::string basePath = parentMenu()->internalBasePath().substr(1);
		size_t pos = newPath.find(basePath.c_str(), 0, basePath.size());
		if(pos != std::string::npos)
			newPath.erase(0, basePath.size());

		return newPath;
	}

	return Wt::WMenuItem::pathComponent();
}

void DboMenuItem::handleInternalPathChanged()
{
	WApplication *app = WApplication::instance();
	WServer *server = WServer::instance();

	//Access host name
	if(_itemPtr->showOnAccessHostNames.find(app->accessHostName()->hostName) == _itemPtr->showOnAccessHostNames.end())
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistAccessHosts)
			_hiddenFlags |= HiddenFromAccessHost; //Not found, white list mode, hide
		else
			_hiddenFlags &= ~HiddenFromAccessHost; //Not found, black list mode, show
	}
	else
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistAccessHosts)
			_hiddenFlags &= ~HiddenFromAccessHost; //Found, white list mode, show
		else
			_hiddenFlags |= HiddenFromAccessHost; //Found, black list mode, hide
	}

	//Page access path
	auto AccessPathPtr = app->currentPageAccessPath();
	if(!AccessPathPtr)
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistAccessPaths)
			_hiddenFlags |= HiddenFromAccessPath; //Not found, white list mode, hide
		else
			_hiddenFlags &= ~HiddenFromAccessPath; //Not found, black list mode, show
	}
	while(AccessPathPtr)
	{
		auto fitr = _itemPtr->showOnAccessPathIds.find(app->currentPageAccessPath()->id());
		if(fitr == _itemPtr->showOnAccessPathIds.end())
		{
			if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistAccessPaths)
				_hiddenFlags |= HiddenFromAccessPath; //Not found, white list mode, hide
			else
				_hiddenFlags &= ~HiddenFromAccessPath; //Not found, black list mode, show

			//Set parent
			if(app->currentPageAccessPath()->parentAccessPathId == -1)
				AccessPathPtr = nullptr;
			else
				AccessPathPtr = server->accessPaths()->pageAccessPathPtr(app->currentPageAccessPath()->parentAccessPathId);
		}
		else
		{
			if(fitr->second || AccessPathPtr == app->currentPageAccessPath()) //if recursive or first iteration
			{
				if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistAccessPaths)
					_hiddenFlags &= ~HiddenFromAccessPath; //Found, white list mode, show
				else
					_hiddenFlags |= HiddenFromAccessPath; //Found, black list mode, hide

				AccessPathPtr = nullptr;
			}
			else
				AccessPathPtr = nullptr;
		}
	}

	setHidden(_hiddenFlags != NotHidden);
}

void DboMenuItem::handleLocaleChanged()
{
	WApplication *app = WApplication::instance();
	if(_itemPtr->showOnLanguageCodes.find(app->locale().name()) == _itemPtr->showOnLanguageCodes.end())
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistLanguages)
			_hiddenFlags |= HiddenFromLocale; //Not found, white list mode, hide
		else
			_hiddenFlags &= ~HiddenFromLocale; //Not found, black list mode, show
	}
	else
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistLanguages)
			_hiddenFlags &= ~HiddenFromLocale; //Found, white list mode, show
		else
			_hiddenFlags |= HiddenFromLocale; //Found, black list mode, hide
	}

	setHidden(_hiddenFlags != NotHidden);
}

void DboMenuItem::handlePageChanged()
{
	WApplication *app = WApplication::instance();
	WServer *server = WServer::instance();

	auto pagePtr = app->currentPage();
	if(!pagePtr)
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistPages)
			_hiddenFlags |= HiddenFromPage; //Not found, white list mode, hide
		else
			_hiddenFlags &= ~HiddenFromPage; //Not found, black list mode, show
	}
	while(pagePtr)
	{
		auto fitr = _itemPtr->showOnPageIds.find(app->currentPage()->id());
		if(fitr == _itemPtr->showOnPageIds.end())
		{
			if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistPages)
				_hiddenFlags |= HiddenFromPage; //Not found, white list mode, hide
			else
				_hiddenFlags &= ~HiddenFromPage; //Not found, black list mode, show

			//Set parent
			if(app->currentPage()->parentPageId == -1)
				pagePtr = nullptr;
			else
				pagePtr = server->pages()->getPtr(app->currentPage()->parentPageId);
		}
		else
		{
			if(fitr->second || pagePtr == app->currentPage()) //if recursive or first iteration
			{
				if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistPages)
					_hiddenFlags &= ~HiddenFromPage; //Found, white list mode, show
				else
					_hiddenFlags |= HiddenFromPage; //Found, black list mode, hide

				pagePtr = nullptr;
			}
			else
				pagePtr = nullptr;
		}
	}

	setHidden(_hiddenFlags != NotHidden);
}

void DboMenuItem::handleStyleChanged()
{
	WApplication *app = WApplication::instance();
	if(_itemPtr->showOnStyleIds.find(app->currentStyle()->id()) == _itemPtr->showOnStyleIds.end())
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistStyles)
			_hiddenFlags |= HiddenFromStyle; //Not found, white list mode, hide
		else
			_hiddenFlags &= ~HiddenFromStyle; //Not found, black list mode, show
	}
	else
	{
		if(_itemPtr->whitelistFlags & Dbo::NavigationMenuItem::WhitelistStyles)
			_hiddenFlags &= ~HiddenFromStyle; //Found, white list mode, show
		else
			_hiddenFlags |= HiddenFromStyle; //Found, black list mode, hide
	}

	setHidden(_hiddenFlags != NotHidden);
}

void DboMenuItem::handleAuthEvent()
{
	if(_itemPtr->loggedInVisibility == Dbo::NavigationMenuItem::LoggedInNoRestriction)
	{
		_hiddenFlags &= ~HiddenFromLoginState;
		setHidden(_hiddenFlags != NotHidden);
		return;
	}

	WApplication *app = WApplication::instance();
	if(_itemPtr->loggedInVisibility == Dbo::NavigationMenuItem::LoggedInOnly)
	{
		if(app->session().login().loggedIn())
			_hiddenFlags &= ~HiddenFromLoginState; //Show
		else
			_hiddenFlags |= HiddenFromLoginState; //Hide
	}
	else
	{
		if(app->session().login().loggedIn())
			_hiddenFlags |= HiddenFromLoginState; //Hide
		else
			_hiddenFlags &= ~HiddenFromLoginState; //Show
	}

	setHidden(_hiddenFlags != NotHidden);
}

}

#include "Widgets/PagesMenu.h"
#include "Application/Application.h"
#include "Application/WServer.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/NavigationMenusDatabase.h"
#include <WebUtils.h>

PagesMenu::PagesMenu(long long MenuId, Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent)
: Wt::WMenu(contentsStack, parent), _MenuId(MenuId)
{
	WServer *Server = WServer::instance();
	if(!Server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	Server->NavigationMenus()->PopulateMenuItems(this);
}

PagesMenu::PagesMenu(long long MenuId, Wt::WContainerWidget *parent)
: Wt::WMenu(parent), _MenuId(MenuId)
{
	WServer *Server = WServer::instance();
	if(!Server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	Server->NavigationMenus()->PopulateMenuItems(this);
}

PagesMenu::PagesMenu(long long MenuId, bool doNotLoad)
: _MenuId(MenuId)
{
	WServer *Server = WServer::instance();
	if(!Server)
		throw std::runtime_error("PagesMenu constructed outside of Wt loop");

	setInternalPathEnabled();
	if(!doNotLoad)
		Server->NavigationMenus()->PopulateMenuItems(this);
}

void PagesMenu::HandleReservedInternalPathChanged()
{
	Application *app = Application::instance();
	if(!app)
		return;

	setInternalBasePath(app->ReservedInternalPath() + _BasePathAfterReserved);
}

void PagesMenu::setInternalPathEnabled(const std::string& basePath /*= ""*/)
{
	Application *app = Application::instance();
	if(!app)
		return;

	_BasePathAfterReserved = Wt::Utils::append(basePath, '/');
	if(_BasePathAfterReserved[0] == '/')
		_BasePathAfterReserved.erase(0);

	if(!internalPathEnabled())
		app->reservedInternalPathChanged().connect(std::bind(&PagesMenu::HandleReservedInternalPathChanged, this));

	Wt::WMenu::setInternalPathEnabled(app->ReservedInternalPath() + _BasePathAfterReserved);
}

PagesMenuItem::PagesMenuItem(std::shared_ptr<const NavigationMenuItemData> ItemPtr)
: Wt::WMenuItem(Wt::WString::tr(ItemPtr->LabelKey, ItemPtr->LabelModuleId), 0, ItemPtr->Preload ? Wt::WMenuItem::PreLoading : Wt::WMenuItem::LazyLoading),
	ItemPtr(ItemPtr)
{
	WServer *Server = WServer::instance();
	if(ItemPtr->AccessPathId != -1)
		AccessPathPtr = Server->AccessPaths()->PageAccessPathPtr(ItemPtr->AccessPathId);
}

std::string PagesMenuItem::pathComponent() const
{
	if(AccessPathPtr && parentMenu())
	{
		std::string NewPath = AccessPathPtr->FullPath;
		std::string BasePath = parentMenu()->internalBasePath().substr(1);
		size_t pos = NewPath.find(BasePath.c_str(), 0, BasePath.size());
		if(pos != std::string::npos)
			NewPath.erase(0, BasePath.size());

		return NewPath;
	}

	return Wt::WMenuItem::pathComponent();
}

// void PagesMenu::select(int index, bool changePath)
// {
// 	Application *app = Application::instance();
// 	if(!app)
// 		return;
// 
// 	int last = currentIndex();
// 	setCurrent(index);
// 
// 	Wt::WMenuItem *item = index >= 0 ? itemAt(index) : 0;
// 	if(changePath && index != -1)
// 	{
// 		
// 	}
// 
// 
// 
// 
// 	if(contentsStack())
// 		previousStackIndex_ = contentsStack()->currentIndex();
// 
// 	WMenuItem *item = index >= 0 ? itemAt(index) : 0;
// 
// 	if(changePath && internalPathEnabled_ &&
// 		index != -1 && item->internalPathEnabled())
// 	{
// 		WApplication *app = wApp;
// 		previousInternalPath_ = app->internalPath();
// 
// 		std::string newPath = basePath_ + item->pathComponent();
// 		if(newPath != app->internalPath())
// 			emitPathChange_ = true;
// 
// 		// The change is emitted in select()
// 		app->setInternalPath(newPath);
// 	}
// 
// 	for(int i = 0; i < count(); ++i)
// 		renderSelected(itemAt(i), (int)i == index);
// 
// 	if(index == -1)
// 		return;
// 
// 	if(showContents && contentsStack_)
// 	{
// 		WWidget *contents = item->contents();
// 		if(contents)
// 			contentsStack_->setCurrentWidget(contents);
// 	}
// 
// 	itemSelectRendered_.emit(item);
// 
// 
// 
// 
// 	if(index != -1)
// 	{
// 		WMenuItem *item = itemAt(index);
// 		item->show();
// 		item->loadContents();
// 
// 		DeletionTracker guard(this);
// 
// 		if(changePath && emitPathChange_)
// 		{
// 			WApplication *app = wApp;
// 			app->internalPathChanged().emit(app->internalPath());
// 			if(guard.deleted())
// 				return;
// 			emitPathChange_ = false;
// 		}
// 
// 		if(last != index)
// 		{
// 			item->triggered().emit(item);
// 			if(!guard.deleted())
// 			{
// 				// item may have been deleted too
// 				if(ul()->indexOf(item) != -1)
// 					itemSelected_.emit(item);
// 				else
// 					select(-1);
// 			}
// 		}
// 	}
// }

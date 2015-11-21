// #include "Widgets/PagesMenu.h"
// #include "Application/Application.h"
// #include "Application/WServer.h"
// #include "DboDatabase/AccessPathsDatabase.h"
// 
// PagesMenu::PagesMenu(Wt::WStackedWidget *contentsStack, Wt::WContainerWidget *parent)
// : Wt::WMenu(contentsStack, parent)
// {
// 	Application *app = Application::instance();
// 	if(!app)
// 	{
// 		throw std::runtime_error("PagesMenu constructed outside of Wt loop");
// 	}
// 
// 	app->PageChanged().connect(std::bind(&PagesMenu::PageChanged, this));
// }
// 
// void PagesMenu::PageChanged()
// {
// 	Application *app = Application::instance();
// 	if(!app)
// 	{
// 		return;
// 	}
// 
// 	app->CurrentPage();
// }
// 
// void PagesMenu::select(int index, bool changePath)
// {
// 	Application *app = Application::instance();
// 	if(!app)
// 	{
// 		return;
// 	}
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
// 
// PagesMenuItem::PagesMenuItem(std::shared_ptr<const NavigationMenuItemData> ItemPtr, Wt::WMenuItem::LoadPolicy policy)
// : Wt::WMenuItem(Wt::WString::tr(ItemPtr->LabelKey, ItemPtr->LabelModuleId), 0, policy), ItemPtr(ItemPtr)
// {
// 	WServer *Server = WServer::instance();
// 	if(ItemPtr->AccessPathId != -1)
// 	{
// 		std::shared_ptr<const PageAccessPathData> AccessPathPtr = Server->AccessPaths()->PageAccessPathPtr(ItemPtr->AccessPathId);
// 		if(AccessPathPtr)
// 		{
// 			setPathComponent(AccessPathPtr->FullPath);
// 		}
// 	}
// }

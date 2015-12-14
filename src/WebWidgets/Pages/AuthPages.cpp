#include "Pages/AuthPages.h"
#include "Widgets/PageStackWidget.h"
#include "Widgets/AuthWidget.h"
#include "Application/WApplication.h"
#include "Application/WServer.h"
#include "DboDatabase/ModuleDatabase.h"

namespace WW
{

Wt::WWidget *LoginPage::createContent()
{
	WServer *server = WServer::instance();
	WApplication *app = WApplication::instance();
	AuthWidget *authWidget = new AuthWidget(app->session());
	authWidget->model()->addPasswordAuth(&server->getPasswordService());
	authWidget->model()->addOAuth(server->getOAuthServices());
	authWidget->setRegistrationEnabled(true);
	authWidget->processEnvironment();
	return authWidget;
}

Wt::WWidget *RegistrationPage::createContent()
{
	//WServer *server = WServer::instance();
	WApplication *app = WApplication::instance();
	int loginPageIndex = app->pageStack()->lazyLoadPage("login", ModuleDatabase::Authentication);
	AuthWidget *authWidget = dynamic_cast<AuthWidget*>(app->pageStack()->widget(loginPageIndex));
	if(!authWidget)
		return nullptr;

	return authWidget->createRegistrationView(Wt::Auth::Identity::Invalid);
}

}

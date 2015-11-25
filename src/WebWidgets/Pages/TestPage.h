#ifndef WWTEST_PAGE_H
#define WWTEST_PAGE_H

#include "Pages/AbstractPage.h"
#include "Application/Application.h"
#include "Application/WServer.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationsCache.h"

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
class Another404Page : public AbstractPage
{
	virtual Wt::WWidget *CreateContent()
	{
		return new Wt::WText("!!!NNNNNNNNNNNNNNNNOLA!!!");
	}
};

class AnotherPage : public AbstractPage
{
public:
	AnotherPage()
	{
		Set404Page(new Another404Page());
	}
protected:
	virtual Wt::WWidget *CreateContent()
	{
		return new Wt::WText("OLA!!!");
	}
};

class TestPage : public AbstractPage
{
	virtual Wt::WWidget *CreateContent()
	{
		//TEST//
		WServer *server = WServer::instance();
		Application *app = Application::instance();

		Wt::WContainerWidget *root = new Wt::WContainerWidget();

		new Wt::WText("HI", root);
		new Wt::WBreak(root);
		(new Wt::WText("Internal path: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto ip = new Wt::WText(app->internalPath(), root);
		new Wt::WBreak(root);
		(new Wt::WText("Reserved internal path: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto ipr = new Wt::WText(app->ReservedInternalPath(), root);
		new Wt::WBreak(root);
		(new Wt::WText("Internal path after reserve(subpath): ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto ipar = new Wt::WText(app->InternalPathAfterReserved(), root);
		new Wt::WBreak(root);
		(new Wt::WText("Internal path after reserve next part(Arg '/'): ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto iparnp = new Wt::WText(app->InternalPathAfterReservedNextPart("/"), root);
		new Wt::WBreak(root);
		(new Wt::WText("Current language: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto txt = new Wt::WText(app->locale().name(), root);
		app->internalPathChanged().connect(std::bind<void>([ip, ipr]() {
			Application *app = Application::instance();
			ip->setText(app->internalPath());
			ipr->setText(app->ReservedInternalPath());
		}));
		app->internalPathAfterReservedChanged().connect(std::bind<void>([ipar, iparnp]() {
			Application *app = Application::instance();
			ipar->setText(app->InternalPathAfterReserved());
			iparnp->setText(app->InternalPathAfterReservedNextPart("/"));
		}));
		app->LocaleChanged().connect(std::bind<void>([txt]() {
			Application *app = Application::instance();
			txt->setText(app->locale().name());
		}));
		new Wt::WBreak(root);
		(new Wt::WText(std::string("Session Default Language: ") + app->_SessionDefaultLocale.name(), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		new Wt::WBreak(root);
		(new Wt::WText(std::string("Mobile Version: "), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto mvt = new Wt::WText(std::string(app->IsMobileVersion() ? "On" : "Off"), root);
		app->MobileVersionChanged().connect(std::bind<void>([mvt](bool MobileVersion) {
			mvt->setText(MobileVersion ? "On" : "Off");
		}, std::placeholders::_1));
		new Wt::WBreak(root);
		(new Wt::WText(std::string("Current Page: "), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
		auto cpt = new Wt::WText(Wt::WString(app->CurrentPage() ? Wt::WString::tr(app->CurrentPage()->TitleKey, app->CurrentPage()->TitleModuleId) : "Invalid Page(404)"), root);
		new Wt::WBreak(root);
		app->PageChanged().connect(std::bind<void>([cpt]() {
			Application *app = Application::instance();
			cpt->setText(app->CurrentPage() ? Wt::WString::tr(app->CurrentPage()->TitleKey, app->CurrentPage()->TitleModuleId) : "Invalid Page(404)");
		}));

		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/testing"), "Test Link", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/en"), "/en", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/fr"), "/fr", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/english"), "/english", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/french"), "/french", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/"), "/", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums"), "/forums", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums/topic"), "/forums/topic", root);
		new Wt::WBreak(root);
		new Wt::WAnchor(Wt::WLink(Wt::WLink::InternalPath, "/forums/topic/1"), "/forums/topic/1", root);
		new Wt::WBreak(root);
		Wt::WPushButton *rc = new Wt::WPushButton("Reload Configurations", root);
		new Wt::WBreak(root);
		rc->clicked().connect(std::bind<void>([root, server]() {
			Application *app = Application::instance();
			new Wt::WText(std::string("[Server Config; Before Reload()]Log Directory: ") + server->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root);
			new Wt::WText(std::string("[Cache Config; Before Reload()]Log Directory: ") + app->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root);
			new Wt::WBreak(root);
			server->DatabaseManager()->Reload();
			new Wt::WText(std::string("[Server Config; AFTER Reload()]Log Directory: ") + server->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root);
			new Wt::WText(std::string("[Cache Config; AFTER Reload()]Log Directory: ") + app->Configurations()->GetStr("LogDirectory", ModulesDatabase::Logging, "./default"), root);
			new Wt::WBreak(root);
		}));

		Wt::WPushButton *rs = new Wt::WPushButton("Reload Styles", root);
		rs->clicked().connect(std::bind<void>([server]() {
			server->DatabaseManager()->Reload();
		}));
		Wt::WPushButton *cs = new Wt::WPushButton("Change Style", root);
		cs->clicked().connect(std::bind<void>([server]() {
			Application *app = Application::instance();
			if(!app->CurrentStyle())
			{
				app->ChangeStyle("Default", 1);
			}
			else if(app->CurrentStyle()->Name() == "Default")
			{
				app->ChangeStyle("Test", 1);
			}
			else
			{
				app->ChangeStyle("Default", 1);
			}
			app->triggerUpdate();
		}));
		new Wt::WBreak(root);
		Wt::WPushButton *rl = new Wt::WPushButton("Reload languages", root);
		rl->clicked().connect(std::bind<void>([server]() {
			server->DatabaseManager()->Reload();
		}));
		new Wt::WBreak(root);
		new Wt::WText(Wt::WString::tr("Wt.Auth.email"), root);
		new Wt::WBreak(root);
		new Wt::WText(Wt::WString::tr("test", ModulesDatabase::Authentication), root);
		new Wt::WBreak(root);
		new Wt::WBreak(root);
		new Wt::WTemplate(Wt::WString::tstr("styletpl", ModulesDatabase::Styles), root);
		new Wt::WTemplate(Wt::WString::tstr("templatetpl", ModulesDatabase::Styles), root);

		return root;
	}
};

#endif
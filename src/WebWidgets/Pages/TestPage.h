#ifndef WW_PAGEHANDLER_TEST_H
#define WW_PAGEHANDLER_TEST_H

#include "Pages/PageHandler.h"
#include "Application/WApplication.h"
#include "Application/WServer.h"
#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ConfigurationCache.h"

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WPushButton>
#include <Wt/WTemplate>

namespace WW
{
	class Another404Page : public PageHandler
	{
		virtual Wt::WWidget *createContent() override
		{
			return new Wt::WText("!!!NNNNNNNNNNNNNNNNOLA!!!");
		}
	};

	class AnotherPage : public PageHandler
	{
	public:
		AnotherPage()
		{
			set404Page(new Another404Page());
		}
	protected:
		virtual Wt::WWidget *createContent() override
		{
			return new Wt::WText("OLA!!!");
		}
	};

	class TestPage : public PageHandler
	{
	public:
		virtual Wt::WWidget *createContent() override
		{
			//TEST//
			WServer *server = WServer::instance();
			WApplication *app = WApplication::instance();

			Wt::WContainerWidget *root = new Wt::WContainerWidget();

			new Wt::WText("HI", root);
			new Wt::WBreak(root);
			(new Wt::WText("Internal path: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto ip = new Wt::WText(app->internalPath(), root);
			new Wt::WBreak(root);
			(new Wt::WText("Reserved internal path: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto ipr = new Wt::WText(app->reservedInternalPath(), root);
			new Wt::WBreak(root);
			(new Wt::WText("Internal path after reserve(subpath): ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto ipar = new Wt::WText(app->internalPathAfterReserved(), root);
			new Wt::WBreak(root);
			(new Wt::WText("Internal path after reserve next part(Arg '/'): ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto iparnp = new Wt::WText(app->internalPathAfterReservedNextPart("/"), root);
			new Wt::WBreak(root);
			(new Wt::WText("Current language: ", root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto txt = new Wt::WText(app->locale().name(), root);
			app->internalPathChanged().connect(std::bind<void>([ip, ipr]() {
				WApplication *app = WApplication::instance();
				ip->setText(app->internalPath());
				ipr->setText(app->reservedInternalPath());
			}));
			app->internalPathAfterReservedChanged().connect(std::bind<void>([ipar, iparnp]() {
				WApplication *app = WApplication::instance();
				ipar->setText(app->internalPathAfterReserved());
				iparnp->setText(app->internalPathAfterReservedNextPart("/"));
			}));
			app->localeChanged().connect(std::bind<void>([txt]() {
				WApplication *app = WApplication::instance();
				txt->setText(app->locale().name());
			}));
			new Wt::WBreak(root);
			(new Wt::WText(std::string("Session Default Language: ") + app->_sessionDefaultLocale.name(), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			new Wt::WBreak(root);
			(new Wt::WText(std::string("Mobile Version: "), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto mvt = new Wt::WText(std::string(app->isMobileVersion() ? "On" : "Off"), root);
			app->mobileVersionChanged().connect(std::bind<void>([mvt](bool MobileVersion) {
				mvt->setText(MobileVersion ? "On" : "Off");
			}, std::placeholders::_1));
			new Wt::WBreak(root);
			(new Wt::WText(std::string("Current Page: "), root))->decorationStyle().font().setWeight(Wt::WFont::Bold);
			auto cpt = new Wt::WText(Wt::WString(app->currentPage() ? Wt::WString::tr(app->currentPage()->titleKey, app->currentPage()->titleModuleId) : "Invalid Page(404)"), root);
			new Wt::WBreak(root);
			app->pageChanged().connect(std::bind<void>([cpt]() {
				WApplication *app = WApplication::instance();
				cpt->setText(app->currentPage() ? Wt::WString::tr(app->currentPage()->titleKey, app->currentPage()->titleModuleId) : "Invalid Page(404)");
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
				WApplication *app = WApplication::instance();
				new Wt::WText(std::string("[Server Config; Before Reload()]Log Directory: ") + server->configurations()->getStr("LogDirectory", ModuleDatabase::Logging, "./default"), root);
				new Wt::WText(std::string("[Cache Config; Before Reload()]Log Directory: ") + app->configurations()->getStr("LogDirectory", ModuleDatabase::Logging, "./default"), root);
				new Wt::WBreak(root);
				server->databaseManager()->reload();
				new Wt::WText(std::string("[Server Config; AFTER Reload()]Log Directory: ") + server->configurations()->getStr("LogDirectory", ModuleDatabase::Logging, "./default"), root);
				new Wt::WText(std::string("[Cache Config; AFTER Reload()]Log Directory: ") + app->configurations()->getStr("LogDirectory", ModuleDatabase::Logging, "./default"), root);
				new Wt::WBreak(root);
			}));

			Wt::WPushButton *rs = new Wt::WPushButton("Reload Styles", root);
			rs->clicked().connect(std::bind<void>([server]() {
				server->databaseManager()->reload();
			}));
			Wt::WPushButton *cs = new Wt::WPushButton("Change Style", root);
			cs->clicked().connect(std::bind<void>([server]() {
				WApplication *app = WApplication::instance();
				if(!app->currentStyle())
				{
					app->changeStyle("Default", 1);
				}
				else if(app->currentStyle()->name() == "Default")
				{
					app->changeStyle("Test", 1);
				}
				else
				{
					app->changeStyle("Default", 1);
				}
				app->triggerUpdate();
			}));
			new Wt::WBreak(root);
			Wt::WPushButton *rl = new Wt::WPushButton("Reload languages", root);
			rl->clicked().connect(std::bind<void>([server]() {
				server->databaseManager()->reload();
			}));
			new Wt::WBreak(root);
			new Wt::WText(Wt::WString::tr("Wt.Auth.email"), root);
			new Wt::WBreak(root);
			new Wt::WText(Wt::WString::tr("test", ModuleDatabase::Authentication), root);
			new Wt::WBreak(root);
			new Wt::WBreak(root);
			new Wt::WTemplate(Wt::WString::tstr("styletpl", ModuleDatabase::Styling), root);
			new Wt::WTemplate(Wt::WString::tstr("templatetpl", ModuleDatabase::Styling), root);

			return root;
		}
	};
}

#endif
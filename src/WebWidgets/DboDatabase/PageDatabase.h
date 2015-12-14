#ifndef WW_DBODATABASE_PAGE_H
#define WW_DBODATABASE_PAGE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "DboDatabase/ModuleDatabase.h"
#include "Dbo/ModuleTreeDbos.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#define NOTFOUNDPAGENAME "notfound"
#define ERRORPAGENAME "error"

namespace WW
{
	class PageHandler;

	class PageDatabase : public AbstractDboDatabase
	{
	protected:
		struct MetaPage
		{
			MetaPage(Ddo::ptr<Ddo::Page> pagePtr, PageHandler *pageHandler = nullptr)
			: pagePtr(pagePtr), pageHandler(pageHandler)
			{ }

			long long id() const { return pagePtr->id(); }
			std::string name() const { return pagePtr->name(); }
			long long moduleId() const { return pagePtr->moduleId(); }
			bool preload() const { return pagePtr->preload; }

			Ddo::ptr<Ddo::Page> pagePtr;
			PageHandler *pageHandler;
		};

		typedef boost::multi_index_container<
			MetaPage,

			boost::multi_index::indexed_by<
				//Index by page id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ById>,
					boost::multi_index::const_mem_fun<MetaPage, long long, &MetaPage::id>
				>,
				//Index by Name and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ByKey>,
					boost::multi_index::composite_key<
						MetaPage,
						boost::multi_index::const_mem_fun<MetaPage, std::string, &MetaPage::name>,
						boost::multi_index::const_mem_fun<MetaPage, long long, &MetaPage::moduleId>
					>
				>,
				//Index by preload
				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<struct ByPreload>,
					boost::multi_index::const_mem_fun<MetaPage, bool, &MetaPage::preload>
				>
			>
		> PageContainer;

		typedef PageContainer::index<ById>::type PageById;
		typedef PageContainer::index<ByKey>::type PageByKey;
		typedef PageContainer::index<ByPreload>::type PagesByPreload;

	public:
		PageDatabase(DboDatabaseManager *manager);
		virtual ~PageDatabase() override;

		Ddo::cPtr<Ddo::Page> getPtr(long long pageId) const;
		Ddo::cPtr<Ddo::Page> getPtr(const std::string &pageName, long long moduleId) const;
		Ddo::cPtr<Ddo::Page> homePagePtr(const std::string &hostName) const;
		PageHandler *getPage(long long pageId) const;
		PageHandler *getPage(const std::string &pageName, long long moduleId) const;
		PageHandler *get404Page() const { return getPage(NOTFOUNDPAGENAME, ModuleDatabase::Navigation); }
		PageHandler *getErrorPage() const { return getPage(ERRORPAGENAME, ModuleDatabase::Navigation); }
		void visitPreloadPages(std::function<void(Ddo::cPtr<Ddo::Page>)> visitor) const;

		void registerPageHandler(long long pageId, PageHandler *handler);
		void registerPageHandler(const std::string &pageName, long long moduleId, PageHandler *handler);
		void register404PageHandler(PageHandler *handler) { registerReplacablePageHandler(NOTFOUNDPAGENAME, ModuleDatabase::Navigation, handler); }
		void registerErrorPageHandler(PageHandler *handler) { registerReplacablePageHandler(ERRORPAGENAME, ModuleDatabase::Navigation, handler); }

		std::size_t pagesCount() const;
		long long getLoadDurationinMS() const;

		virtual std::string name() const override { return "PagesDatabase"; }

	protected:
		virtual void load(Wt::Dbo::Session &dboSession) override;
		virtual void reload(Wt::Dbo::Session &dboSession) override;
		void fetchAll(Wt::Dbo::Session &dboSession);

		void registerReplacablePageHandler(const std::string &pageName, long long moduleId, PageHandler *handler);

		virtual bool isVital() const override { return true; }

		PageContainer _pageContainer;
		boost::posix_time::time_duration _loadDuration;
	};
}

#endif
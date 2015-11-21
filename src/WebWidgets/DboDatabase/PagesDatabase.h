#ifndef PAGES_DATABASE_H
#define PAGES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Page.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class AbstractPage;

class PagesDatabase : public AbstractDboDatabase
{
	protected:
		struct MetaPage
		{
			MetaPage(std::shared_ptr<PageData> PagePtr, AbstractPage *PageHandler = 0)
				: PagePtr(PagePtr), PageHandler(PageHandler)
			{ }

			std::shared_ptr<PageData> PagePtr;
			AbstractPage *PageHandler;
		};

		struct Page_key_id
		{
			typedef long long result_type;
			result_type operator()(const MetaPage &Page) const
			{
				return Page.PagePtr->id();
			}
		};
		struct Page_key_Name
		{
			typedef std::string result_type;
			result_type operator()(const MetaPage &Page) const
			{
				return Page.PagePtr->Name();
			}
		};
		struct Page_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const MetaPage &Page) const
			{
				return Page.PagePtr->ModuleId();
			}
		};
		struct ById{};
		struct ByKey{};

		typedef boost::multi_index_container<
			MetaPage,

			boost::multi_index::indexed_by<
				//Index by page id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ById>,
					Page_key_id
				>,
				//Index by Name and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByKey>,
					boost::multi_index::composite_key<
						MetaPage,
						Page_key_Name,
						Page_key_ModuleId
					>
				>
			>
		> PageContainers;

		typedef PageContainers::index<ById>::type PageById;
		typedef PageContainers::index<ByKey>::type PageByKey;

	public:
		PagesDatabase(DboDatabaseManager *Manager);

		std::shared_ptr<const PageData> GetPtr(long long PageId) const;
		std::shared_ptr<const PageData> GetPtr(const std::string &PageName, long long ModuleId) const;
		AbstractPage *GetPage(long long PageId) const;
		AbstractPage *GetPage(const std::string &PageName, long long ModuleId) const;
		std::shared_ptr<const PageData> HomePagePtr(const std::string &HostName) const;

		void RegisterPageHandler(long long PageId);
		void RegisterPageHandler(const std::string &PageName, long long ModuleId);

		std::size_t CountPages() const;
		long long GetLoadDurationinMS() const;

		virtual std::string Name() const { return "PagesDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);
		void FetchAll(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		PageContainers PageContainer;
		boost::posix_time::time_duration LoadDuration;
};

#endif
#ifndef PAGES_DATABASE_H
#define PAGES_DATABASE_H

#include "Dbo/Page.h"
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class WServer;

class PagesDatabase
{
	private:
		struct MetaPage
		{
			MetaPage(Wt::Dbo::ptr<Page> PagePtr = Wt::Dbo::ptr<Page>())
				: PagePtr(PagePtr)
			{ }
			MetaPage(Wt::Dbo::ptr<Page> PagePtr, boost::function<void()> Handler)
				: PagePtr(PagePtr), HandlerFunction(Handler)
			{ }

			Wt::Dbo::ptr<Page> PagePtr;
			boost::function<void()> HandlerFunction;
		};

		struct Page_key_id
		{
			typedef long long result_type;
			result_type operator()(const MetaPage &Page) const
			{
				return Page.PagePtr.id().id;
			}
		};
		struct Page_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const MetaPage &Page) const
			{
				return Page.PagePtr.id().ModulePtr.id();
			}
		};
		struct ByCompositeKey{};

		typedef boost::multi_index_container<
			MetaPage,

			boost::multi_index::indexed_by<
				//Index by Page Id and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByCompositeKey>,
					boost::multi_index::composite_key<
						Wt::Dbo::ptr<Page>,
						Page_key_id,
						Page_key_ModuleId
					>
				>
			>
		> PageContainers;
		typedef PageContainers::index<ByCompositeKey>::type PageByCompositeKey;

	public:
		PagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		PagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void Load();
		void Reload();

		Wt::Dbo::ptr<Page> GetPtr(long long PageId, long long ModuleId) const;
		//Wt::Dbo::ptr<Page> GetPtr(const std::string &InternalPath) const;

		void RegisterPageHandler(long long PageId, long long ModuleId, boost::function<void()> Handler);

		std::size_t CountPages() const;
		long long GetLoadDurationinMS() const;

	protected:
		void MapClasses();
		void FetchAll();

		PageContainers PageContainer;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
		mutable boost::shared_mutex mutex;

	private:
		friend class ReadLock;
};

#endif
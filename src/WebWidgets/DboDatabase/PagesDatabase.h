#ifndef PAGES_DATABASE_H
#define PAGES_DATABASE_H

#include "Dbo/Page.h"
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/unordered_map.hpp>

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

		typedef boost::unordered_map<std::pair<long long, long long>, MetaPage> PageMaps;

	public:
		PagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		PagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void Load() { FetchAll(); }
		void Reload();

		Wt::Dbo::ptr<Page> GetPtr(long long PageId, long long ModuleId) const;
		Wt::Dbo::ptr<Page> HomePagePtr() const;
		//Wt::Dbo::ptr<Page> GetPtr(const std::string &InternalPath) const;

		void RegisterPageHandler(long long PageId, long long ModuleId, boost::function<void()> Handler);

		std::size_t CountPages() const;
		long long GetLoadDurationinMS() const;

	protected:
		void MapClasses();
		void FetchAll();

		bool CallPageHandler(long long PageId, long long ModuleId);

		PageMaps PageMap;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
		mutable boost::shared_mutex mutex;

	private:
		friend class ReadLock;
		friend class Application;
};

#endif
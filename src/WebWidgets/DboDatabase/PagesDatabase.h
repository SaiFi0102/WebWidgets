#ifndef PAGES_DATABASE_H
#define PAGES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Page.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

class PagesDatabase : public AbstractDboDatabase
{
	private:
		struct MetaPage
		{
			MetaPage(boost::shared_ptr<PageData> PagePtr = boost::shared_ptr<PageData>())
				: PagePtr(PagePtr)
			{ }
			MetaPage(boost::shared_ptr<PageData> PagePtr, boost::function<void()> Handler)
				: PagePtr(PagePtr), HandlerFunction(Handler)
			{ }

			boost::shared_ptr<PageData> PagePtr;
			boost::function<void()> HandlerFunction;
		};

		typedef boost::unordered_map<std::pair<long long, long long>, MetaPage> PageMaps;

	public:
		PagesDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<PageData> GetPtr(long long PageId, long long ModuleId) const;
		boost::shared_ptr<PageData> HomePagePtr() const;
		//boost::shared_ptr<PageData> GetPtr(const std::string &InternalPath) const;

		void RegisterPageHandler(long long PageId, long long ModuleId, boost::function<void()> Handler);

		std::size_t CountPages() const;
		long long GetLoadDurationinMS() const;

		virtual std::string Name() const { return "PagesDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);
		void FetchAll(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		bool CallPageHandler(long long PageId, long long ModuleId);

		PageMaps PageMap;
		boost::posix_time::time_duration LoadDuration;

	private:
		friend class Application;
};

#endif
#ifndef PAGES_DATABASE_H
#define PAGES_DATABASE_H

#include "Dbo/Page.h"
#include "Application/WServer.h"
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class PagesDatabase
{
	private:
		struct Page_key_id
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<Page> PagePtr) const
			{
				return PagePtr.id().id;
			}
		};
		struct Page_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<Page> PagePtr) const
			{
				return PagePtr.id().ModulePtr.id();
			}
		};
		struct Page_key_InternalPath
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<Page> PagePtr) const
			{
				return PagePtr->InternalPath;
			}
		};
		struct ByCompositeKey{};
		struct ByInternalPath{};

		typedef boost::multi_index_container<
			Wt::Dbo::ptr<Page>,

			boost::multi_index::indexed_by<
				//Index by Page Id and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByCompositeKey>,
					boost::multi_index::composite_key<
						Wt::Dbo::ptr<Page>,
						Page_key_id,
						Page_key_ModuleId
					>
				>,
				//Index by Internal Path string
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByInternalPath>,
					Page_key_InternalPath
				>
			>
		> PageContainers;
		typedef PageContainers::index<ByCompositeKey>::type PageByCompositeKey;
		typedef PageContainers::index<ByInternalPath>::type PageByInternalPath;

	public:
		PagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		PagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll();

		Page GetDbo(long long PageId, long long ModuleId) const;
		Page GetDbo(const std::string &InternalPath) const;

		std::size_t CountPages() const;
		long long GetLoadDurationinMS() const;

	protected:
		virtual void MapClasses();
		Wt::Dbo::ptr<Page> GetPtr(long long PageId, long long ModuleId) const;
		Wt::Dbo::ptr<Page> GetPtr(const std::string &InternalPath) const;

		PageContainers PageContainer;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
		mutable boost::shared_mutex mutex;
};

#endif
#ifndef ACCESSPATH_DATABASE_H
#define ACCESSPATH_DATABASE_H

#include "Dbo/AccessPath.h"
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/ordered_index.hpp>

class WServer;

class AccessPathsDatabase
{
	private:
		struct AccessPath_key_id
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<AccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->id();
			}
		};
		struct AccessPath_key_HostName
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<AccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->HostName;
			}
		};
		struct AccessPath_key_InternalPath
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<AccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->InternalPath;
			}
		};
		struct AccessPath_key_LanguageCode
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<AccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->LanguageCode;
			}
		};
		struct ById{};
		struct ByURL{};
		struct ByLanguageHostname{};

		typedef boost::multi_index_container<
			boost::shared_ptr<AccessPathData>,

			boost::multi_index::indexed_by<
				//Index by access path id
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<ById>,
					AccessPath_key_id
				>,
				//Index by Name and Type
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<ByURL>,
					boost::multi_index::composite_key<
						boost::shared_ptr<AccessPathData>,
						AccessPath_key_HostName,
						AccessPath_key_InternalPath
					>
				>,
				//Index by language code and hostname
				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<ByLanguageHostname>,
					boost::multi_index::composite_key<
						boost::shared_ptr<AccessPathData>,
						AccessPath_key_LanguageCode,
						AccessPath_key_HostName
					>
				>
			>
		> AccessPathContainers;
		typedef AccessPathContainers::index<ById>::type AccessPathById;
		typedef AccessPathContainers::index<ByURL>::type AccessPathByURL;
		typedef AccessPathContainers::index<ByLanguageHostname>::type AccessPathByLanguageHostname;

	public:
		AccessPathsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		AccessPathsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void Load() { FetchAll(); }
		void Reload() { FetchAll(); }

		boost::shared_ptr<AccessPathData> GetPtr(long long Id) const;
		boost::shared_ptr<AccessPathData> GetPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<AccessPathData> LanguageAccessPathPtr(long long Id) const;
		boost::shared_ptr<AccessPathData> LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<AccessPathData> PageAccessPathPtr(long long Id) const;
		boost::shared_ptr<AccessPathData> PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<AccessPathData> HomePageAccessPathPtr() const;

		std::string FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const;
		bool AccessPathExists(long long Id) const;
		bool AccessPathExists(const std::string &HostName, const std::string &InternalPath) const;
		bool LanguageAccessPathExists(long long Id) const;
		bool LanguageAccessPathExists(const std::string &HostName, const std::string &InternalPath) const;

		std::size_t CountAccessPaths() const;
		long long GetLoadDurationinMS() const;

	protected:
		void MapClasses();
		void FetchAll();

		AccessPathContainers AccessPathContainer;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
		mutable boost::shared_mutex mutex;

	private:
		friend class Application;
		friend class ReadLock;
};

#endif
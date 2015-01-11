#ifndef ACCESSPATH_DATABASE_H
#define ACCESSPATH_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/AccessPath.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/ordered_index.hpp>

class AccessPathsDatabase : public AbstractDboDatabase
{
	protected:
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
		AccessPathsDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const AccessPathData> GetPtr(long long Id) const;
		boost::shared_ptr<const AccessPathData> GetPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<const AccessPathData> LanguageAccessPathPtr(long long Id) const;
		boost::shared_ptr<const AccessPathData> LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<const AccessPathData> PageAccessPathPtr(long long Id) const;
		boost::shared_ptr<const AccessPathData> PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const;
		boost::shared_ptr<const AccessPathData> HomePageAccessPathPtr() const;

		std::string FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const;
		bool AccessPathExists(long long Id) const;
		bool AccessPathExists(const std::string &HostName, const std::string &InternalPath) const;
		bool LanguageAccessPathExists(long long Id) const;
		bool LanguageAccessPathExists(const std::string &HostName, const std::string &InternalPath) const;

		std::size_t CountAccessPaths() const;
		long long GetLoadDurationinMS() const;

		virtual std::string Name() const { return "AccessPathsDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession) { FetchAll(DboSession); }
		void FetchAll(Wt::Dbo::Session &DboSession);

		AccessPathContainers AccessPathContainer;
		boost::posix_time::time_duration LoadDuration;

		virtual bool IsVital() const { return true; }

	private:
		friend class Application;
};

#endif
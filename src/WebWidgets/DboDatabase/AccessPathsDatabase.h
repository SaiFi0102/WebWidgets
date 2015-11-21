#ifndef ACCESSPATH_DATABASE_H
#define ACCESSPATH_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/AccessPath.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

class AccessPathsDatabase : public AbstractDboDatabase
{
	protected:
		struct AccessHostName_key_HostName
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<AccessHostNameData> &AccessHostNamePtr) const
			{
				return AccessHostNamePtr->HostName;
			}
		};
		struct PageAccessPath_key_id
		{
			typedef long long result_type;
			result_type operator()(const std::shared_ptr<PageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->id();
			}
		};
		struct LanguageAccessPath_key_id
		{
			typedef long long result_type;
			result_type operator()(const std::shared_ptr<LanguageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->id();
			}
		};
		struct PageAccessPath_key_HostName
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<PageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->HostName;
			}
		};
		struct LanguageAccessPath_key_HostName
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<LanguageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->HostName;
			}
		};
		struct PageAccessPath_key_InternalPath
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<PageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->InternalPath;
			}
		};
		struct PageAccessPath_key_ParentAccessPath
		{
			typedef long long result_type;
			result_type operator()(const std::shared_ptr<PageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->ParentAccessPathId;
			}
		};
		struct LanguageAccessPath_key_InternalPath
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<LanguageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->InternalPath;
			}
		};
		struct LanguageAccessPath_key_LanguageCode
		{
			typedef std::string result_type;
			result_type operator()(const std::shared_ptr<LanguageAccessPathData> &AccessPathPtr) const
			{
				return AccessPathPtr->LanguageCode;
			}
		};
		struct ById{};
		struct ByURL{};
		struct ByLanguageHostname{};

		//AccessHostName container
		typedef boost::multi_index_container<
			std::shared_ptr<AccessHostNameData>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<AccessHostName_key_HostName>
			>
		> AccessHostNameContainers;

		//PageAccessPath container
		typedef boost::multi_index_container<
			std::shared_ptr<PageAccessPathData>,

			boost::multi_index::indexed_by<
				//Index by access path id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ById>,
					PageAccessPath_key_id
				>,
				//Index by HostName and InternalPath
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByURL>,
					boost::multi_index::composite_key<
						std::shared_ptr<PageAccessPathData>,
						PageAccessPath_key_HostName,
						PageAccessPath_key_InternalPath,
						PageAccessPath_key_ParentAccessPath
					>
				>
			>
		> PageAccessPathContainers;

		//LanguageAccessPath container
		typedef boost::multi_index_container<
			std::shared_ptr<LanguageAccessPathData>,

			boost::multi_index::indexed_by<
				//Index by access path id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ById>,
					LanguageAccessPath_key_id
				>,
				//Index by HostName and InternalPath
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByURL>,
					boost::multi_index::composite_key<
						std::shared_ptr<LanguageAccessPathData>,
						LanguageAccessPath_key_HostName,
						LanguageAccessPath_key_InternalPath
					>
				>,
				//Index by LanguageCode and HostName(for getting internal path)
				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<ByLanguageHostname>,
					boost::multi_index::composite_key<
						std::shared_ptr<LanguageAccessPathData>,
						LanguageAccessPath_key_LanguageCode,
						LanguageAccessPath_key_HostName
					>
				>
			>
		> LanguageAccessPathContainers;

		typedef AccessHostNameContainers::nth_index<0>::type AccessHostNameType;
		typedef PageAccessPathContainers::index<ById>::type PageAccessPathById;
		typedef PageAccessPathContainers::index<ByURL>::type PageAccessPathByURL;
		typedef LanguageAccessPathContainers::index<ById>::type LanguageAccessPathById;
		typedef LanguageAccessPathContainers::index<ByURL>::type LanguageAccessPathByURL;
		typedef LanguageAccessPathContainers::index<ByLanguageHostname>::type LanguageAccessPathByLH;

	public:
		AccessPathsDatabase(DboDatabaseManager *Manager);

		std::shared_ptr<const AccessHostNameData> AccessHostNamePtr(const std::string &HostName) const;
		std::shared_ptr<const AccessHostNameData> AccessHostOrGlobalPtr(const std::string &HostName) const;
		std::shared_ptr<const LanguageAccessPathData> LanguageAccessPathPtr(long long Id) const;
		std::shared_ptr<const LanguageAccessPathData> LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const;
		std::shared_ptr<const PageAccessPathData> PageAccessPathPtr(long long Id) const;
		std::shared_ptr<const PageAccessPathData> PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath, long long ParentAccessPathId = -1) const;

		std::string FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const;

		std::size_t CountAccessHostNames() const;
		std::size_t CountPageAccessPaths() const;
		std::size_t CountLanguageAccessPaths() const;
		long long GetLoadDurationinMS() const;

		virtual std::string Name() const { return "AccessPathsDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession) { FetchAll(DboSession); }
		void FetchAll(Wt::Dbo::Session &DboSession);

		AccessHostNameContainers AccessHostNameContainer;
		PageAccessPathContainers PageAccessPathContainer;
		LanguageAccessPathContainers LanguageAccessPathContainer;
		boost::posix_time::time_duration LoadDuration;

		virtual bool IsVital() const { return true; }

	private:
		friend class Application;
};

#endif
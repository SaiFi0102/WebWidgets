#ifndef WW_DBODATABASE_ACCESSPATH_H
#define WW_DBODATABASE_ACCESSPATH_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace WW
{
	class AccessPathDatabase : public AbstractDboDatabase
	{
	protected:
		//AccessHostName container
		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::AccessHostName>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::member<Ddo::AccessHostName, std::string, &Ddo::AccessHostName::hostName>
				>
			>
		> AccessHostNameContainer;

		//PageAccessPath container
		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::PageAccessPath>,

			boost::multi_index::indexed_by<
				//Index by access path id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ById>,
					boost::multi_index::const_mem_fun<Ddo::SurrogateKey, long long, &Ddo::PageAccessPath::id>
				>,
				//Index by HostName and InternalPath
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ByURL>,
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::PageAccessPath>,
						boost::multi_index::member<Ddo::PageAccessPath, std::string, &Ddo::PageAccessPath::hostName>,
						boost::multi_index::member<Ddo::PageAccessPath, std::string, &Ddo::PageAccessPath::internalPath>,
						boost::multi_index::member<Ddo::PageAccessPath, long long, &Ddo::PageAccessPath::parentAccessPathId>
					>
				>,
				//Index by HostName and Page Id
				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<struct ByPage>,
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::PageAccessPath>,
						boost::multi_index::member<Ddo::PageAccessPath, long long, &Ddo::PageAccessPath::pageId>,
						boost::multi_index::member<Ddo::PageAccessPath, std::string, &Ddo::PageAccessPath::hostName>
					>
				>
			>
		> PageAccessPathContainer;

		//LanguageAccessPath container
		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::LanguageAccessPath>,

			boost::multi_index::indexed_by<
				//Index by access path id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ById>,
					boost::multi_index::const_mem_fun<Ddo::SurrogateKey, long long, &Ddo::LanguageAccessPath::id>
				>,
				//Index by HostName and InternalPath
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByURL>,
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::LanguageAccessPath>,
						boost::multi_index::member<Ddo::LanguageAccessPath, std::string, &Ddo::LanguageAccessPath::hostName>,
						boost::multi_index::member<Ddo::LanguageAccessPath, std::string, &Ddo::LanguageAccessPath::internalPath>
					>
				>,
				//Index by LanguageCode and HostName(for getting internal path)
				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<struct ByLanguageHostname>,
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::LanguageAccessPath>,
						boost::multi_index::member<Ddo::LanguageAccessPath, std::string, &Ddo::LanguageAccessPath::languageCode>,
						boost::multi_index::member<Ddo::LanguageAccessPath, std::string, &Ddo::LanguageAccessPath::hostName>
					>
				>
			>
		> LanguageAccessPathContainer;

		typedef AccessHostNameContainer::nth_index<0>::type AccessHostNameType;
		typedef PageAccessPathContainer::index<ById>::type PageAccessPathById;
		typedef PageAccessPathContainer::index<ByURL>::type PageAccessPathByURL;
		typedef PageAccessPathContainer::index<ByPage>::type PageAccessPathByPage;
		typedef LanguageAccessPathContainer::index<ById>::type LanguageAccessPathById;
		typedef LanguageAccessPathContainer::index<ByURL>::type LanguageAccessPathByURL;
		typedef LanguageAccessPathContainer::index<ByLanguageHostname>::type LanguageAccessPathByLH;

	public:
		AccessPathDatabase(DboDatabaseManager *Manager);

		Ddo::cPtr<Ddo::AccessHostName> accessHostNamePtr(const std::string &hostName) const;
		Ddo::cPtr<Ddo::AccessHostName> accessHostOrGlobalPtr(const std::string &hostName) const;
		Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPathPtr(long long Id) const;
		Ddo::cPtr<Ddo::LanguageAccessPath> languageAccessPathPtr(const std::string &hostName, const std::string &internalPath) const;
		Ddo::cPtr<Ddo::PageAccessPath> pageAccessPathPtr(long long Id) const;
		Ddo::cPtr<Ddo::PageAccessPath> pageAccessPathPtr(const std::string &hostName, const std::string &internalPath, long long parentAccessPathId = -1) const;

		std::string firstLanguagePath(const std::string &languageCode, const std::string &hostName, bool languageFromHostname) const;
		std::string firstPagePath(long long pageId, const std::string &hostName) const;

		std::size_t accessHostNamesCount() const;
		std::size_t pageAccessPathsCount() const;
		std::size_t languageAccessPathsCount() const;
		long long getLoadDurationinMS() const;

		virtual std::string name() const override { return "AccessPathsDatabase"; }

	protected:
		virtual void load(Wt::Dbo::Session &dboSession) override;
		virtual void reload(Wt::Dbo::Session &dboSession) override { fetchAll(dboSession); }
		void fetchAll(Wt::Dbo::Session &dboSession);

		AccessHostNameContainer _accessHostNameContainer;
		PageAccessPathContainer _pageAccessPathContainer;
		LanguageAccessPathContainer _languageAccessPathContainer;
		boost::posix_time::time_duration _loadDuration;

		virtual bool isVital() const override { return true; }

	private:
		friend class WApplication;
	};
}

#endif

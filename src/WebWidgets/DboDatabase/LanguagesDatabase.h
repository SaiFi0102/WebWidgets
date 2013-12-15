#ifndef LANGUAGES_DATABASE_H
#define LANGUAGES_DATABASE_H

#include <Wt/WLocale>
#include "Dbo/Language.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "Application/WServer.h"
#include <boost/thread.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class LanguagesDatabase
{
	private:
		//Language container key extractors
		struct Language_key_Code
		{
			typedef std::string result_type; 
			result_type operator()(const Wt::Dbo::ptr<Language> LanguagePtr) const
			{
				return LanguagePtr.id();
			}
		};
		struct Language_key_LanguageAccept
		{
			typedef std::string result_type; 
			result_type operator()(const Wt::Dbo::ptr<Language> LanguagePtr) const
			{
				return LanguagePtr->LanguageAccept;
			}
		};

		//LanguageSingle key extractors
		struct LanguageSingle_key_Code
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguageSingle> LanguageSinglePtr) const
			{
				return LanguageSinglePtr.id().LanguagePtr.id();
			}
		};
		struct LanguageSingle_key_Key
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguageSingle> LanguageSinglePtr) const
			{
				return LanguageSinglePtr.id().Key;
			}
		};
		struct LanguageSingle_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguageSingle> LanguageSinglePtr) const
			{
				return LanguageSinglePtr.id().ModulePtr.id();
			}
		};

		//LanguagePlural key extractors
		struct LanguagePlural_key_Code
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguagePlural> LanguagePluralPtr) const
			{
				return LanguagePluralPtr.id().LanguagePtr.id();
			}
		};
		struct LanguagePlural_key_Key
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguagePlural> LanguagePluralPtr) const
			{
				return LanguagePluralPtr.id().Key;
			}
		};
		struct LanguagePlural_key_Case
		{
			typedef int result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguagePlural> LanguagePluralPtr) const
			{
				return LanguagePluralPtr.id().Case;
			}
		};
		struct LanguagePlural_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<LanguagePlural> LanguagePluralPtr) const
			{
				return LanguagePluralPtr.id().ModulePtr.id();
			}
		};

		//multi_index containers
		typedef boost::multi_index_container<
			Wt::Dbo::ptr<LanguageSingle>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Wt::Dbo::ptr<LanguageSingle>,
						LanguageSingle_key_Code,
						LanguageSingle_key_Key,
						LanguageSingle_key_ModuleId
					>
				>
			>
		> LanguageSingleContainers;

		typedef boost::multi_index_container<
			Wt::Dbo::ptr<LanguagePlural>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Wt::Dbo::ptr<LanguagePlural>,
						LanguagePlural_key_Code,
						LanguagePlural_key_Key,
						LanguagePlural_key_Case,
						LanguagePlural_key_ModuleId
					>
				>
			>
		> LanguagePluralContainers;

		typedef boost::multi_index_container<
			Wt::Dbo::ptr<Language>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<Language_key_Code>, //index by code
				boost::multi_index::hashed_unique<Language_key_LanguageAccept> //index by Language-Accept
			>
		> LanguageContainers;

		typedef LanguageContainers::nth_index<0>::type LanguageByCode;
		typedef LanguageContainers::nth_index<1>::type LanguageByLanguageAccept;
		typedef LanguageSingleContainers::nth_index<0>::type LanguageSingleType;
		typedef LanguagePluralContainers::nth_index<0>::type LanguagePluralType;
		
	public:
		LanguagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		LanguagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll();

		Language GetLanguageDboFromCode(const std::string &Code) const;
		Language GetLanguageDboFromLanguageAccept(const std::string &LanguageAccept) const;
		LanguageSingle GetSingleDbo(const std::string &Code, const std::string &Key, long long ModuleId) const;
		LanguagePlural GetPluralDbo(const std::string &Code, const std::string &Key, long long ModuleId, int Case) const;

		bool LanguageCodeExists(const std::string &Code) const;
		bool LanguageAcceptExists(const std::string &LanguageAccept) const;
		bool GetPluralExpression(const std::string &Code, std::string &Result) const;
		bool GetSingleString(const std::string &Code, const std::string &Key, long long ModuleId, std::string &Result) const;
		bool GetPluralString(const std::string &Code, const std::string &Key, long long ModuleId, int Case, std::string &Result) const;

		Wt::WLocale GetLocaleFromCode(const std::string &Code) const;
		Wt::WLocale GetLocaleFromLanguageAccept(const std::string &LanguageAccept) const;

		long long GetLoadDurationinMS() const;
		int CountSingle() const;
		int CountPlural() const;
		int CountLanguages() const;

	protected:
		virtual void MapClasses();
		Wt::Dbo::ptr<Language> GetLanguagePtrFromCode(const std::string &Code) const;
		Wt::Dbo::ptr<Language> GetLanguagePtrFromLanguageAccept(const std::string &LanguageAccept) const;
		Wt::Dbo::ptr<LanguageSingle> GetSinglePtr(const std::string &Code, const std::string &Key, long long ModuleId) const;
		Wt::Dbo::ptr<LanguagePlural> GetPluralPtr(const std::string &Code, const std::string &Key, long long ModuleId, int Case) const;

		Wt::Dbo::Session DboSession;
		WServer &_Server;
		LanguageContainers LanguageContainer;
		LanguageSingleContainers LanguageSingleContainer;
		LanguagePluralContainers LanguagePluralContainer;
		boost::posix_time::time_duration LoadDuration;
		mutable boost::shared_mutex mutex;
};

#endif
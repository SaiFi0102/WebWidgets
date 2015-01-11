#ifndef LANGUAGES_DATABASE_H
#define LANGUAGES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Language.h"
#include <Wt/WLocale>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class LanguagesDatabase : public AbstractDboDatabase
{
	protected:
		//Language container key extractors
		struct Language_key_Code
		{
			typedef std::string result_type; 
			result_type operator()(const boost::shared_ptr<LanguageData> &LanguagePtr) const
			{
				return LanguagePtr->Code();
			}
		};
		struct Language_key_LanguageAccept
		{
			typedef std::string result_type; 
			result_type operator()(const boost::shared_ptr<LanguageData> &LanguagePtr) const
			{
				return LanguagePtr->LanguageAccept;
			}
		};

		//LanguageSingle key extractors
		struct LanguageSingle_key_Code
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<LanguageSingleData> &LanguageSinglePtr) const
			{
				return LanguageSinglePtr->LanguageCode();
			}
		};
		struct LanguageSingle_key_Key
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<LanguageSingleData> &LanguageSinglePtr) const
			{
				return LanguageSinglePtr->Key();
			}
		};
		struct LanguageSingle_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<LanguageSingleData> &LanguageSinglePtr) const
			{
				return LanguageSinglePtr->ModuleId();
			}
		};

		//LanguagePlural key extractors
		struct LanguagePlural_key_Code
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<LanguagePluralData> &LanguagePluralPtr) const
			{
				return LanguagePluralPtr->LanguageCode();
			}
		};
		struct LanguagePlural_key_Key
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<LanguagePluralData> &LanguagePluralPtr) const
			{
				return LanguagePluralPtr->Key();
			}
		};
		struct LanguagePlural_key_Case
		{
			typedef int result_type;
			result_type operator()(const boost::shared_ptr<LanguagePluralData> &LanguagePluralPtr) const
			{
				return LanguagePluralPtr->Case();
			}
		};
		struct LanguagePlural_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<LanguagePluralData> &LanguagePluralPtr) const
			{
				return LanguagePluralPtr->ModuleId();
			}
		};

		//multi_index containers
		typedef boost::multi_index_container<
			boost::shared_ptr<LanguageSingleData>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						boost::shared_ptr<LanguageSingleData>,
						LanguageSingle_key_Code,
						LanguageSingle_key_Key,
						LanguageSingle_key_ModuleId
					>
				>
			>
		> LanguageSingleContainers;

		typedef boost::multi_index_container<
			boost::shared_ptr<LanguagePluralData>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						boost::shared_ptr<LanguagePluralData>,
						LanguagePlural_key_Code,
						LanguagePlural_key_Key,
						LanguagePlural_key_Case,
						LanguagePlural_key_ModuleId
					>
				>
			>
		> LanguagePluralContainers;

		typedef boost::multi_index_container<
			boost::shared_ptr<LanguageData>,
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
		LanguagesDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const LanguageData> GetLanguagePtrFromCode(const std::string &Code) const;
		boost::shared_ptr<const LanguageData> GetLanguagePtrFromLanguageAccept(const std::string &LanguageAccept) const;
		boost::shared_ptr<const LanguageSingleData> GetSinglePtr(const std::string &Code, const std::string &Key, long long ModuleId) const;
		boost::shared_ptr<const LanguagePluralData> GetPluralPtr(const std::string &Code, const std::string &Key, long long ModuleId, int Case) const;

		bool LanguageCodeExists(const std::string &Code) const;
		bool LanguageAcceptExists(const std::string &LanguageAccept) const;
		bool GetPluralExpression(const std::string &Code, std::string &Result) const;
		bool GetSingleString(const std::string &Code, const std::string &Key, long long ModuleId, std::string &Result) const;
		bool GetPluralString(const std::string &Code, const std::string &Key, long long ModuleId, int Case, std::string &Result) const;

		Wt::WLocale GetLocaleFromCode(const std::string &Code) const;
		Wt::WLocale GetLocaleFromLanguageAccept(const std::string &LanguageAccept) const
		{
			return GetLocaleFromCode(GetLanguagePtrFromLanguageAccept(LanguageAccept)->Code());
		}

		long long GetLoadDurationinMS() const;
		std::size_t CountSingle() const;
		std::size_t CountPlural() const;
		std::size_t CountLanguages() const;

		virtual std::string Name() const { return "LanguagesDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);
		void FetchAll(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		LanguageContainers LanguageContainer;
		LanguageSingleContainers LanguageSingleContainer;
		LanguagePluralContainers LanguagePluralContainer;
		boost::posix_time::time_duration LoadDuration;
};

#endif
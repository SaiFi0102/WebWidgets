#ifndef WW_DBODATABASE_LANGUAGE_H
#define WW_DBODATABASE_LANGUAGE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"
#include <Wt/WLocale>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace WW
{
	class LanguageDatabase : public AbstractDboDatabase
	{
	protected:
		//multi_index containers
		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::SingularString>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::SingularString>,
						boost::multi_index::const_mem_fun<Ddo::SingularString, std::string, &Ddo::SingularString::languageCode>,
						boost::multi_index::const_mem_fun<Ddo::SingularString, std::string, &Ddo::SingularString::key>,
						boost::multi_index::const_mem_fun<Ddo::SingularString, long long, &Ddo::SingularString::moduleId>
					>
				>
			>
		> LanguageSingleContainer;

		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::PluralString>,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::PluralString>,
						boost::multi_index::const_mem_fun<Ddo::PluralString, std::string, &Ddo::PluralString::languageCode>,
						boost::multi_index::const_mem_fun<Ddo::PluralString, std::string, &Ddo::PluralString::key>,
						boost::multi_index::const_mem_fun<Ddo::PluralString, int, &Ddo::PluralString::Case>,
						boost::multi_index::const_mem_fun<Ddo::PluralString, long long, &Ddo::PluralString::moduleId>
					>
				>
			>
		> LanguagePluralContainer;

		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::Language>,
			boost::multi_index::indexed_by<
				//index by code
				boost::multi_index::hashed_unique<
				boost::multi_index::const_mem_fun<Dbo::BaseLanguage, std::string, &Ddo::Language::code>
				>,
				//index by Language-Accept
				boost::multi_index::hashed_unique<
				boost::multi_index::member<Dbo::BaseLanguage, std::string, &Ddo::Language::languageAccept>
				>
			>
		> LanguageContainer;

		typedef LanguageContainer::nth_index<0>::type LanguageByCode;
		typedef LanguageContainer::nth_index<1>::type LanguageByLanguageAccept;
		typedef LanguageSingleContainer::nth_index<0>::type LanguageSingleType;
		typedef LanguagePluralContainer::nth_index<0>::type LanguagePluralType;
		
	public:
		LanguageDatabase(DboDatabaseManager *manager);

		Ddo::cPtr<Ddo::Language> getLanguagePtrFromCode(const std::string &code) const;
		Ddo::cPtr<Ddo::Language> getLanguagePtrFromLanguageAccept(const std::string &languageAccept) const;
		Ddo::cPtr<Ddo::SingularString> getSinglePtr(const std::string &code, const std::string &key, long long moduleId) const;
		Ddo::cPtr<Ddo::PluralString> getPluralPtr(const std::string &code, const std::string &key, long long moduleId, int Case) const;

		std::string defaultLanguageCode(const std::string &hostName) const;

		bool languageCodeExists(const std::string &code) const;
		bool languageAcceptExists(const std::string &languageAccept) const;
		bool getPluralExpression(const std::string &code, std::string &result) const;
		bool getSingularString(const std::string &code, const std::string &key, long long moduleId, std::string &result) const;
		bool getPluralString(const std::string &code, const std::string &key, long long moduleId, int Case, std::string &result) const;

		Wt::WLocale getLocaleFromCode(const std::string &code, const std::string &hostName) const;
		Wt::WLocale getLocaleFromLanguageAccept(const std::string &languageAccept, const std::string &hostName) const
		{
			return getLocaleFromCode(getLanguagePtrFromLanguageAccept(languageAccept)->code(), hostName);
		}

		long long getLoadDurationinMS() const;
		std::size_t singularsCount() const;
		std::size_t pluralsCount() const;
		std::size_t languagesCount() const;

		virtual std::string name() const override { return "LanguagesDatabase"; }

	protected:
		virtual void load(Wt::Dbo::Session &dboSession) override;
		virtual void reload(Wt::Dbo::Session &dboSession) override;
		void FetchAll(Wt::Dbo::Session &dboSession);

		virtual bool isVital() const override { return true; }

		LanguageContainer _languageContainer;
		LanguageSingleContainer _languageSingleContainer;
		LanguagePluralContainer _languagePluralContainer;
		boost::posix_time::time_duration _loadDuration;
	};
}

#endif
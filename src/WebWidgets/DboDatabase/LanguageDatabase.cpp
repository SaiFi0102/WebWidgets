#include "DboDatabase/LanguageDatabase.h"
#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"

namespace WW
{

LanguageDatabase::LanguageDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

void LanguageDatabase::FetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary objects first
	LanguageContainer languagecontainer;
	LanguageSingleContainer languagesinglecontainer;
	LanguagePluralContainer languagepluralcontainer;

	Wt::Dbo::Transaction transaction(dboSession);
	Dbo::LanguageCollection languageCollection = dboSession.find<Dbo::Language>().where("\"installed\" = 1");

	//All languages
	for(Dbo::ptr<Dbo::Language> &languagePtr : languageCollection)
	{
		//Fetch em all
		Dbo::SingularStringCollection languageSingularCollection = languagePtr->singularStringCollection;
		Dbo::PluralStringCollection languagePluralCollection = languagePtr->pluralStringCollection;

		//Insert language ptr
		languagecontainer.get<0>().insert(Ddo::ptr<Ddo::Language>(new Ddo::Language(languagePtr)));

		//All single strings of this language
		for(Dbo::ptr<Dbo::SingularString> &ptr : languageSingularCollection)
		{
			languagesinglecontainer.get<0>().insert(Ddo::ptr<Ddo::SingularString>(new Ddo::SingularString(ptr)));
		}

		//All plural strings of this language
		for(Dbo::ptr<Dbo::PluralString> &ptr : languagePluralCollection)
		{
			languagepluralcontainer.get<0>().insert(Ddo::ptr<Ddo::PluralString>(new Ddo::PluralString(ptr)));
		}
	}

	transaction.commit();
	_languageContainer.swap(languagecontainer);
	_languageSingleContainer.swap(languagesinglecontainer);
	_languagePluralContainer.swap(languagepluralcontainer);

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = boost::posix_time::time_duration(ptEnd - ptStart);

	if(_languageContainer.get<0>().find("en") == _languageContainer.get<0>().end())
		Wt::log("warn") << name() << ": English language was not found or installed";

	Wt::log("info") << name() << ": " << _languageSingleContainer.size() << " Single and "
		<< _languagePluralContainer.size() << " Plural entries from " << _languageContainer.size()
		<< " Languages successfully loaded in " << _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::Language> LanguageDatabase::getLanguagePtrFromCode(const std::string &code) const
{
	ReadLock lock(manager());
	LanguageByCode::const_iterator itr = _languageContainer.get<0>().find(code);
	if(itr == _languageContainer.get<0>().end())
		return Ddo::cPtr<Ddo::Language>();

	return *itr;
}

Ddo::cPtr<Ddo::Language> LanguageDatabase::getLanguagePtrFromLanguageAccept(const std::string &languageAccept) const
{
	ReadLock lock(manager());
	LanguageByLanguageAccept::const_iterator itr = _languageContainer.get<1>().find(languageAccept);
	if(itr == _languageContainer.get<1>().end())
		return Ddo::cPtr<Ddo::Language>();

	return *itr;
}

Ddo::cPtr<Ddo::SingularString> LanguageDatabase::getSinglePtr(const std::string &code, const std::string &key, long long moduleId) const
{
	ReadLock lock(manager());
	LanguageSingleType::const_iterator itr = _languageSingleContainer.get<0>().find(boost::make_tuple(code, key, moduleId));
	if(itr == _languageSingleContainer.get<0>().end())
		return Ddo::cPtr<Ddo::SingularString>();

	return *itr;
}

Ddo::cPtr<Ddo::PluralString> LanguageDatabase::getPluralPtr(const std::string &code, const std::string &key, long long moduleId, int Case) const
{
	ReadLock lock(manager());
	LanguagePluralType::const_iterator itr = _languagePluralContainer.get<0>().find(boost::make_tuple(code, key, Case, moduleId));
	if(itr == _languagePluralContainer.get<0>().end())
		return Ddo::cPtr<Ddo::PluralString>();

	return *itr;
}

std::string LanguageDatabase::defaultLanguageCode(const std::string &hostName) const
{
	ReadLock lock(manager());
	Ddo::cPtr<Ddo::AccessHostName> accessHostNamePtr = server()->accessPaths()->accessHostOrGlobalPtr(hostName);
	if(accessHostNamePtr)
	{
		if(!accessHostNamePtr->languageCode.empty())
			return accessHostNamePtr->languageCode;
		else
			return "en";
	}
	return "en";
}

bool LanguageDatabase::languageCodeExists(const std::string &code) const
{
	ReadLock lock(manager());
	LanguageByCode::const_iterator itr = _languageContainer.get<0>().find(code);
	return itr != _languageContainer.get<0>().end();
}
bool LanguageDatabase::languageAcceptExists(const std::string &languageAccept) const
{
	ReadLock lock(manager());
	LanguageByLanguageAccept::const_iterator itr = _languageContainer.get<1>().find(languageAccept);
	return itr != _languageContainer.get<1>().end();
}

bool LanguageDatabase::getPluralExpression(const std::string &code, std::string &result) const
{
	Ddo::cPtr<Ddo::Language> languagePtr = getLanguagePtrFromCode(code);
	if(!languagePtr)
		return false;

	result = languagePtr->pluralExpression;
	return true;
}

bool LanguageDatabase::getSingularString(const std::string &code, const std::string &key, long long moduleId, std::string &result) const
{
	Ddo::cPtr<Ddo::SingularString> languageSingularPtr = getSinglePtr(code, key, moduleId);
	if(!languageSingularPtr)
		return false;

	result = languageSingularPtr->string;
	return true;
}

bool LanguageDatabase::getPluralString(const std::string &code, const std::string &key, long long moduleId, int Case, std::string &result) const
{
	Ddo::cPtr<Ddo::PluralString> languagePluralPtr = getPluralPtr(code, key, moduleId, Case);
	if(!languagePluralPtr)
	{
		return false;
	}
	result = languagePluralPtr->string;
	return true;
}

Wt::WLocale LanguageDatabase::getLocaleFromCode(const std::string &code, const std::string &hostName) const
{
	ReadLock lock(manager());

	Wt::WLocale locale(code);
	std::string defaultLanguage = defaultLanguageCode(hostName);
	std::string decimalPointCharacter = ".";
	std::string numberThousandSeparator = ",";
	std::string dateFormat = "MMMM dd, yyyy";

	if(!getSingularString(code, "DecimalPointCharacter", ModuleDatabase::Localization, decimalPointCharacter))
		getSingularString(defaultLanguage, "DecimalPointCharacter", ModuleDatabase::Localization, decimalPointCharacter);

	if(!getSingularString(code, "NumberThousandSeparator", ModuleDatabase::Localization, numberThousandSeparator))
		getSingularString(defaultLanguage, "NumberThousandSeparator", ModuleDatabase::Localization, numberThousandSeparator);

	if(!getSingularString(code, "DateFormat", ModuleDatabase::Localization, dateFormat))
		getSingularString(defaultLanguage, "DateFormat", ModuleDatabase::Localization, dateFormat);

	locale.setDecimalPoint(decimalPointCharacter);
	locale.setGroupSeparator(numberThousandSeparator);
	locale.setDateFormat(dateFormat);
	return locale;
}

long long LanguageDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}
std::size_t LanguageDatabase::singularsCount() const
{
	ReadLock lock(manager());
	return _languageSingleContainer.size();
}
std::size_t LanguageDatabase::pluralsCount() const
{
	ReadLock lock(manager());
	return _languagePluralContainer.size();
}
std::size_t LanguageDatabase::languagesCount() const
{
	ReadLock lock(manager());
	return _languageContainer.size();
}

void LanguageDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	FetchAll(dboSession);
}
void LanguageDatabase::reload(Wt::Dbo::Session &dboSession)
{
	FetchAll(dboSession);
}

}

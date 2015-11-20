#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"

LanguagesDatabase::LanguagesDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager)
{ }

void LanguagesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary objects first
	LanguageContainers languagecontainer;
	LanguageSingleContainers languagesinglecontainer;
	LanguagePluralContainers languagepluralcontainer;

	Wt::Dbo::Transaction transaction(DboSession);
	LanguageCollections LanguageCollection = DboSession.find<Language>().where("\"Installed\" = 1");

	//All languages
	for(LanguageCollections::const_iterator itr = LanguageCollection.begin();
		itr != LanguageCollection.end();
		++itr)
	{
		//Fetch em all
		SingularStringCollections LanguageSingleCollection = DboSession.find<SingularString>().where("\"Language_Code\" = ?").bind((*itr)->Code());
		PluralStringCollections LanguagePluralCollection = DboSession.find<PluralString>().where("\"Language_Code\" = ?").bind((*itr)->Code());

		//Insert language ptr
		languagecontainer.get<0>().insert(boost::shared_ptr<LanguageData>(new LanguageData(*itr)));

		//All single strings of this language
		for(SingularStringCollections::const_iterator i = LanguageSingleCollection.begin();
			i != LanguageSingleCollection.end();
			++i)
		{
			languagesinglecontainer.get<0>().insert(boost::shared_ptr<SingularStringData>(new SingularStringData(*i)));
		}

		//All plural strings of this language
		for(PluralStringCollections::const_iterator i = LanguagePluralCollection.begin();
			i != LanguagePluralCollection.end();
			++i)
		{
			languagepluralcontainer.get<0>().insert(boost::shared_ptr<PluralStringData>(new PluralStringData(*i)));
		}
	}

	transaction.commit();
	LanguageContainer.swap(languagecontainer);
	LanguageSingleContainer.swap(languagesinglecontainer);
	LanguagePluralContainer.swap(languagepluralcontainer);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = boost::posix_time::time_duration(PTEnd - PTStart);

	if(LanguageContainer.get<0>().find("en") == LanguageContainer.get<0>().end())
	{
		Wt::log("warn") << Name() << ": English language was not found or installed";
	}
	Wt::log("info") << Name() << ": " << LanguageSingleContainer.size() << " Single and "
		<< LanguagePluralContainer.size() << " Plural entries from " << LanguageContainer.size()
		<< " Languages successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
}

boost::shared_ptr<const LanguageData> LanguagesDatabase::GetLanguagePtrFromCode(const std::string &Code) const
{
	ReadLock lock(Manager());
	LanguageByCode::const_iterator itr = LanguageContainer.get<0>().find(Code);
	if(itr == LanguageContainer.get<0>().end())
	{
		return boost::shared_ptr<const LanguageData>();
	}
	return *itr;
}

boost::shared_ptr<const LanguageData> LanguagesDatabase::GetLanguagePtrFromLanguageAccept(const std::string &LanguageAccept) const
{
	ReadLock lock(Manager());
	LanguageByLanguageAccept::const_iterator itr = LanguageContainer.get<1>().find(LanguageAccept);
	if(itr == LanguageContainer.get<1>().end())
	{
		return boost::shared_ptr<const LanguageData>();
	}
	return *itr;
}

boost::shared_ptr<const SingularStringData> LanguagesDatabase::GetSinglePtr(const std::string &Code, const std::string &Key, long long ModuleId) const
{
	ReadLock lock(Manager());
	LanguageSingleType::const_iterator itr = LanguageSingleContainer.get<0>().find(boost::make_tuple(Code, Key, ModuleId));
	if(itr == LanguageSingleContainer.get<0>().end())
	{
		return boost::shared_ptr<const SingularStringData>();
	}
	return *itr;
}

boost::shared_ptr<const PluralStringData> LanguagesDatabase::GetPluralPtr(const std::string &Code, const std::string &Key, long long ModuleId, int Case) const
{
	ReadLock lock(Manager());
	LanguagePluralType::const_iterator itr = LanguagePluralContainer.get<0>().find(boost::make_tuple(Code, Key, Case, ModuleId));
	if(itr == LanguagePluralContainer.get<0>().end())
	{
		return boost::shared_ptr<const PluralStringData>();
	}
	return *itr;
}

std::string LanguagesDatabase::DefaultLanguageCode(const std::string &HostName) const
{
	ReadLock lock(Manager());
	boost::shared_ptr<const AccessHostNameData> AccessHostNamePtr = Server()->AccessPaths()->AccessHostOrGlobalPtr(HostName);
	if(AccessHostNamePtr)
	{
		if(!AccessHostNamePtr->LanguageCode.empty())
		{
			return AccessHostNamePtr->LanguageCode;
		}
		else
		{
			return "en";
		}
	}
	return "en";
}

bool LanguagesDatabase::LanguageCodeExists(const std::string &Code) const
{
	ReadLock lock(Manager());
	LanguageByCode::const_iterator itr = LanguageContainer.get<0>().find(Code);
	return itr != LanguageContainer.get<0>().end();
}
bool LanguagesDatabase::LanguageAcceptExists(const std::string &LanguageAccept) const
{
	ReadLock lock(Manager());
	LanguageByLanguageAccept::const_iterator itr = LanguageContainer.get<1>().find(LanguageAccept);
	return itr != LanguageContainer.get<1>().end();
}

bool LanguagesDatabase::GetPluralExpression(const std::string &Code, std::string &Result) const
{
	boost::shared_ptr<const LanguageData> LanguagePtr = GetLanguagePtrFromCode(Code);
	if(!LanguagePtr)
	{
		return false;
	}
	Result = LanguagePtr->PluralExpression;
	return true;
}

bool LanguagesDatabase::GetSingleString(const std::string &Code, const std::string &Key, long long ModuleId, std::string &Result) const
{
	boost::shared_ptr<const SingularStringData> LanguageSinglePtr = GetSinglePtr(Code, Key, ModuleId);
	if(!LanguageSinglePtr)
	{
		return false;
	}
	Result = LanguageSinglePtr->String;
	return true;
}

bool LanguagesDatabase::GetPluralString(const std::string &Code, const std::string &Key, long long ModuleId, int Case, std::string &Result) const
{
	boost::shared_ptr<const PluralStringData> LanguagePluralPtr = GetPluralPtr(Code, Key, ModuleId, Case);
	if(!LanguagePluralPtr)
	{
		return false;
	}
	Result = LanguagePluralPtr->String;
	return true;
}

Wt::WLocale LanguagesDatabase::GetLocaleFromCode(const std::string &Code, const std::string &HostName) const
{
	ReadLock lock(Manager());

	Wt::WLocale Locale(Code);
	std::string DefaultLanguage = DefaultLanguageCode(HostName);
	std::string DecimalPointCharacter = ".";
	std::string NumberThousandSeparator = ",";
	std::string DateFormat = "MMMM dd, yyyy";

	if(!GetSingleString(Code, "DecimalPointCharacter", ModulesDatabase::Localization, DecimalPointCharacter))
	{
		GetSingleString(DefaultLanguage, "DecimalPointCharacter", ModulesDatabase::Localization, DecimalPointCharacter);
	}
	if(!GetSingleString(Code, "NumberThousandSeparator", ModulesDatabase::Localization, NumberThousandSeparator))
	{
		GetSingleString(DefaultLanguage, "NumberThousandSeparator", ModulesDatabase::Localization, NumberThousandSeparator);
	}
	if(!GetSingleString(Code, "DateFormat", ModulesDatabase::Localization, DateFormat))
	{
		GetSingleString(DefaultLanguage, "DateFormat", ModulesDatabase::Localization, DateFormat);
	}

	Locale.setDecimalPoint(DecimalPointCharacter);
	Locale.setGroupSeparator(NumberThousandSeparator);
	Locale.setDateFormat(DateFormat);
	return Locale;
}

long long LanguagesDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}
std::size_t LanguagesDatabase::CountSingle() const
{
	ReadLock lock(Manager());
	return LanguageSingleContainer.size();
}
std::size_t LanguagesDatabase::CountPlural() const
{
	ReadLock lock(Manager());
	return LanguagePluralContainer.size();
}
std::size_t LanguagesDatabase::CountLanguages() const
{
	ReadLock lock(Manager());
	return LanguageContainer.size();
}

void LanguagesDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}
void LanguagesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
}

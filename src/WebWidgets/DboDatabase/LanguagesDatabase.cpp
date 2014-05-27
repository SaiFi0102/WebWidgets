#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "Application/WServer.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

LanguagesDatabase::LanguagesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: _Server(Server)
{
	MapClasses();
	DboSession.setConnectionPool(SQLPool);
}

LanguagesDatabase::LanguagesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: _Server(Server)
{
	MapClasses();
	DboSession.setConnection(SQLConnection);
}

void LanguagesDatabase::FetchAll()
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	LanguageContainers languagecontainer;
	languagecontainer.swap(LanguageContainer);

	LanguageSingleContainers languagesinglecontainer;
	languagesinglecontainer.swap(LanguageSingleContainer);

	LanguagePluralContainers languagepluralcontainer;
	languagepluralcontainer.swap(LanguagePluralContainer);

	//Strong transaction like exception safety
	try
	{
		DboSession.disconnectAll();

		Wt::Dbo::Transaction transaction(DboSession);
		LanguageCollections LanguageCollection = DboSession.find<Language>().where("\"Installed\" = 1");

		//All languages
		for(LanguageCollections::const_iterator itr = LanguageCollection.begin();
			itr != LanguageCollection.end();
			++itr)
		{
			//Fetch em all
			LanguageSingleCollections LanguageSingleCollection = DboSession.find<LanguageSingle>().where("\"Language_Code\" = ?").bind((*itr)->Code);
			LanguagePluralCollections LanguagePluralCollection = DboSession.find<LanguagePlural>().where("\"Language_Code\" = ?").bind((*itr)->Code);

			//Insert language ptr
			LanguageContainer.get<0>().insert(*itr);

			//All single strings of this language
			for(LanguageSingleCollections::const_iterator i = LanguageSingleCollection.begin();
				i != LanguageSingleCollection.end();
				++i)
			{
				LanguageSingleContainer.get<0>().insert(*i);
			}

			//All plural strings of this language
			for(LanguagePluralCollections::const_iterator i = LanguagePluralCollection.begin();
				i != LanguagePluralCollection.end();
				++i)
			{
				LanguagePluralContainer.get<0>().insert(*i);
			}
		}

		transaction.commit();
	}
	catch(...)
	{
		LanguageContainer.swap(languagecontainer);
		LanguageSingleContainer.swap(languagesinglecontainer);
		LanguagePluralContainer.swap(languagepluralcontainer);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = boost::posix_time::time_duration(PTEnd - PTStart);
}

void LanguagesDatabase::MapClasses()
{
	DboSession.mapClass<Author>(Author::TableName());
	DboSession.mapClass<Module>(Module::TableName());
	DboSession.mapClass<Configuration>(Configuration::TableName());
	DboSession.mapClass<ConfigurationBool>(ConfigurationBool::TableName());
	DboSession.mapClass<ConfigurationEnum>(ConfigurationEnum::TableName());
	DboSession.mapClass<ConfigurationEnumValue>(ConfigurationEnumValue::TableName());
	DboSession.mapClass<ConfigurationDouble>(ConfigurationDouble::TableName());
	DboSession.mapClass<ConfigurationFloat>(ConfigurationFloat::TableName());
	DboSession.mapClass<ConfigurationInt>(ConfigurationInt::TableName());
	DboSession.mapClass<ConfigurationLongInt>(ConfigurationLongInt::TableName());
	DboSession.mapClass<ConfigurationString>(ConfigurationString::TableName());
	DboSession.mapClass<Language>(Language::TableName());
	DboSession.mapClass<LanguageSingle>(LanguageSingle::TableName());
	DboSession.mapClass<LanguagePlural>(LanguagePlural::TableName());
	DboSession.mapClass<Page>(Page::TableName());
	DboSession.mapClass<Template>(Template::TableName());
	DboSession.mapClass<Style>(Style::TableName());
	DboSession.mapClass<StyleTemplate>(StyleTemplate::TableName());
	DboSession.mapClass<StyleCssRule>(StyleCssRule::TableName());
	DboSession.mapClass<TemplateCssRule>(TemplateCssRule::TableName());
	DboSession.mapClass<AccessPath>(AccessPath::TableName());
}

Wt::Dbo::ptr<Language> LanguagesDatabase::GetLanguagePtrFromCode(const std::string &Code) const
{
	READ_LOCK;
	LanguageByCode::const_iterator itr = LanguageContainer.get<0>().find(Code);
	if(itr == LanguageContainer.get<0>().end())
	{
		return Wt::Dbo::ptr<Language>();
	}
	return *itr;
}

Wt::Dbo::ptr<Language> LanguagesDatabase::GetLanguagePtrFromLanguageAccept(const std::string &LanguageAccept) const
{
	READ_LOCK;
	LanguageByLanguageAccept::const_iterator itr = LanguageContainer.get<1>().find(LanguageAccept);
	if(itr == LanguageContainer.get<1>().end())
	{
		return Wt::Dbo::ptr<Language>();
	}
	return *itr;
}

Wt::Dbo::ptr<LanguageSingle> LanguagesDatabase::GetSinglePtr(const std::string &Code, const std::string &Key, long long ModuleId) const
{
	READ_LOCK;
	LanguageSingleType::const_iterator itr = LanguageSingleContainer.get<0>().find(boost::make_tuple(Code, Key, ModuleId));
	if(itr == LanguageSingleContainer.get<0>().end())
	{
		return Wt::Dbo::ptr<LanguageSingle>();
	}
	return *itr;
}

Wt::Dbo::ptr<LanguagePlural> LanguagesDatabase::GetPluralPtr(const std::string &Code, const std::string &Key, long long ModuleId, int Case) const
{
	READ_LOCK;
	LanguagePluralType::const_iterator itr = LanguagePluralContainer.get<0>().find(boost::make_tuple(Code, Key, Case, ModuleId));
	if(itr == LanguagePluralContainer.get<0>().end())
	{
		return Wt::Dbo::ptr<LanguagePlural>();
	}
	return *itr;
}

bool LanguagesDatabase::LanguageCodeExists(const std::string &Code) const
{
	READ_LOCK;
	LanguageByCode::const_iterator itr = LanguageContainer.get<0>().find(Code);
	return itr != LanguageContainer.get<0>().end();
}
bool LanguagesDatabase::LanguageAcceptExists(const std::string &LanguageAccept) const
{
	READ_LOCK;
	LanguageByLanguageAccept::const_iterator itr = LanguageContainer.get<1>().find(LanguageAccept);
	return itr != LanguageContainer.get<1>().end();
}

bool LanguagesDatabase::GetPluralExpression(const std::string &Code, std::string &Result) const
{
	READ_LOCK;
	Wt::Dbo::ptr<Language> LanguagePtr = GetLanguagePtrFromCode(Code);
	if(!LanguagePtr)
	{
		return false;
	}
	Result = LanguagePtr->PluralExpression;
	return true;
}

bool LanguagesDatabase::GetSingleString(const std::string &Code, const std::string &Key, long long ModuleId, std::string &Result) const
{
	READ_LOCK;
	Wt::Dbo::ptr<LanguageSingle> LanguageSinglePtr = GetSinglePtr(Code, Key, ModuleId);
	if(!LanguageSinglePtr)
	{
		return false;
	}
	Result = LanguageSinglePtr->String;
	return true;
}

bool LanguagesDatabase::GetPluralString(const std::string &Code, const std::string &Key, long long ModuleId, int Case, std::string &Result) const
{
	READ_LOCK;
	Wt::Dbo::ptr<LanguagePlural> LanguagePluralPtr = GetPluralPtr(Code, Key, ModuleId, Case);
	if(!LanguagePluralPtr)
	{
		return false;
	}
	Result = LanguagePluralPtr->String;
	return true;
}

Wt::WLocale LanguagesDatabase::GetLocaleFromCode(const std::string &Code) const
{
	READ_LOCK;
	Wt::WLocale Locale(Code);
	if(!LanguageCodeExists(Code))
	{
		return Locale;
	}

	Wt::Dbo::ptr<AccessPath> DefaultAccessPath = _Server.GetAccessPaths()->GetPtr(_Server.GetConfigurations()->GetLongInt("DefaultAccessPath", ModulesDatabase::Localization, 1));
	std::string DefaultLanguage = "en";
	if(DefaultAccessPath && DefaultAccessPath->LanguagePtr)
	{
		DefaultLanguage = DefaultAccessPath->LanguagePtr.id();
	}

	std::string DecimalPointCharacter = ".";
	std::string NumberThousandSeparator = ",";
	std::string DateFormat = "MMMM dd, yyyy";
	if(!_Server.GetLanguages()->GetSingleString(Code, "DecimalPointCharacter", ModulesDatabase::Localization, DecimalPointCharacter))
	{
		_Server.GetLanguages()->GetSingleString(DefaultLanguage, "DecimalPointCharacter", ModulesDatabase::Localization, DecimalPointCharacter);
	}
	if(!_Server.GetLanguages()->GetSingleString(Code, "NumberThousandSeparator", ModulesDatabase::Localization, NumberThousandSeparator))
	{
		_Server.GetLanguages()->GetSingleString(DefaultLanguage, "NumberThousandSeparator", ModulesDatabase::Localization, NumberThousandSeparator);
	}
	if(!_Server.GetLanguages()->GetSingleString(Code, "DateFormat", ModulesDatabase::Localization, DateFormat))
	{
		_Server.GetLanguages()->GetSingleString(DefaultLanguage, "DateFormat", ModulesDatabase::Localization, DateFormat);
	}
	Locale.setDecimalPoint(DecimalPointCharacter);
	Locale.setGroupSeparator(NumberThousandSeparator);
	Locale.setDateFormat(DateFormat);
	return Locale;
}
Wt::WLocale LanguagesDatabase::GetLocaleFromLanguageAccept(const std::string &LanguageAccept) const
{
	READ_LOCK;
	return GetLocaleFromCode(GetLanguagePtrFromLanguageAccept(LanguageAccept)->Code);
}

long long LanguagesDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}
std::size_t LanguagesDatabase::CountSingle() const
{
	READ_LOCK;
	return LanguageSingleContainer.get<0>().size();
}
std::size_t LanguagesDatabase::CountPlural() const
{
	READ_LOCK;
	return LanguagePluralContainer.get<0>().size();
}
std::size_t LanguagesDatabase::CountLanguages() const
{
	READ_LOCK;
	return LanguageContainer.get<0>().size();
}

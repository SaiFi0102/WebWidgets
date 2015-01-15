#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

ModulesDatabase::ModulesDatabase(DboDatabaseManager *Manager)
	: AbstractDboDatabase(Manager)
{ }

void ModulesDatabase::Load(Wt::Dbo::Session &DboSession)
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
	DboSession.mapClass<AccessHostName>(AccessHostName::TableName());
	DboSession.mapClass<PageAccessPath>(PageAccessPath::TableName());
	DboSession.mapClass<LanguageAccessPath>(LanguageAccessPath::TableName());

	FetchAll(DboSession);
}

void ModulesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	ModuleMaps modulemap;
	modulemap.swap(ModuleMap);

	//Strong transaction like exception safety
	try
	{
		Wt::Dbo::Transaction transaction(DboSession);
		ModuleCollections ModuleCollection = DboSession.find<Module>();
	
		//All Modules
		for(ModuleCollections::const_iterator itr = ModuleCollection.begin();
			itr != ModuleCollection.end();
			++itr)
		{
			ModuleMap[itr->id()] = boost::shared_ptr<ModuleData>(new ModuleData(*itr));
		}

		transaction.commit();
	}
	catch(...)
	{
		ModuleMap.swap(modulemap);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << ModuleMap.size() << " entries successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
}

boost::shared_ptr<const ModuleData> ModulesDatabase::GetPtr(long long Id) const
{
	ReadLock lock(Manager());
	ModuleMaps::const_iterator itr = ModuleMap.find(Id);
	if(itr == ModuleMap.end())
	{
		return boost::shared_ptr<const ModuleData>();
	}
	return itr->second;
}

std::size_t ModulesDatabase::CountModules() const
{
	ReadLock lock(Manager());
	return ModuleMap.size();
}
long long ModulesDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}

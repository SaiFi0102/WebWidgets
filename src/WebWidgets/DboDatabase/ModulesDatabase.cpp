#include "DboDatabase/ModulesDatabase.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

ModulesDatabase::ModulesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnectionPool(SQLPool);
	MapClasses();
}

ModulesDatabase::ModulesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnection(SQLConnection);
	MapClasses();
}

void ModulesDatabase::MapClasses()
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

void ModulesDatabase::FetchAll()
{
	WRITE_LOCK;

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
			ModuleMap[itr->id()] = *itr;
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
}

Wt::Dbo::ptr<Module> ModulesDatabase::GetPtr(long long Id) const
{
	READ_LOCK;
	ModuleMaps::const_iterator itr = ModuleMap.find(Id);
	if(itr == ModuleMap.end())
	{
		return Wt::Dbo::ptr<Module>();
	}
	return itr->second;
}

Module ModulesDatabase::GetDbo(Wt::Dbo::dbo_traits<Module>::IdType Id) const
{
	READ_LOCK;
	Wt::Dbo::ptr<Module> Ptr = GetPtr(Id);
	if(!Ptr)
	{
		return Module();
	}
	return *Ptr;
}

std::size_t ModulesDatabase::CountModules() const
{
	READ_LOCK;
	return ModuleMap.size();
}
long long ModulesDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}

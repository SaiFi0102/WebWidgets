#include "DboDatabase/ModulesDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

ModulesDatabase::ModulesDatabase(DboDatabaseManager *Manager)
	: AbstractDboDatabase(Manager)
{ }

void ModulesDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}

void ModulesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	ModuleMaps modulemap;

	Wt::Dbo::Transaction transaction(DboSession);
	ModuleCollections ModuleCollection = DboSession.find<Module>();
	
	//All Modules
	for(ModuleCollections::const_iterator itr = ModuleCollection.begin();
		itr != ModuleCollection.end();
		++itr)
	{
		modulemap[itr->id()] = boost::shared_ptr<ModuleData>(new ModuleData(*itr));
	}

	transaction.commit();
	ModuleMap.swap(modulemap);

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

#include "DboDatabase/ModuleDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

namespace WW
{

ModuleDatabase::ModuleDatabase(DboDatabaseManager *manager)
	: AbstractDboDatabase(manager)
{ }

void ModuleDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	fetchAll(dboSession);
}

void ModuleDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary object first
	ModuleMap modulemap;

	Wt::Dbo::Transaction transaction(dboSession);
	Dbo::ModuleCollection moduleCollection = dboSession.find<Dbo::Module>();
	
	//All Modules
	for(Dbo::ptr<Dbo::Module> &ptr : moduleCollection)
	{
		modulemap[ptr.id()] = Ddo::ptr<Ddo::Module>(new Ddo::Module(ptr));
	}

	transaction.commit();
	_moduleMap.swap(modulemap);

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = ptEnd - ptStart;

	Wt::log("info") << name() << ": " << _moduleMap.size() << " entries successfully loaded in " << _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::Module> ModuleDatabase::getPtr(long long id) const
{
	ReadLock lock(manager());
	ModuleMap::const_iterator itr = _moduleMap.find(id);
	if(itr == _moduleMap.end())
		return Ddo::cPtr<Ddo::Module>();

	return itr->second;
}

std::size_t ModuleDatabase::modulesCount() const
{
	ReadLock lock(manager());
	return _moduleMap.size();
}
long long ModuleDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}

}

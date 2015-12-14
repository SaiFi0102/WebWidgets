#include "DboDatabase/DboDatabaseManager.h"
#include "DboDatabase/AbstractDboDatabase.h"
#include "Application/WServer.h"
#include <boost/thread/lock_guard.hpp>
#include <Wt/Dbo/Exception>
#include <Wt/WLogger>

namespace WW
{

DboDatabaseManager::DboDatabaseManager(WServer *server, Wt::Dbo::SqlConnection *sqlConnection)
: _server(server), _IsConnection(true), _sqlConnection(sqlConnection)
{ }
DboDatabaseManager::DboDatabaseManager(WServer *server, Wt::Dbo::SqlConnectionPool *sqlPool)
: _server(server), _IsConnection(false), _sqlPool(sqlPool)
{ }

DboDatabaseManager::~DboDatabaseManager()
{
	for(DatabaseSet::const_iterator itr = _databaseSet.begin();
		itr != _databaseSet.end();)
	{
		delete *(itr++);
	}
}

void DboDatabaseManager::add(AbstractDboDatabase *dboDatabase)
{
	if(!dboDatabase)
		return;

	boost::lock_guard<boost::shared_mutex> lock(_mutex);
	
	std::pair<DatabaseSet::iterator, bool> itr = _databaseSet.insert(dboDatabase);
	if(!itr.second)	//Already added
		return;

	if(_IsConnection)
		(*itr.first)->_dboSession.setConnection(*_sqlConnection);
	else
		(*itr.first)->_dboSession.setConnectionPool(*_sqlPool);
}

void DboDatabaseManager::remove(AbstractDboDatabase *dboDatabase)
{
	if(dboDatabase)
	{
		boost::lock_guard<boost::shared_mutex> lock(_mutex);
		_databaseSet.erase(dboDatabase);
		dboDatabase->_manager = nullptr;
	}
}

void DboDatabaseManager::load()
{
	if(!_initiallyLoaded)
	{
		boost::lock_guard<boost::shared_mutex> lock(_mutex);
		for(DatabaseSet::iterator itr = _databaseSet.begin();
			itr != _databaseSet.end();
			++itr)
		{
// 			try
// 			{
				Wt::log("info") << "Loading " << (*itr)->name();
				(*itr)->load((*itr)->_dboSession);
				(*itr)->_isLoaded = true;
				Wt::log("success") << "Successfully loaded " << (*itr)->name();
// 			}
// 			catch(Wt::Dbo::Exception &e)
// 			{
// 				Wt::log("error") << "Database error loading " << (*itr)->Name() << ": " << e.what();
// 				if((*itr)->IsVital())
// 				{
// 					throw e;
// 				}
// 			}
// 			catch(std::exception &e)
// 			{
// 				Wt::log("error") << "Error loading " << (*itr)->Name() << ": " << e.what();
// 				if((*itr)->IsVital())
// 				{
// 					throw e;
// 				}
// 			}
		}
	}
	else
	{
		reload();
	}
}
void DboDatabaseManager::reload()
{
	{
		boost::lock_guard<boost::shared_mutex> lock(_mutex);
		for(DatabaseSet::iterator itr = _databaseSet.begin();
			itr != _databaseSet.end();
			++itr)
		{
			try
			{
				if(!(*itr)->_isLoaded)
				{
					Wt::log("info") << "Loading " << (*itr)->name();
					(*itr)->load((*itr)->_dboSession);
				}
				else
				{
					Wt::log("info") << "Reloading " << (*itr)->name();
					(*itr)->reload((*itr)->_dboSession);
				}
				(*itr)->_isLoaded = true;
				Wt::log("success") << "Successfully loaded " << (*itr)->name();
			}
			catch(Wt::Dbo::Exception &e)
			{
				Wt::log("error") << "Database error " << ((*itr)->_isLoaded ? "reloading " : "loading ")
					<< (*itr)->name() << ": " << e.what();

				if((*itr)->isVital() && !(*itr)->_isLoaded)
					throw e;
			}
			catch(std::exception &e)
			{
				Wt::log("error") << "Error " << ((*itr)->_isLoaded ? "reloading " : "loading ")
					<< (*itr)->name() << ": " << e.what();

				if((*itr)->isVital() && !(*itr)->_isLoaded)
					throw e;
			}
		}
	}
	server()->dboDatabaseReloadHandler();
}

}

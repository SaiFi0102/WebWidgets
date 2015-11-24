#include "DboDatabase/DboDatabaseManager.h"
#include "DboDatabase/AbstractDboDatabase.h"
#include "Application/WServer.h"
#include <boost/thread/lock_guard.hpp>
#include <Wt/Dbo/Exception>
#include <Wt/WLogger>

DboDatabaseManager::DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnection *SQLConnection)
: _Server(Server), _IsConnection(true), _InitiallyLoaded(false), SQLConnection(SQLConnection)
{ }
DboDatabaseManager::DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnectionPool *SQLPool)
: _Server(Server), _IsConnection(false), _InitiallyLoaded(false), SQLPool(SQLPool)
{ }

DboDatabaseManager::~DboDatabaseManager()
{
	for(DatabaseSets::const_iterator itr = DatabaseSet.begin();
		itr != DatabaseSet.end();)
	{
		delete *(itr++);
	}
}

void DboDatabaseManager::Add(AbstractDboDatabase *DboDatabase)
{
	if(!DboDatabase)
	{
		return;
	}

	boost::lock_guard<boost::shared_mutex> lock(mutex);
	
	std::pair<DatabaseSets::iterator, bool> itr = DatabaseSet.insert(DboDatabase);
	if(!itr.second)	//Already added
	{
		return;
	}
	if(_IsConnection)
	{
		(*itr.first)->_DboSession.setConnection(*SQLConnection);
	}
	else
	{
		(*itr.first)->_DboSession.setConnectionPool(*SQLPool);
	}
}

void DboDatabaseManager::Remove(AbstractDboDatabase *DboDatabase)
{
	if(DboDatabase)
	{
		boost::lock_guard<boost::shared_mutex> lock(mutex);
		DatabaseSet.erase(DboDatabase);
		DboDatabase->_Manager = 0;
	}
}

void DboDatabaseManager::Load()
{
	if(!_InitiallyLoaded)
	{
		boost::lock_guard<boost::shared_mutex> lock(mutex);
		for(DatabaseSets::iterator itr = DatabaseSet.begin();
			itr != DatabaseSet.end();
			++itr)
		{
// 			try
// 			{
				Wt::log("info") << "Loading " << (*itr)->Name();
				(*itr)->Load((*itr)->_DboSession);
				(*itr)->_IsLoaded = true;
				Wt::log("success") << "Successfully loaded " << (*itr)->Name();
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
		Reload();
	}
}
void DboDatabaseManager::Reload()
{
	{
		boost::lock_guard<boost::shared_mutex> lock(mutex);
		for(DatabaseSets::iterator itr = DatabaseSet.begin();
			itr != DatabaseSet.end();
			++itr)
		{
			try
			{
				if(!(*itr)->_IsLoaded)
				{
					Wt::log("info") << "Loading " << (*itr)->Name();
					(*itr)->Load((*itr)->_DboSession);
				}
				else
				{
					Wt::log("info") << "Reloading " << (*itr)->Name();
					(*itr)->Reload((*itr)->_DboSession);
				}
				(*itr)->_IsLoaded = true;
				Wt::log("success") << "Successfully loaded " << (*itr)->Name();
			}
			catch(Wt::Dbo::Exception &e)
			{
				Wt::log("error") << "Database error " << ((*itr)->_IsLoaded ? "reloading " : "loading ")
					<< (*itr)->Name() << ": " << e.what();

				if((*itr)->IsVital() && !(*itr)->_IsLoaded)
				{
					throw e;
				}
			}
			catch(std::exception &e)
			{
				Wt::log("error") << "Error " << ((*itr)->_IsLoaded ? "reloading " : "loading ")
					<< (*itr)->Name() << ": " << e.what();

				if((*itr)->IsVital() && !(*itr)->_IsLoaded)
				{
					throw e;
				}
			}
		}
	}
	Server()->DboDatabaseReloadHandler();
}
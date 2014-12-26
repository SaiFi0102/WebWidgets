#include "DboDatabase/DboDatabaseManager.h"
#include "DboDatabase/AbstractDboDatabase.h"
#include "Application/WServer.h"
#include <Wt/Dbo/Exception>
#include <boost/thread/lock_guard.hpp>

DboDatabaseManager::DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnection *SQLConnection)
: _Server(Server), _IsConnection(true), _InitiallyLoaded(false), SQLConnection(SQLConnection)
{ }
DboDatabaseManager::DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnectionPool *SQLPool)
: _Server(Server), _IsConnection(false), _InitiallyLoaded(false), SQLPool(SQLPool)
{ }

DboDatabaseManager::~DboDatabaseManager()
{
	for(DatabaseMaps::const_iterator itr = DatabaseMap.begin();
		itr != DatabaseMap.end();)
	{
		delete (itr++)->first;
	}
}

void DboDatabaseManager::Add(AbstractDboDatabase *DboDatabase)
{
	if(!DboDatabase)
	{
		return;
	}

	boost::lock_guard<boost::mutex> lock(mutex);
	
	if(_IsConnection)
	{
		DatabaseMap[DboDatabase].setConnection(*SQLConnection);
	}
	else
	{
		DatabaseMap[DboDatabase].setConnectionPool(*SQLPool);
	}
}

void DboDatabaseManager::Remove(AbstractDboDatabase *DboDatabase)
{
	if(DboDatabase)
	{
		boost::lock_guard<boost::mutex> lock(mutex);
		DatabaseMap.erase(DboDatabase);
		DboDatabase->Manager = 0;
	}
}

void DboDatabaseManager::Load()
{
	if(!_InitiallyLoaded)
	{
		boost::lock_guard<boost::mutex> lock(mutex);
		for(DatabaseMaps::iterator itr = DatabaseMap.begin();
			itr != DatabaseMap.end();
			++itr)
		{
			try
			{
				Wt::log("info") << "Loading " << itr->first->Name();
				itr->first->Load(itr->second);
				itr->first->_IsLoaded = true;
				Wt::log("success") << "Successfully loaded " << itr->first->Name();
			}
			catch(Wt::Dbo::Exception &e)
			{
				Wt::log("error") << "Database error loading " << itr->first->Name() << ": " << e.what();
				if(itr->first->IsVital())
				{
					throw e;
				}
			}
			catch(std::exception &e)
			{
				Wt::log("error") << "Error loading " << itr->first->Name() << ": " << e.what();
				if(itr->first->IsVital())
				{
					throw e;
				}
			}
		}
	}
	else
	{
		Reload();
	}
}
void DboDatabaseManager::Reload()
{
	boost::lock_guard<boost::mutex> lock(mutex);
	for(DatabaseMaps::iterator itr = DatabaseMap.begin();
		itr != DatabaseMap.end();
		++itr)
	{
		try
		{
			if(!itr->first->_IsLoaded)
			{
				Wt::log("info") << "Loading " << itr->first->Name();
				itr->first->Load(itr->second);
			}
			else
			{
				Wt::log("info") << "Reloading " << itr->first->Name();
				itr->first->Reload(itr->second);
			}
			itr->first->_IsLoaded = true;
			Wt::log("success") << "Successfully loaded " << itr->first->Name();
		}
		catch(Wt::Dbo::Exception &e)
		{
			Wt::log("error") << "Database error " << (itr->first->_IsLoaded ? "reloading " : "loading ")
				<< itr->first->Name() << ": " << e.what();

			if(itr->first->IsVital() && !itr->first->_IsLoaded)
			{
				throw e;
			}
		}
		catch(std::exception &e)
		{
			Wt::log("error") << "Error " << (itr->first->_IsLoaded ? "reloading " : "loading ")
				<< itr->first->Name() << ": " << e.what();

			if(itr->first->IsVital() && !itr->first->_IsLoaded)
			{
				throw e;
			}
		}
	}
}
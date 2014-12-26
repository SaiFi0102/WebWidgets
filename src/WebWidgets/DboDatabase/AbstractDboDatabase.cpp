#include "DboDatabase/AbstractDboDatabase.h"

AbstractDboDatabase::AbstractDboDatabase(DboDatabaseManager *Manager)
: Manager(Manager), _IsLoaded(false)
{
	Manager->Add(this);
}

AbstractDboDatabase::~AbstractDboDatabase()
{
	Manager->Remove(this);
}

WServer *AbstractDboDatabase::Server() const
{
	if(!Manager)
	{
		return 0;
	}
	return Manager->Server();
}
#include "DboDatabase/AbstractDboDatabase.h"

AbstractDboDatabase::AbstractDboDatabase(DboDatabaseManager *Manager)
: _Manager(Manager), _IsLoaded(false)
{
	Manager->Add(this);
}

AbstractDboDatabase::~AbstractDboDatabase()
{
	_Manager->Remove(this);
}

WServer *AbstractDboDatabase::Server() const
{
	if(!_Manager)
	{
		return 0;
	}
	return _Manager->Server();
}
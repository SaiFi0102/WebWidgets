#include "DboDatabase/AbstractDboDatabase.h"

namespace WW
{

AbstractDboDatabase::AbstractDboDatabase(DboDatabaseManager *manager)
	: _manager(manager)
{
	manager->add(this);
}

AbstractDboDatabase::~AbstractDboDatabase()
{
	_manager->remove(this);
}

WServer *AbstractDboDatabase::server() const
{
	if(!_manager)
		return nullptr;

	return _manager->server();
}

}

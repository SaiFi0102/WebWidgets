#include "DboDatabase/ReadLock.h"
#include "DboDatabase/AbstractDboDatabase.h"
#include "DboDatabase/DboDatabaseManager.h"

ReadLock::ReadLock(AbstractDboDatabase *Database)
	: lock(Database->mutex)
{ }

void ReadLock::Unlock()
{
	if(lock.owns_lock())
	{
		lock.unlock();
	}
}
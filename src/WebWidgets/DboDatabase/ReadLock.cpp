#include "DboDatabase/ReadLock.h"
#include "DboDatabase/AbstractDboDatabase.h"

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
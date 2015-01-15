#include "DboDatabase/ReadLock.h"
#include "DboDatabase/DboDatabaseManager.h"

ReadLock::ReadLock(DboDatabaseManager *Manager)
	: lock(Manager->mutex)
{ }

void ReadLock::Unlock()
{
	if(lock.owns_lock())
	{
		lock.unlock();
	}
}
#include "DboDatabase/ReadLock.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/LanguagesDatabase.h"
#include "DboDatabase/PagesDatabase.h"
#include "DboDatabase/StylesDatabase.h"

ReadLock::ReadLock(ConfigurationsDatabase *Database)
	: lock(Database->mutex)
{ }

ReadLock::ReadLock(AccessPathsDatabase *Database)
	: lock(Database->mutex)
{ }

ReadLock::ReadLock(LanguagesDatabase *Database)
	: lock(Database->mutex)
{ }

ReadLock::ReadLock(PagesDatabase *Database)
	: lock(Database->mutex)
{ }

ReadLock::ReadLock(StylesDatabase *Database)
	: lock(Database->mutex)
{ }

void ReadLock::Release()
{
	lock.release();
}

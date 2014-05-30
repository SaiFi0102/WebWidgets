#ifndef READLOCK_H
#define READLOCK_H

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>

class ConfigurationsDatabase;
class AccessPathsDatabase;
class LanguagesDatabase;
class PagesDatabase;
class StylesDatabase;

class ReadLock
{
	public:
		ReadLock(ConfigurationsDatabase *Database);
		ReadLock(AccessPathsDatabase *Database);
		ReadLock(LanguagesDatabase *Database);
		ReadLock(PagesDatabase *Database);
		ReadLock(StylesDatabase *Database);
		void Release();

	protected:
		boost::shared_lock<boost::shared_mutex> lock;
};

#endif
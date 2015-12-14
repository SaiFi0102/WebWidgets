#ifndef WW_DBODATABASE_READLOCK_H
#define WW_DBODATABASE_READLOCK_H

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/lock_guard.hpp>

namespace WW
{
	class DboDatabaseManager;

	class ReadLock
	{
	public:
		ReadLock(DboDatabaseManager *manager)
			: _lock(manager->_mutex)
		{ }
		void Unlock()
		{
			if(_lock.owns_lock())
				_lock.unlock();
		}

	protected:
		boost::shared_lock<boost::shared_mutex> _lock;
	};

	class WriteLock
	{
	public:
		WriteLock(DboDatabaseManager *manager)
			: _lock(manager->_mutex)
		{ }

	protected:
		boost::lock_guard<boost::shared_mutex> _lock;
	};
}

#endif
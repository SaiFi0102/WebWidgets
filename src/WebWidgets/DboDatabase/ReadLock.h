#ifndef READLOCK_H
#define READLOCK_H

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/lock_guard.hpp>

class DboDatabaseManager;

class ReadLock
{
	public:
		ReadLock(DboDatabaseManager *Manager)
			: lock(Manager->mutex)
		{ }
		void Unlock()
		{
			if(lock.owns_lock())
				lock.unlock();
		}

	protected:
		boost::shared_lock<boost::shared_mutex> lock;
};

class WriteLock
{
	public:
		WriteLock(DboDatabaseManager *Manager)
			: lock(Manager->mutex)
		{ }

	protected:
		boost::lock_guard<boost::shared_mutex> lock;
};

#endif
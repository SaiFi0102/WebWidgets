#ifndef READLOCK_H
#define READLOCK_H

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>

class AbstractDboDatabase;

class ReadLock
{
	public:
		ReadLock(AbstractDboDatabase *Database);
		void Unlock();

	protected:
		boost::shared_lock<boost::shared_mutex> lock;
};

#endif
#ifndef ABSTRACT_DBO_DATABASE_H
#define ABSTRACT_DBO_DATABASE_H

#include "DboDatabase/DboDatabaseManager.h"
#include <boost/thread/shared_mutex.hpp>

class AbstractDboDatabase
{
	public:
		AbstractDboDatabase(DboDatabaseManager *Manager);
		virtual ~AbstractDboDatabase();
		WServer *Server() const;
		virtual std::string Name() const = 0;

	protected:
		bool _IsLoaded;
		DboDatabaseManager *Manager;
		mutable boost::shared_mutex mutex;

		virtual void Load(Wt::Dbo::Session &DboSession) = 0;
		virtual void Reload(Wt::Dbo::Session &DboSession) { }
		virtual bool IsVital() const { return false; }

	private:
		friend class DboDatabaseManager;
		friend class ReadLock;
};

#endif
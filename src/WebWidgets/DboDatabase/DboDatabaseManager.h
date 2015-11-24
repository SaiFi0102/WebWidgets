#ifndef DBO_DATABASE_MANAGER_H
#define DBO_DATABASE_MANAGER_H

#include <boost/thread/shared_mutex.hpp>
#include <Wt/Dbo/SqlConnectionPool>
#include <set>

class WServer;
class AbstractDboDatabase;

class DboDatabaseManager
{
	public:
		typedef std::set<AbstractDboDatabase*> DatabaseSets;

		DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnection *SQLConnection);
		DboDatabaseManager(WServer *Server, Wt::Dbo::SqlConnectionPool *SQLPool);
		~DboDatabaseManager();

		void Add(AbstractDboDatabase *DboDatabase);
		void Remove(AbstractDboDatabase *DboDatabase);

		void Load();
		void Reload();

		WServer *Server() const
		{
			return _Server;
		}

	protected:
		WServer *_Server;
		bool _InitiallyLoaded;
		DatabaseSets DatabaseSet;
		mutable boost::shared_mutex mutex;

		bool _IsConnection;
		Wt::Dbo::SqlConnection *SQLConnection;
		Wt::Dbo::SqlConnectionPool *SQLPool;

	private:
		friend class ReadLock;
		friend class WriteLock;
};

#endif
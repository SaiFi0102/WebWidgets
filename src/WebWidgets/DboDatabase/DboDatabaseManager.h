#ifndef WW_DBODATABASE_MANAGER_H
#define WW_DBODATABASE_MANAGER_H

#include <boost/thread/shared_mutex.hpp>
#include <Wt/Dbo/SqlConnectionPool>
#include <set>

namespace WW
{
	class WServer;
	class AbstractDboDatabase;

	class DboDatabaseManager
	{
	public:
		typedef std::set<AbstractDboDatabase*> DatabaseSet;

		DboDatabaseManager(WServer *server, Wt::Dbo::SqlConnection *sqlConnection);
		DboDatabaseManager(WServer *server, Wt::Dbo::SqlConnectionPool *sqlPool);
		~DboDatabaseManager();

		void add(AbstractDboDatabase *dboDatabase);
		void remove(AbstractDboDatabase *dboDatabase);

		void load();
		void reload();

		WServer *server() const { return _server; }

	protected:
		WServer *_server;
		bool _initiallyLoaded = false;
		DatabaseSet _databaseSet;
		mutable boost::shared_mutex _mutex;

		bool _IsConnection;
		Wt::Dbo::SqlConnection *_sqlConnection = nullptr;
		Wt::Dbo::SqlConnectionPool *_sqlPool = nullptr;

	private:
		friend class ReadLock;
		friend class WriteLock;
	};
}

#endif

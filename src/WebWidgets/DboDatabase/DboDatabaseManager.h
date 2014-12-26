#ifndef DBO_DATABASE_MANAGER_H
#define DBO_DATABASE_MANAGER_H

#include <Wt/Dbo/Session>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

class WServer;
class AbstractDboDatabase;

class DboDatabaseManager
{
	public:
		typedef std::map<AbstractDboDatabase*, Wt::Dbo::Session> DatabaseMaps;

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
		DatabaseMaps DatabaseMap;
		mutable boost::mutex mutex;

		bool _IsConnection;
		Wt::Dbo::SqlConnection *SQLConnection;
		Wt::Dbo::SqlConnectionPool *SQLPool;

	private:
		friend class ReadLock;
};

#endif
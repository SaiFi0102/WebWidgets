#ifndef ABSTRACT_DBO_DATABASE_H
#define ABSTRACT_DBO_DATABASE_H

#include "DboDatabase/DboDatabaseManager.h"
#include <Wt/Dbo/Session>

class AbstractDboDatabase
{
	public:
		AbstractDboDatabase(DboDatabaseManager *Manager);
		virtual ~AbstractDboDatabase();
		WServer *Server() const;
		DboDatabaseManager *Manager() const { return _Manager; }
		virtual std::string Name() const = 0;

	protected:
		bool _IsLoaded;
		DboDatabaseManager *_Manager;

		virtual void Load(Wt::Dbo::Session &DboSession) = 0;
		virtual void Reload(Wt::Dbo::Session &DboSession) { }
		virtual bool IsVital() const { return false; }

	private:
		Wt::Dbo::Session _DboSession;

		friend class DboDatabaseManager;
};

#endif
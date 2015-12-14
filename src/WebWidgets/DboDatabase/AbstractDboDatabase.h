#ifndef WW_ABSTRACT_DBODATABASE_H
#define WW_ABSTRACT_DBODATABASE_H

#include "DboDatabase/DboDatabaseManager.h"
#include <Wt/Dbo/Session>

namespace WW
{
	class AbstractDboDatabase
	{
	public:
		virtual ~AbstractDboDatabase();
		WServer *server() const;
		DboDatabaseManager *manager() const { return _manager; }
		virtual std::string name() const = 0;

	protected:
		AbstractDboDatabase(DboDatabaseManager *Manager);
		virtual void load(Wt::Dbo::Session &dboSession) = 0;
		virtual void reload(Wt::Dbo::Session &dboSession) { }
		virtual bool isVital() const { return false; }

		bool _isLoaded = false;
		DboDatabaseManager *_manager = nullptr;

	private:
		Wt::Dbo::Session _dboSession;
		friend class DboDatabaseManager;
	};
}

#endif
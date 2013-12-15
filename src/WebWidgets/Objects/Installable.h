#ifndef INSTALLABLE_H
#define INSTALLABLE_H

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/SqlConnection>
#include <Wt/Dbo/FixedSqlConnectionPool>

class Installable
{
	public:
		Installable(Wt::Dbo::SqlConnectionPool &SQLPool);
		Installable(Wt::Dbo::SqlConnection &SQLConnection);
		virtual ~Installable();

		virtual void CreateTables();
		virtual void InsertRows();
		virtual void DropTables();
		virtual void Install();
		virtual void Reinstall();

	protected:
		virtual void MapClasses() = 0;

		Wt::Dbo::Session DboSession;
};

#endif
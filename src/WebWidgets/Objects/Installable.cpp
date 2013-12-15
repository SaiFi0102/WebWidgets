#include "Objects/Installable.h"

Installable::Installable(Wt::Dbo::SqlConnectionPool &SQLPool)
{
	DboSession.setConnectionPool(SQLPool);
}

Installable::Installable(Wt::Dbo::SqlConnection &SQLConnection)
{
	DboSession.setConnection(SQLConnection);
}

Installable::~Installable()
{ }

void Installable::MapClasses()
{ }

void Installable::CreateTables()
{
	DboSession.createTables();
}

void Installable::InsertRows()
{ }

void Installable::DropTables()
{
	DboSession.dropTables();
}

void Installable::Install()
{
	CreateTables();
	InsertRows();
}

void Installable::Reinstall()
{
	DropTables();
	Install();
}
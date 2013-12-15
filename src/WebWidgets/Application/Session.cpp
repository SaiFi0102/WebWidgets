#include "Application/Session.h"

Session::Session(Wt::Dbo::SqlConnectionPool &SQLPool)
{
	setConnectionPool(SQLPool);
}

Session::Session(Wt::Dbo::SqlConnection &SQLConnection)
{
	setConnection(SQLConnection);
}

Session::~Session()
{ }
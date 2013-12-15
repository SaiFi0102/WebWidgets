#ifndef DBOINSTALLER_H
#define DBOINSTALLER_H

#include "Objects/Installable.h"

class DboInstaller : public Installable
{
	public:
		DboInstaller(Wt::Dbo::SqlConnectionPool &SQLPool);
		DboInstaller(Wt::Dbo::SqlConnection &SQLConnection);

		virtual void CreateTables();
		virtual void InsertRows();

	protected:
		virtual void MapClasses();
};

#endif
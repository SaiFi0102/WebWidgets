#ifndef ACCESSPATH_DBO_H
#define ACCESSPATH_DBO_H

#include "Dbo/Language.h"

class AccessPath :	public Wt::Dbo::Dbo<AccessPath>
{
	public:
		std::string HostName;
		std::string InternalPath;
		Wt::Dbo::ptr<Language> LanguagePtr;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, HostName, "HostName", 256);
			Wt::Dbo::field(a, InternalPath, "InternalPath", 256);
			Wt::Dbo::belongsTo(a, LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "accesspaths";
		}
};

#endif
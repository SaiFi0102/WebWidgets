#ifndef ACCESSPATH_DBO_H
#define ACCESSPATH_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Language.h"
#include "Dbo/Page.h"

class BaseAccessPath
{
	public:
		std::string HostName;
		std::string InternalPath;
		bool HasChildPaths;

	protected:
		BaseAccessPath()
			: HasChildPaths(false)
		{ }
};
class AccessPathData : public BaseAccessPath, public DataSurrogateKey
{
	public:
		std::string LanguageCode;

		AccessPathData(long long id)
			: DataSurrogateKey(id)
		{ }
};
class AccessPath : public BaseAccessPath
{
	public:
		Wt::Dbo::ptr<Page> PagePtr;
		Wt::Dbo::ptr<Language> LanguagePtr;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, HostName, "HostName", 256);
			Wt::Dbo::field(a, InternalPath, "InternalPath", 256);
			Wt::Dbo::field(a, HasChildPaths, "HasChildPaths");
			Wt::Dbo::belongsTo(a, LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, PagePtr, "accesspath", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "accesspaths";
		}
};

#endif
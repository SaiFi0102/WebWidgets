#ifndef ACCESSPATH_DBO_H
#define ACCESSPATH_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Language.h"
#include "Dbo/Page.h"

class AccessHostName
{
	public:
		std::string HostName() const { return _HostName; }
		Wt::Dbo::ptr<Language> LanguagePtr;
		Wt::Dbo::ptr<Page> DefaultPagePtr;
		bool MobileMode;
		std::string MobileInternalPath;

		PageAccessPathCollections PageAccessPathCollection;
		LanguageAccessPathCollections LanguageAccessPathCollection;

		AccessHostName(const std::string &HostName = "")
			: _HostName(HostName), MobileMode(false)
		{ }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _HostName, "HostName", 255);
			Wt::Dbo::belongsTo(a, LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, DefaultPagePtr, "DefaultPage", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, MobileMode, "MobileMode");
			Wt::Dbo::field(a, MobileInternalPath, "MobileInternalPath", 255);
			Wt::Dbo::hasMany(a, PageAccessPathCollection, Wt::Dbo::ManyToOne, "Access");
			Wt::Dbo::hasMany(a, LanguageAccessPathCollection, Wt::Dbo::ManyToOne, "Access");
		}
		static const char *TableName()
		{
			return "accesshostnames";
		}

	protected:
		std::string _HostName;
};
class AccessHostNameData
{
	public:
		std::string HostName;
		std::string LanguageCode;
		long long DefaultPageId;
		bool MobileMode;
		std::string MobileInternalPath;

		AccessHostNameData(Wt::Dbo::ptr<AccessHostName> Ptr)
			: HostName(Ptr.id()),
			LanguageCode(Ptr->LanguagePtr.id()),
			DefaultPageId(Ptr->DefaultPagePtr.id()),
			MobileMode(Ptr->MobileMode),
			MobileInternalPath(Ptr->MobileInternalPath)
		{ }
};

class BaseAccessPath
{
	public:
		std::string InternalPath;
		Wt::Dbo::ptr<AccessHostName> AccessHostNamePtr;
};

class PageAccessPath : public BaseAccessPath
{
	public:
		Wt::Dbo::ptr<Page> PagePtr;

		Wt::Dbo::ptr<PageAccessPath> ParentAccessPathPtr;
		PageAccessPathCollections ChildrenAccessPaths;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, AccessHostNamePtr, "Access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, InternalPath, "InternalPath", 255);
			Wt::Dbo::belongsTo(a, PagePtr, "Page", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);

			Wt::Dbo::hasMany(a, ChildrenAccessPaths, Wt::Dbo::ManyToOne, "Parent_AccessPath");
			Wt::Dbo::belongsTo(a, ParentAccessPathPtr, "Parent_AccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "pageaccesspaths";
		}
};
class PageAccessPathData : public DataSurrogateKey
{
	public:
		std::string HostName;
		std::string InternalPath;
		long long PageId;
		long long ParentAccessPathId;

		PageAccessPathData(Wt::Dbo::ptr<PageAccessPath> Ptr)
			: DataSurrogateKey(Ptr.id()),
			HostName(Ptr->AccessHostNamePtr.id()),
			InternalPath(Ptr->InternalPath),
			PageId(Ptr->PagePtr.id()),
			ParentAccessPathId(Ptr->ParentAccessPathPtr.id())
		{ }
};

class LanguageAccessPath : public BaseAccessPath
{
	public:
		Wt::Dbo::ptr<Language> LanguagePtr;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, AccessHostNamePtr, "Access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, InternalPath, "InternalPath", 255);
			Wt::Dbo::belongsTo(a, LanguagePtr, "Language", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "languageaccesspaths";
		}
};
class LanguageAccessPathData : public DataSurrogateKey
{
	public:
		std::string HostName;
		std::string InternalPath;
		std::string LanguageCode;

		LanguageAccessPathData(Wt::Dbo::ptr<LanguageAccessPath> Ptr)
			: DataSurrogateKey(Ptr.id()),
			HostName(Ptr->AccessHostNamePtr.id()),
			InternalPath(Ptr->InternalPath),
			LanguageCode(Ptr->LanguagePtr.id())
		{ }
};

#endif
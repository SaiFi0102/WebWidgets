#ifndef ACCESSPATH_DBO_H
#define ACCESSPATH_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Language.h"
#include "Dbo/Page.h"
#include "Dbo/Style.h"

class AccessHostName
{
	public:
		std::string HostName() const { return _HostName; }
		Wt::Dbo::ptr<Language> LanguagePtr;
		Wt::Dbo::ptr<Page> DefaultPagePtr;
		Wt::Dbo::ptr<Style> StylePtr;
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
			Wt::Dbo::belongsTo(a, LanguagePtr, "Language", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, DefaultPagePtr, "DefaultPage", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, StylePtr, "Style", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
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
		long long StyleId;
		bool MobileMode;
		std::string MobileInternalPath;

		AccessHostNameData(Wt::Dbo::ptr<AccessHostName> Ptr)
			: HostName(Ptr.id()),
			LanguageCode(Ptr->LanguagePtr.id()),
			DefaultPageId(Ptr->DefaultPagePtr.id()),
			StyleId(Ptr->StylePtr.id()),
			MobileMode(Ptr->MobileMode),
			MobileInternalPath(Ptr->MobileInternalPath)
		{ }
};

class BaseAccessPath
{
	public:
		std::string InternalPath() const { return _InternalPath; }
		Wt::Dbo::ptr<AccessHostName> AccessHostNamePtr() const{ return _AccessHostNamePtr; }

		BaseAccessPath(Wt::Dbo::ptr<AccessHostName> AccessHostNamePtr, const std::string &InternalPath)
			: _AccessHostNamePtr(AccessHostNamePtr), _InternalPath(InternalPath)
		{ }
	
	protected:
		std::string _InternalPath;
		Wt::Dbo::ptr<AccessHostName> _AccessHostNamePtr;
};

class PageAccessPath : public BaseAccessPath
{
	public:
		Wt::Dbo::ptr<Page> PagePtr;

		Wt::Dbo::ptr<PageAccessPath> ParentAccessPathPtr() const { return _ParentAccessPathPtr; }
		PageAccessPathCollections ChildrenAccessPaths;

		PageAccessPath(Wt::Dbo::ptr<AccessHostName> AccessHostNamePtr = Wt::Dbo::ptr<AccessHostName>(),
			const std::string &InternalPath = "",
			Wt::Dbo::ptr<PageAccessPath> ParentAccessPathPtr = Wt::Dbo::ptr<PageAccessPath>())
			: BaseAccessPath(AccessHostNamePtr, InternalPath), _ParentAccessPathPtr(ParentAccessPathPtr)
		{ }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _AccessHostNamePtr, "Access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, _InternalPath, "InternalPath", 255);
			Wt::Dbo::belongsTo(a, PagePtr, "Page", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);

			Wt::Dbo::hasMany(a, ChildrenAccessPaths, Wt::Dbo::ManyToOne, "Parent_AccessPath");
			Wt::Dbo::belongsTo(a, _ParentAccessPathPtr, "Parent_AccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
		}
		static const char *TableName()
		{
			return "pageaccesspaths";
		}

	protected:
		Wt::Dbo::ptr<PageAccessPath> _ParentAccessPathPtr;
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
			HostName(Ptr->AccessHostNamePtr().id()),
			InternalPath(Ptr->InternalPath()),
			PageId(Ptr->PagePtr.id()),
			ParentAccessPathId(Ptr->ParentAccessPathPtr().id())
		{ }
};

class LanguageAccessPath : public BaseAccessPath
{
	public:
		Wt::Dbo::ptr<Language> LanguagePtr;

		LanguageAccessPath(Wt::Dbo::ptr<AccessHostName> AccessHostNamePtr = Wt::Dbo::ptr<AccessHostName>(),
			const std::string &InternalPath = "")
			: BaseAccessPath(AccessHostNamePtr, InternalPath)
		{ }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _AccessHostNamePtr, "Access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, _InternalPath, "InternalPath", 255);
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
			HostName(Ptr->AccessHostNamePtr().id()),
			InternalPath(Ptr->InternalPath()),
			LanguageCode(Ptr->LanguagePtr.id())
		{ }
};

#endif
#ifndef WW_DBO_ACCESSPATH_H
#define WW_DBO_ACCESSPATH_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{
		class AccessHostName
		{
		public:
			std::string hostName() const { return _hostName; }
			ptr<Language> languagePtr;
			ptr<Page> defaultPagePtr;
			ptr<Style> stylePtr;
			bool mobileMode = false;
			std::string mobileInternalPath;

			PageAccessPathCollection pageAccessPathCollection;
			LanguageAccessPathCollection languageAccessPathCollection;
			NavigationMenuItemCollection shownOnPageMenuItemCollection;

			AccessHostName() = default;
			AccessHostName(const std::string &hostName)
				: _hostName(hostName)
			{ }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _hostName, "hostName", 255);
				Wt::Dbo::belongsTo(a, languagePtr, "language", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::belongsTo(a, defaultPagePtr, "defaultPage", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::belongsTo(a, stylePtr, "style", Wt::Dbo::OnDeleteSetNull | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::field(a, mobileMode, "mobileMode");
				Wt::Dbo::field(a, mobileInternalPath, "mobileInternalPath", 255);
				Wt::Dbo::hasMany(a, pageAccessPathCollection, Wt::Dbo::ManyToOne, "access");
				Wt::Dbo::hasMany(a, languageAccessPathCollection, Wt::Dbo::ManyToOne, "access");
				Wt::Dbo::hasMany(a, shownOnPageMenuItemCollection, Wt::Dbo::ManyToMany, "accesshost_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "accesshostname";
			}

		protected:
			std::string _hostName;
		};

		class BaseAccessPath
		{
		public:
			std::string internalPath() const { return _internalPath; }
			ptr<AccessHostName> accessHostNamePtr() const { return _accessHostNamePtr; }

		protected:
			BaseAccessPath() = default;
			BaseAccessPath(ptr<AccessHostName> accessHostNamePtr, const std::string &internalPath)
				: _accessHostNamePtr(accessHostNamePtr), _internalPath(internalPath)
			{ }

			std::string _internalPath;
			ptr<AccessHostName> _accessHostNamePtr;
		};

		class PageAccessPath : public BaseAccessPath
		{
		public:
			ptr<Page> pagePtr;
			NavigationMenuItemCollection pageMenuItemCollection;
			MenuItemOnPageAccessPathCollection menuItemOnPageAccessPathCollection;

			ptr<PageAccessPath> parentAccessPathPtr() const { return _parentAccessPathPtr; }
			PageAccessPathCollection childrenAccessPaths;

			PageAccessPath() = default;
			PageAccessPath(ptr<AccessHostName> accessHostNamePtr, const std::string &internalPath, ptr<PageAccessPath> parentAccessPathPtr = ptr<PageAccessPath>())
				: BaseAccessPath(accessHostNamePtr, internalPath), _parentAccessPathPtr(parentAccessPathPtr)
			{ }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, _accessHostNamePtr, "access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::field(a, _internalPath, "internalPath", 255);
				Wt::Dbo::belongsTo(a, pagePtr, "page", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::hasMany(a, pageMenuItemCollection, Wt::Dbo::ManyToOne, "accessPath");
				Wt::Dbo::hasMany(a, menuItemOnPageAccessPathCollection, Wt::Dbo::ManyToOne, "pageAccessPath");

				Wt::Dbo::hasMany(a, childrenAccessPaths, Wt::Dbo::ManyToOne, "parent_accessPath");
				Wt::Dbo::belongsTo(a, _parentAccessPathPtr, "parent_accessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			}
			constexpr static const char *tableName()
			{
				return "pageaccesspath";
			}

		protected:
			ptr<PageAccessPath> _parentAccessPathPtr;
		};

		class LanguageAccessPath : public BaseAccessPath
		{
		public:
			ptr<Language> languagePtr;

			LanguageAccessPath() = default;
			LanguageAccessPath(ptr<AccessHostName> accessHostNamePtr, const std::string &internalPath)
				: BaseAccessPath(accessHostNamePtr, internalPath)
			{ }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, _accessHostNamePtr, "access", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
				Wt::Dbo::field(a, _internalPath, "internalPath", 255);
				Wt::Dbo::belongsTo(a, languagePtr, "language", Wt::Dbo::NotNull | Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			}
			constexpr static const char *tableName()
			{
				return "languageaccesspath";
			}
		};
	}

	namespace Ddo
	{
		class AccessHostName
		{
		public:
			std::string hostName;
			std::string languageCode;
			long long defaultPageId;
			long long styleId;
			bool mobileMode;
			std::string mobileInternalPath;

			AccessHostName(Dbo::ptr<Dbo::AccessHostName> ptr)
				: hostName(ptr.id()),
				languageCode(ptr->languagePtr.id()),
				defaultPageId(ptr->defaultPagePtr.id()),
				styleId(ptr->stylePtr.id()),
				mobileMode(ptr->mobileMode),
				mobileInternalPath(ptr->mobileInternalPath)
			{ }
		};

		class PageAccessPath : public SurrogateKey
		{
		public:
			std::string hostName;
			std::string internalPath;
			std::string fullPath;
			long long pageId;
			long long parentAccessPathId;

			PageAccessPath(Dbo::ptr<Dbo::PageAccessPath> ptr)
				: SurrogateKey(ptr.id()),
				hostName(ptr->accessHostNamePtr().id()),
				internalPath(ptr->internalPath()),
				pageId(ptr->pagePtr.id()),
				parentAccessPathId(ptr->parentAccessPathPtr().id())
			{
				fullPath = internalPath;
				Dbo::ptr<Dbo::PageAccessPath> parentAccessPathPtr = ptr->parentAccessPathPtr();
				while (parentAccessPathPtr)
				{
					fullPath.insert(0, parentAccessPathPtr->internalPath() + "/");
					parentAccessPathPtr = parentAccessPathPtr->parentAccessPathPtr();
				}
			}
		};

		class LanguageAccessPath : public SurrogateKey
		{
		public:
			std::string hostName;
			std::string internalPath;
			std::string languageCode;

			LanguageAccessPath(Dbo::ptr<Dbo::LanguageAccessPath> Ptr)
				: SurrogateKey(Ptr.id()),
				hostName(Ptr->accessHostNamePtr().id()),
				internalPath(Ptr->internalPath()),
				languageCode(Ptr->languagePtr.id())
			{ }
		};
	}
}

#endif
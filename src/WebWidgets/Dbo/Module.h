#ifndef MODULE_DBO_H
#define MODULE_DBO_H

#include "DboTraits.h"

class BaseModule
{
	public:
		std::string Name;
		int VersionSeries;
		int VersionMajor;
		int VersionMinor;

	protected:
		BaseModule(long long id = -1)
			: _id(id), VersionSeries(-1), VersionMajor(-1), VersionMinor(-1)
		{};

		long long _id;
};

class Module : public BaseModule
{
	public:
		ConfigurationCollections ConfigurationCollection;
		PageCollections PageCollection;
		TemplateCollections TemplateCollection;
		SingularKeyCollections SingularKeyCollection;
		PluralKeyCollections PluralKeyCollection;

		Wt::Dbo::ptr<Author> AuthorPtr;

		Module() { };

		Module(long long id)
			: BaseModule(id)
		{};

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _id);
			Wt::Dbo::field(a, Name,			"Name", 255);
			Wt::Dbo::field(a, VersionSeries,"VersionSeries");
			Wt::Dbo::field(a, VersionMajor,	"VersionMajor");
			Wt::Dbo::field(a, VersionMinor,	"VersionMinor");

			Wt::Dbo::belongsTo(a, AuthorPtr, "Author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::hasMany(a, ConfigurationCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, PageCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, TemplateCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, SingularKeyCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, PluralKeyCollection, Wt::Dbo::ManyToOne, "Module");
		}
		static const char *TableName()
		{
			return "modules";
		}
};

class ModuleData : public BaseModule
{
public:
	long long AuthorId;

	ModuleData(Wt::Dbo::ptr<Module> Ptr)
		: BaseModule(*Ptr), AuthorId(Ptr->AuthorPtr.id())
	{ }

	long long id() const
	{
		return _id;
	}
};

#include "Dbo/Configuration.h"
#include "Dbo/Page.h"
#include "Dbo/Style.h"
#include "Dbo/Language.h"
#include "Dbo/Author.h"

#endif
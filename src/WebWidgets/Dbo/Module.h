#ifndef MODULE_DBO_H
#define MODULE_DBO_H

#include <Wt/Dbo/Dbo>

class Configuration;
class Module;
class Page;
class Template;
class LanguageSingle;
class LanguagePlural;
class Author;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< Configuration > > ConfigurationCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< Module > > ModuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< Page > > PageCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< Template > > TemplateCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< LanguageSingle > > LanguageSingleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< LanguagePlural > > LanguagePluralCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< Author > > AuthorCollections;

template<>
struct Wt::Dbo::dbo_traits<Author> : public Wt::Dbo::dbo_default_traits
{
	static const char *surrogateIdField() { return 0; }
};

template<>
struct Wt::Dbo::dbo_traits<Module> : public Wt::Dbo::dbo_default_traits
{
	static const char *surrogateIdField() { return 0; }
};

class Module : public Wt::Dbo::Dbo<Module>
{
	public:
		std::string Name;
		int VersionSeries;
		int VersionMajor;
		int VersionMinor;

		ConfigurationCollections ConfigurationCollection;
		PageCollections PageCollection;
		TemplateCollections TemplateCollection;
		LanguageSingleCollections LanguageSingleCollection;
		LanguagePluralCollections LanguagePluralCollection;

		Wt::Dbo::ptr<Author> AuthorPtr;

		Module()
			: _id(-1), VersionSeries(-1), VersionMajor(-1), VersionMinor(-1)
		{};

		Module(long long id)
			: _id(id), VersionSeries(-1), VersionMajor(-1), VersionMinor(-1)
		{};

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _id);
			Wt::Dbo::field(a, Name,			"Name", 256);
			Wt::Dbo::field(a, VersionSeries,"VersionSeries");
			Wt::Dbo::field(a, VersionMajor,	"VersionMajor");
			Wt::Dbo::field(a, VersionMinor,	"VersionMinor");

			Wt::Dbo::belongsTo(a, AuthorPtr, "Author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::hasMany(a, ConfigurationCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, PageCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, TemplateCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, LanguageSingleCollection, Wt::Dbo::ManyToOne, "Module");
			Wt::Dbo::hasMany(a, LanguagePluralCollection, Wt::Dbo::ManyToOne, "Module");
		}
		static const char *TableName()
		{
			return "modules";
		}

	private:
		long long _id;
};

#include "Dbo/Configuration.h"
#include "Dbo/Page.h"
#include "Dbo/Style.h"
#include "Dbo/Language.h"
#include "Dbo/Author.h"

#endif
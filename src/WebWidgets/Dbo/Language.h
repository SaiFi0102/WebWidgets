#ifndef LANGUAGE_DBO_H
#define LANGUAGE_DBO_H

#include <Wt/Dbo/Dbo>
#include "Dbo/Module.h"

class Language;
class LanguageSingle;
class LanguagePlural;
struct LanguageSingleKeys;
struct LanguagePluralKeys;

typedef Wt::Dbo::collection< Wt::Dbo::ptr< Language > > LanguageCollections;
namespace Wt
{
	namespace Dbo
	{
		//Overloaded Wt::Dbo::field() for Language Single Keys structure
		template<class Action>
		void field(Action &action, LanguageSingleKeys &Keys, const std::string &name, int size = -1)
		{
			belongsTo(action, Keys.LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			field(action, Keys.Key, "Key", 256);
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		};

		//Overloaded Wt::Dbo::field() for Language Plural Keys structure
		template<class Action>
		void field(Action &action, LanguagePluralKeys &Keys, const std::string &name, int size = -1)
		{
			belongsTo(action, Keys.LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			field(action, Keys.Key, "Key", 256);
			field(action, Keys.Case, "Case");
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		};

		//Overloaded dbo_traits for Language DBO
		template<>
		struct dbo_traits<Language> : public dbo_default_traits
		{
			typedef std::string IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};

		//Overloaded dbo_traits for LanguageSingle DBO
		template<>
		struct dbo_traits<LanguageSingle> : public dbo_default_traits
		{
			typedef LanguageSingleKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};

		//Overloaded dbo_traits for LanguagePlural DBO
		template<>
		struct dbo_traits<LanguagePlural> : public dbo_default_traits
		{
			typedef LanguagePluralKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
}

//Language Single Keys structure
struct LanguageSingleKeys
{
	Wt::Dbo::ptr<Language>	LanguagePtr;
	std::string				Key;
	Wt::Dbo::ptr<Module>	ModulePtr;

	LanguageSingleKeys();
	LanguageSingleKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr);

	bool operator< (const LanguageSingleKeys &other) const;
	bool operator== (const LanguageSingleKeys &other) const;
};

struct LanguagePluralKeys : public LanguageSingleKeys
{
	int Case; //Case ID (for example 1 = singular, 2 = plural)

	LanguagePluralKeys();
	LanguagePluralKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr, int Case);

	bool operator< (const LanguagePluralKeys &other) const;
	bool operator== (const LanguagePluralKeys &other) const;
};

//Overloaded operator<< for Language Single Keys structure
std::ostream &operator<< (std::ostream &o, const LanguageSingleKeys &c);

//Overloaded operator<< for Language Plural Keys structure
std::ostream &operator<< (std::ostream &o, const LanguagePluralKeys &c);

class Language : public Wt::Dbo::Dbo<Language>
{
	public:
		std::string	Code; //en, cn, eng, urd, ur
		std::string	Name; //English, 中文 (Chinese), Русский (Russian)
		std::string	LanguageAccept; //en*, wildcard allowed
		std::string	PluralExpression; //For most languages "n > 1 ? 2 : 1" where n = number of that item provided, 2 : 1, 1 means case 1(singular) and case 2(plural)
		int			PluralCases; //In English and most languages there are 2, Singular and Plural, for example file and files

		Language();

		//hasMany
		LanguageSingleCollections LanguageSingleCollection;
		LanguagePluralCollections LanguagePluralCollection;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, Code,				"Code", 3);
			Wt::Dbo::field(a, Name,				"Name", 256);
			Wt::Dbo::field(a, LanguageAccept,	"LanguageAccept", 256);
			Wt::Dbo::field(a, PluralExpression,	"PluralExpression");
			Wt::Dbo::field(a, PluralCases,		"PluralCases");

			Wt::Dbo::hasMany(a, LanguageSingleCollection, Wt::Dbo::ManyToOne, "Language");
			Wt::Dbo::hasMany(a, LanguagePluralCollection, Wt::Dbo::ManyToOne, "Language");
		}
		static const char *TableName()
		{
			return "languages";
		}
};

class LanguageSingle : public Wt::Dbo::Dbo<LanguageSingle>
{
	public:
		std::string String;
		bool IsEmail;

		LanguageSingle();
		LanguageSingle(LanguageSingleKeys Key);
		LanguageSingle(const std::string &Key, const std::string &String = "", Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>(), Wt::Dbo::ptr<Language> LanguagePtr = Wt::Dbo::ptr<Language>(), bool IsEmail = false);

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Language");
			Wt::Dbo::field(a, String, "String");
			Wt::Dbo::field(a, IsEmail, "IsEmail");
		}
		static const char *TableName()
		{
			return "languagesingles";
		}

	private:
		LanguageSingleKeys _Id;
};

class LanguagePlural : public Wt::Dbo::Dbo<LanguagePlural>
{
	public:
		std::string String;

		LanguagePlural();
		LanguagePlural(LanguagePluralKeys Key);

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Language");
			Wt::Dbo::field(a, String, "String");
		}
		static const char *TableName()
		{
			return "languageplurals";
		}

	private:
		LanguagePluralKeys _Id;
};

#endif
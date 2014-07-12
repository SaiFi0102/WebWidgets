#ifndef LANGUAGE_DBO_H
#define LANGUAGE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"
#include <Wt/WString>
#include <Wt/Dbo/WtSqlTraits>

//Language Single Keys structure
struct LanguageSingleKeys
{
	Wt::Dbo::ptr<Language>	LanguagePtr;
	std::string				Key;
	Wt::Dbo::ptr<Module>	ModulePtr;

	LanguageSingleKeys()
	{ }
	LanguageSingleKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr)
		: LanguagePtr(LanguagePtr), Key(Key), ModulePtr(ModulePtr)
	{ }

	bool operator< (const LanguageSingleKeys &other) const;
	bool operator== (const LanguageSingleKeys &other) const
	{
		return LanguagePtr == other.LanguagePtr && Key == other.Key && ModulePtr == other.ModulePtr;
	}
};

struct LanguagePluralKeys : public LanguageSingleKeys
{
	int Case; //Case ID (for example 1 = singular, 2 = plural)

	LanguagePluralKeys()
		: Case(-1)
	{ }
	LanguagePluralKeys(Wt::Dbo::ptr<Language> LanguagePtr, const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr, int Case)
		: LanguageSingleKeys(LanguagePtr, Key, ModulePtr), Case(Case)
	{ }

	bool operator< (const LanguagePluralKeys &other) const;
	bool operator== (const LanguagePluralKeys &other) const
	{
		return LanguagePtr == other.LanguagePtr && Key == other.Key && Case == other.Case && ModulePtr == other.ModulePtr;
	}
};

//Overloaded operator<< for Language Single Keys structure
std::ostream &operator<< (std::ostream &o, const LanguageSingleKeys &c);

//Overloaded operator<< for Language Plural Keys structure
std::ostream &operator<< (std::ostream &o, const LanguagePluralKeys &c);

class Language : public Wt::Dbo::Dbo<Language>
{
	public:
		std::string	Code; //en, zh, ur, ISO 639-1: two-letter code
		Wt::WString	Name; //English, Chinese, Russian
		Wt::WString NativeName; // English, 中文, Русский
		std::string	LanguageAccept; //en*, wildcard allowed
		std::string	PluralExpression; //For most languages "n > 1 ? 2 : 1" where n = number of that item provided, 2 : 1, 1 means case 1(singular) and case 2(plural)
		int			PluralCases; //In English and most languages there are 2, Singular and Plural, for example file and files
		bool		Installed; //Weather its installed or just for the name

		Language()
			: PluralCases(-1), Installed(false)
		{ }
		Language(const std::string &Code, const Wt::WString &Name, const Wt::WString &NativeName, const std::string &LanguageAccept, const std::string &PluralExpression, int PluralCases, bool Installed)
			: Code(Code), Name(Name), NativeName(NativeName), LanguageAccept(LanguageAccept), PluralExpression(PluralExpression), PluralCases(PluralCases), Installed(Installed)
		{ }

		//hasMany
		AccessPathCollections AccessPathCollection;
		LanguageSingleCollections LanguageSingleCollection;
		LanguagePluralCollections LanguagePluralCollection;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, Code,				"Code", 2);
			Wt::Dbo::field(a, Name,				"Name", 256);
			Wt::Dbo::field(a, NativeName,		"NativeName", 256);
			Wt::Dbo::field(a, LanguageAccept,	"LanguageAccept", 256);
			Wt::Dbo::field(a, PluralExpression,	"PluralExpression");
			Wt::Dbo::field(a, PluralCases,		"PluralCases");
			Wt::Dbo::field(a, Installed,		"Installed");

			Wt::Dbo::hasMany(a, AccessPathCollection, Wt::Dbo::ManyToOne, "Language");
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

		LanguageSingle()
			: IsEmail(false)
		{ }
		LanguageSingle(LanguageSingleKeys &Key)
			: _Id(Key), IsEmail(false)
		{ }
		LanguageSingle(const std::string &Key, const std::string &String = "", Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>(), Wt::Dbo::ptr<Language> LanguagePtr = Wt::Dbo::ptr<Language>(), bool IsEmail = false)
			: _Id(LanguagePtr, Key, ModulePtr), String(String), IsEmail(IsEmail)
		{ }

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

		LanguagePlural() { }
		LanguagePlural(LanguagePluralKeys Key)
			: _Id(Key)
		{ }

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

#include "Dbo/AccessPath.h"

#endif
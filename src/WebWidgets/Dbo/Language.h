#ifndef LANGUAGE_DBO_H
#define LANGUAGE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"
#include <Wt/WString>
#include <Wt/Dbo/WtSqlTraits>

class BaseLanguage
{
	protected:
		std::string _Code; //en, zh, ur, ISO 639-1: two-letter code

	public:
		Wt::WString	Name; //English, Chinese, Russian
		Wt::WString NativeName; // English, 中文, Русский
		std::string	LanguageAccept; //en*, wildcard allowed
		std::string	PluralExpression; //For most languages "n > 1 ? 2 : 1" where n = number of that item provided, 2 : 1, 1 means case 1(singular) and case 2(plural)
		int			PluralCases; //In English and most languages there are 2, Singular and Plural, for example file and files
		bool		Installed; //Weather its installed or just for the name

		std::string Code() const { return _Code; }

		BaseLanguage(const std::string &Code = "")
			: _Code(Code), PluralCases(-1), Installed(false)
		{ }
		BaseLanguage(const std::string &Code, const Wt::WString &Name, const Wt::WString &NativeName, const std::string &LanguageAccept, const std::string &PluralExpression, int PluralCases, bool Installed)
			: _Code(Code), Name(Name), NativeName(NativeName), LanguageAccept(LanguageAccept), PluralExpression(PluralExpression), PluralCases(PluralCases), Installed(Installed)
		{ }
};
class Language : public BaseLanguage
{
	public:
		Language() { }
		Language(const std::string &Code)
			: BaseLanguage(Code)
		{ }
		Language(const std::string &Code, const Wt::WString &Name, const Wt::WString &NativeName, const std::string &LanguageAccept, const std::string &PluralExpression, int PluralCases, bool Installed)
			: BaseLanguage(Code, Name, NativeName, LanguageAccept, PluralExpression, PluralCases, Installed)
		{ }

		//hasMany
		AccessHostNameCollections AccessHostNameCollection;
		LanguageAccessPathCollections LanguageAccessPathCollection;
		LanguageSingleCollections LanguageSingleCollection;
		LanguagePluralCollections LanguagePluralCollection;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Code,				"Code", 2);
			Wt::Dbo::field(a, Name,				"Name", 255);
			Wt::Dbo::field(a, NativeName,		"NativeName", 255);
			Wt::Dbo::field(a, LanguageAccept,	"LanguageAccept", 255);
			Wt::Dbo::field(a, PluralExpression,	"PluralExpression");
			Wt::Dbo::field(a, PluralCases,		"PluralCases");
			Wt::Dbo::field(a, Installed,		"Installed");

			Wt::Dbo::hasMany(a, AccessHostNameCollection, Wt::Dbo::ManyToOne, "Language");
			Wt::Dbo::hasMany(a, LanguageAccessPathCollection, Wt::Dbo::ManyToOne, "Language");
			Wt::Dbo::hasMany(a, LanguageSingleCollection, Wt::Dbo::ManyToOne, "Language");
			Wt::Dbo::hasMany(a, LanguagePluralCollection, Wt::Dbo::ManyToOne, "Language");
		}
		static const char *TableName()
		{
			return "languages";
		}
};
class LanguageData : public BaseLanguage
{
	public:
		LanguageData(Wt::Dbo::ptr<Language> Ptr)
			: BaseLanguage(*Ptr)
		{ }
};

class BaseLanguageSingle
{
	protected:
		std::string _Key;

		BaseLanguageSingle()
			: IsEmail(false)
		{ }
		BaseLanguageSingle(const std::string &Key, const std::string &String = "", bool IsEmail = false)
			: _Key(Key), String(String), IsEmail(IsEmail)
		{ }

	public:
		std::string String;
		bool IsEmail;

		std::string Key() const { return _Key; }
};
class LanguageSingle : public BaseLanguageSingle
{
	protected:
		Wt::Dbo::ptr<Language> _LanguagePtr;
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		LanguageSingle() { }
		LanguageSingle(const std::string &Key, const std::string &String = "", Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>(), Wt::Dbo::ptr<Language> LanguagePtr = Wt::Dbo::ptr<Language>(), bool IsEmail = false)
			: BaseLanguageSingle(Key, String, IsEmail), _LanguagePtr(LanguagePtr), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Language> LanguagePtr() const { return _LanguagePtr; };
		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, _Key, "Key", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, String, "String");
			Wt::Dbo::field(a, IsEmail, "IsEmail");
		}
		static const char *TableName()
		{
			return "languagesingles";
		}
};
class LanguageSingleData : public BaseLanguageSingle, public DataSurrogateKey
{
	protected:
		std::string _LanguageCode;
		long long _ModuleId;

	public:
		LanguageSingleData(Wt::Dbo::ptr<LanguageSingle> Ptr)
			: BaseLanguageSingle(*Ptr), _LanguageCode(Ptr->LanguagePtr().id()),
			_ModuleId(Ptr->ModulePtr().id()), DataSurrogateKey(Ptr.id())
		{ }

		std::string LanguageCode() const { return _LanguageCode; };
		long long ModuleId() const { return _ModuleId; };
};

class BaseLanguagePlural
{
	protected:
		std::string _Key;
		int _Case;

		BaseLanguagePlural()
			: _Case(-1)
		{ }
		BaseLanguagePlural(const std::string &Key, int Case, const std::string &String = "")
			: _Key(Key), _Case(Case), String(String)
		{ }

	public:
		std::string String;

		std::string Key() const { return _Key; };
		int Case() const { return _Case; };
};
class LanguagePlural : public BaseLanguagePlural
{
	protected:
		Wt::Dbo::ptr<Language> _LanguagePtr;
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		LanguagePlural() { }
		LanguagePlural(const std::string &Key, int Case, Wt::Dbo::ptr<Module> ModulePtr, Wt::Dbo::ptr<Language> LanguagePtr, const std::string &String = "")
			: BaseLanguagePlural(Key, Case, String), _LanguagePtr(LanguagePtr), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Language> LanguagePtr() const { return _LanguagePtr; };
		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, _Key, "Key", 50);
			Wt::Dbo::field(a, _Case, "Case");
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, String, "String");
		}
		static const char *TableName()
		{
			return "languageplurals";
		}
};
class LanguagePluralData : public BaseLanguagePlural, public DataSurrogateKey
{
	protected:
		std::string _LanguageCode;
		long long _ModuleId;

	public:
		LanguagePluralData(Wt::Dbo::ptr<LanguagePlural> Ptr)
			: BaseLanguagePlural(*Ptr), _LanguageCode(Ptr->LanguagePtr().id()),
			_ModuleId(Ptr->ModulePtr().id()), DataSurrogateKey(Ptr.id())
		{ }

		std::string LanguageCode() const { return _LanguageCode; };
		long long ModuleId() const { return _ModuleId; };
};

#include "Dbo/AccessPath.h"

#endif
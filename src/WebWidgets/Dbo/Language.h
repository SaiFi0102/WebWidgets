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
		SingularStringCollections SingularStringCollection;
		PluralStringCollections PluralStringCollection;

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
			Wt::Dbo::hasMany(a, SingularStringCollection, Wt::Dbo::ManyToOne, "Language");
			Wt::Dbo::hasMany(a, PluralStringCollection, Wt::Dbo::ManyToOne, "Language");
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

class SingularKey
{
	protected:
		std::string _Key;
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		SingularStringCollections SingularStringCollection;
		PageCollections PageCollection;
		ConfigurationCollections ConfTitleCollection;
		ConfigurationCollections ConfDetailsCollection;
		ConfigurationCollections ConfExpertWarningCollection;
		//NavigationMenuItemCollections ShownOnPageMenuItemCollection;

		SingularKey() { }
		SingularKey(const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>())
			: _Key(Key), _ModulePtr(ModulePtr)
		{ }

		std::string Key() const { return _Key; }
		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _Key, "Key", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::hasMany(a, SingularStringCollection, Wt::Dbo::ManyToOne, "Key");
			Wt::Dbo::hasMany(a, PageCollection, Wt::Dbo::ManyToOne, "TitleKey");
			Wt::Dbo::hasMany(a, ConfTitleCollection, Wt::Dbo::ManyToOne, "TitleKey");
			Wt::Dbo::hasMany(a, ConfDetailsCollection, Wt::Dbo::ManyToOne, "DetailsKey");
			Wt::Dbo::hasMany(a, ConfExpertWarningCollection, Wt::Dbo::ManyToOne, "ExpertWarningKey");
			//Wt::Dbo::hasMany(a, ShownOnPageMenuItemCollection, Wt::Dbo::ManyToOne, "ShowOnLanguage");
		}
		static const char *TableName()
		{
			return "languagesingularkeys";
		}
};

class BaseSingularString
{
	protected:
		BaseSingularString(const std::string &String = "", bool IsEmail = false)
			: String(String), IsEmail(IsEmail)
		{ }

	public:
		std::string String;
		bool IsEmail;
};
class SingularString : public BaseSingularString
{
	protected:
		Wt::Dbo::ptr<Language> _LanguagePtr;
		Wt::Dbo::ptr<SingularKey> _KeyPtr;

	public:
		SingularString() { }
		SingularString(Wt::Dbo::ptr<SingularKey> KeyPtr, const std::string &String = "", bool IsEmail = false, Wt::Dbo::ptr<Language> LanguagePtr = Wt::Dbo::ptr<Language>())
			: BaseSingularString(String, IsEmail), _LanguagePtr(LanguagePtr), _KeyPtr(KeyPtr)
		{ }

		Wt::Dbo::ptr<Language> LanguagePtr() const { return _LanguagePtr; };
		Wt::Dbo::ptr<SingularKey> KeyPtr() const { return _KeyPtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(a, _KeyPtr, "Key", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, String, "String");
			Wt::Dbo::field(a, IsEmail, "IsEmail");
		}
		static const char *TableName()
		{
			return "languagesingularstrings";
		}
};
class SingularStringData : public BaseSingularString, public DataSurrogateKey
{
	protected:
		std::string _Key;
		std::string _LanguageCode;
		long long _ModuleId;

	public:
		SingularStringData(Wt::Dbo::ptr<SingularString> Ptr)
			: BaseSingularString(*Ptr), _LanguageCode(Ptr->LanguagePtr().id()),
			_Key(Ptr->KeyPtr()->Key()), _ModuleId(Ptr->KeyPtr()->ModulePtr().id()),
			DataSurrogateKey(Ptr.id())
		{ }

		std::string Key() const { return _Key; };
		std::string LanguageCode() const { return _LanguageCode; };
		long long ModuleId() const { return _ModuleId; };
};

class PluralKey
{
	protected:
		std::string _Key;
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		PluralStringCollections PluralStringCollection;

		PluralKey() { }
		PluralKey(const std::string &Key, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>())
			: _Key(Key), _ModulePtr(ModulePtr)
		{ }

		std::string Key() const { return _Key; }
		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _Key, "Key", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::hasMany(a, PluralStringCollection, Wt::Dbo::ManyToOne, "Key");
		}
		static const char *TableName()
		{
			return "languagepluralkeys";
		}
};

class BasePluralString
{
	protected:
		int _Case;

		BasePluralString()
			: _Case(-1)
		{ }
		BasePluralString(int Case, const std::string &String = "")
			: _Case(Case), String(String)
		{ }

	public:
		std::string String;

		int Case() const { return _Case; };
};
class PluralString : public BasePluralString
{
	protected:
		Wt::Dbo::ptr<Language> _LanguagePtr;
		Wt::Dbo::ptr<PluralKey> _KeyPtr;

	public:
		PluralString() { }
		PluralString(Wt::Dbo::ptr<PluralKey> KeyPtr, int Case, Wt::Dbo::ptr<Language> LanguagePtr, const std::string &String = "")
			: BasePluralString(Case, String), _LanguagePtr(LanguagePtr), _KeyPtr(KeyPtr)
		{ }

		Wt::Dbo::ptr<Language> LanguagePtr() const { return _LanguagePtr; };
		Wt::Dbo::ptr<PluralKey> KeyPtr() const { return _KeyPtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _LanguagePtr, "Language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(a, _KeyPtr, "Key", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, _Case, "Case");
			Wt::Dbo::field(a, String, "String");
		}
		static const char *TableName()
		{
			return "languagepluralstrings";
		}
};
class PluralStringData : public BasePluralString, public DataSurrogateKey
{
	protected:
		std::string _Key;
		std::string _LanguageCode;
		int _Case;
		long long _ModuleId;

	public:
		PluralStringData(Wt::Dbo::ptr<PluralString> Ptr)
			: BasePluralString(*Ptr), _LanguageCode(Ptr->LanguagePtr().id()),
			_Key(Ptr->KeyPtr()->Key()), _ModuleId(Ptr->KeyPtr()->ModulePtr().id()),
			_Case(Ptr->Case()), DataSurrogateKey(Ptr.id())
		{ }

		std::string Key() const { return _Key; };
		std::string LanguageCode() const { return _LanguageCode; };
		int Case() const { return _Case; };
		long long ModuleId() const { return _ModuleId; };
};

#include "Dbo/AccessPath.h"

#endif
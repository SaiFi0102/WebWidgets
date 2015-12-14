#ifndef WW_DBO_LANGUAGE_H
#define WW_DBO_LANGUAGE_H

#include "Dbo/ModuleTreeDbos.h"
#include <Wt/WString>
#include <Wt/Dbo/WtSqlTraits>

namespace WW
{
	namespace Dbo
	{

		class BaseLanguage
		{
		protected:
			BaseLanguage() = default;
			BaseLanguage(const std::string &code)
				: _code(code)
			{ }
			BaseLanguage(const std::string &code, const Wt::WString &name, const Wt::WString &nativeName, const std::string &languageAccept, const std::string &pluralExpression, int pluralCases, bool installed)
				: _code(code), name(name), nativeName(nativeName), languageAccept(languageAccept), pluralExpression(pluralExpression), pluralCases(pluralCases), installed(installed)
			{ }

			std::string _code; //en, zh, ur, ISO 639-1: two-letter code

		public:
			Wt::WString	name; //English, Chinese, Russian
			Wt::WString nativeName; // English, 中文, Русский
			std::string	languageAccept; //en*, wildcard allowed
			std::string	pluralExpression; //For most languages "n > 1 ? 2 : 1" where n = number of that item provided, 2 : 1, 1 means case 1(singular) and case 2(plural)
			int			pluralCases = -1; //In English and most languages there are 2, Singular and Plural, for example file and files
			bool		installed = false; //Whether its installed or just to show the name

			std::string code() const { return _code; }
		};
		class Language : public BaseLanguage
		{
		public:
			Language() = default;
			Language(const std::string &code)
				: BaseLanguage(code)
			{ }
			Language(const std::string &code, const Wt::WString &name, const Wt::WString &nativeName, const std::string &languageAccept, const std::string &pluralExpression, int pluralCases, bool installed)
				: BaseLanguage(code, name, nativeName, languageAccept, pluralExpression, pluralCases, installed)
			{ }

			//hasMany
			AccessHostNameCollection accessHostNameCollection;
			LanguageAccessPathCollection languageAccessPathCollection;
			SingularStringCollection singularStringCollection;
			PluralStringCollection pluralStringCollection;

			NavigationMenuItemCollection shownOnPageMenuItemCollection;

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::id(a, _code, "code", 2);
				Wt::Dbo::field(a, name, "name", 255);
				Wt::Dbo::field(a, nativeName, "nativeName", 255);
				Wt::Dbo::field(a, languageAccept, "languageAccept", 255);
				Wt::Dbo::field(a, pluralExpression, "pluralExpression");
				Wt::Dbo::field(a, pluralCases, "pluralCases");
				Wt::Dbo::field(a, installed, "installed");

				Wt::Dbo::hasMany(a, accessHostNameCollection, Wt::Dbo::ManyToOne, "language");
				Wt::Dbo::hasMany(a, languageAccessPathCollection, Wt::Dbo::ManyToOne, "language");
				Wt::Dbo::hasMany(a, singularStringCollection, Wt::Dbo::ManyToOne, "language");
				Wt::Dbo::hasMany(a, pluralStringCollection, Wt::Dbo::ManyToOne, "language");
				Wt::Dbo::hasMany(a, shownOnPageMenuItemCollection, Wt::Dbo::ManyToMany, "language_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "language";
			}
		};

		class SingularKey
		{
		protected:
			std::string _key;
			ptr<Module> _modulePtr;

		public:
			SingularStringCollection singularStringCollection;
			PageCollection pageCollection;
			ConfigurationCollection confTitleCollection;
			ConfigurationCollection confDetailsCollection;
			ConfigurationCollection confExpertWarningCollection;

			SingularKey() = default;
			SingularKey(const std::string &key, ptr<Module> modulePtr)
				: _key(key), _modulePtr(modulePtr)
			{ }

			std::string key() const { return _key; }
			ptr<Module> modulePtr() const { return _modulePtr; }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, _key, "key", 50);
				Wt::Dbo::belongsTo(a, _modulePtr, "module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::hasMany(a, singularStringCollection, Wt::Dbo::ManyToOne, "key");
				Wt::Dbo::hasMany(a, pageCollection, Wt::Dbo::ManyToOne, "titleKey");
				Wt::Dbo::hasMany(a, confTitleCollection, Wt::Dbo::ManyToOne, "titleKey");
				Wt::Dbo::hasMany(a, confDetailsCollection, Wt::Dbo::ManyToOne, "detailsKey");
				Wt::Dbo::hasMany(a, confExpertWarningCollection, Wt::Dbo::ManyToOne, "expertWarningKey");
			}
			constexpr static const char *tableName()
			{
				return "languagesingularkey";
			}
		};

		class BaseSingularString
		{
		protected:
			BaseSingularString() = default;
			BaseSingularString(const std::string &string, bool isEmail)
				: string(string), isEmail(isEmail)
			{ }

		public:
			std::string string;
			bool isEmail = false;
		};
		class SingularString : public BaseSingularString
		{
		protected:
			ptr<Language> _languagePtr;
			ptr<SingularKey> _keyPtr;

		public:
			SingularString() = default;
			SingularString(ptr<SingularKey> keyPtr, const std::string &string, ptr<Language> languagePtr = ptr<Language>(), bool isEmail = false)
				: BaseSingularString(string, isEmail), _languagePtr(languagePtr), _keyPtr(keyPtr)
			{ }

			ptr<Language> languagePtr() const { return _languagePtr; };
			ptr<SingularKey> keyPtr() const { return _keyPtr; };

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, _languagePtr, "language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::belongsTo(a, _keyPtr, "key", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, string, "string");
				Wt::Dbo::field(a, isEmail, "isEmail");
			}
			constexpr static const char *tableName()
			{
				return "languagesingularstring";
			}
		};

		class PluralKey
		{
		protected:
			std::string _key;
			ptr<Module> _modulePtr;

		public:
			PluralStringCollection pluralStringCollection;

			PluralKey() = default;
			PluralKey(const std::string &key, ptr<Module> modulePtr)
				: _key(key), _modulePtr(modulePtr)
			{ }

			std::string key() const { return _key; }
			ptr<Module> modulePtr() const { return _modulePtr; }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, _key, "key", 50);
				Wt::Dbo::belongsTo(a, _modulePtr, "module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::hasMany(a, pluralStringCollection, Wt::Dbo::ManyToOne, "key");
			}
			constexpr static const char *tableName()
			{
				return "languagepluralkey";
			}
		};

		class BasePluralString
		{
		protected:
			BasePluralString() = default;
			BasePluralString(int Case, const std::string &string)
				: _case(Case), string(string)
			{ }

			int _case = -1;

		public:
			std::string string;

			int Case() const { return _case; };
		};
		class PluralString : public BasePluralString
		{
		protected:
			ptr<Language> _languagePtr;
			ptr<PluralKey> _keyPtr;

		public:
			PluralString() = default;
			PluralString(ptr<PluralKey> keyPtr, int Case, ptr<Language> languagePtr, const std::string &string = "")
				: BasePluralString(Case, string), _languagePtr(languagePtr), _keyPtr(keyPtr)
			{ }

			ptr<Language> languagePtr() const { return _languagePtr; };
			ptr<PluralKey> keyPtr() const { return _keyPtr; };

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, _languagePtr, "language", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::belongsTo(a, _keyPtr, "key", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, _case, "case");
				Wt::Dbo::field(a, string, "string");
			}
			constexpr static const char *tableName()
			{
				return "languagepluralstring";
			}
		};

	}

	namespace Ddo
	{

		class Language : public Dbo::BaseLanguage
		{
		public:
			Language(Dbo::ptr<Dbo::Language> ptr)
				: BaseLanguage(*ptr)
			{ }
		};
		class SingularString : public Dbo::BaseSingularString, public SurrogateKey
		{
		protected:
			std::string _key;
			std::string _languageCode;
			long long _moduleId;

		public:
			SingularString(Dbo::ptr<Dbo::SingularString> ptr)
				: BaseSingularString(*ptr), _languageCode(ptr->languagePtr().id()),
				_key(ptr->keyPtr()->key()), _moduleId(ptr->keyPtr()->modulePtr().id()),
				SurrogateKey(ptr.id())
			{ }

			std::string key() const { return _key; };
			std::string languageCode() const { return _languageCode; };
			long long moduleId() const { return _moduleId; };
		};
		class PluralString : public Dbo::BasePluralString, public SurrogateKey
		{
		protected:
			std::string _key;
			std::string _languageCode;
			int _case;
			long long _moduleId;

		public:
			PluralString(Dbo::ptr<Dbo::PluralString> ptr)
				: BasePluralString(*ptr), _languageCode(ptr->languagePtr().id()),
				_key(ptr->keyPtr()->key()), _moduleId(ptr->keyPtr()->modulePtr().id()),
				_case(ptr->Case()), SurrogateKey(ptr.id())
			{ }

			std::string key() const { return _key; };
			std::string languageCode() const { return _languageCode; };
			int Case() const { return _case; };
			long long moduleId() const { return _moduleId; };
		};

	}
}

#endif

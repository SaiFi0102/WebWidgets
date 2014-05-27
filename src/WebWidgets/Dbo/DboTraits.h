#ifndef DBOTRAITS_DBO_H
#define DBOTRAITS_DBO_H

#include <Wt/Dbo/Dbo>

class Module;
class Configuration;
class ConfigurationBool;
class ConfigurationDouble;
class ConfigurationEnum;
class ConfigurationFloat;
class ConfigurationInt;
class ConfigurationLongInt;
class ConfigurationString;
class ConfigurationEnumValue;
class Page;
class Template;
class Language;
class LanguageSingle;
class LanguagePlural;
class Author;
class AccessPath;
class Style;
class StyleCssRule;
class TemplateCssRule;
class StyleTemplate;

struct ConfigurationKeys;
struct LanguageSingleKeys;
struct LanguagePluralKeys;
struct PageKeys;
struct StyleKeys;
struct TemplateKeys;
struct StyleTemplateKeys;

typedef Wt::Dbo::collection< Wt::Dbo::ptr<Module> > ModuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Configuration> > ConfigurationCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationBool> > BoolCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationEnum> > EnumCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationDouble> > DoubleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationFloat> > FloatCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationInt> > IntCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationLongInt> > LongIntCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationString> > StringCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationEnumValue> > EnumValueCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Page> > PageCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Template> > TemplateCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Language> > LanguageCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<LanguageSingle> > LanguageSingleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<LanguagePlural> > LanguagePluralCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Author> > AuthorCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<AccessPath> > AccessPathCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Style> > StyleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<StyleCssRule> > StyleCssRuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<StyleTemplate> > StyleTemplateCollections;

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

namespace Wt
{
	namespace Dbo
	{
		//Overloaded Wt::Dbo::field() for Configuration Keys structure
		template<class Action>
		void field(Action &action, ConfigurationKeys &Keys, const std::string &name, int size = -1)
		{
			field(action, Keys.Name, "Name", 256);
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			field(action, Keys.Type, "Type");
		}

		//Overloaded dbo_traits for Configuration DBO
		template<>
		struct dbo_traits<::Configuration> : public dbo_default_traits
		{
			typedef ConfigurationKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationBool DBO
		template<>
		struct dbo_traits<ConfigurationBool> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationDouble DBO
		template<>
		struct dbo_traits<ConfigurationDouble> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationEnum DBO
		template<>
		struct dbo_traits<ConfigurationEnum> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationFloat DBO
		template<>
		struct dbo_traits<ConfigurationFloat> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationInt DBO
		template<>
		struct dbo_traits<ConfigurationInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationLongInt DBO
		template<>
		struct dbo_traits<ConfigurationLongInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationString DBO
		template<>
		struct dbo_traits<ConfigurationString> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
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
		template<class Action>
		void field(Action &action, PageKeys &Keys, const std::string &name, int size = -1)
		{
			field(action, Keys.id, "page_id");
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<Page> : public dbo_default_traits
		{
			typedef PageKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		template<class Action>
		void field(Action &action, StyleKeys &Key, const std::string &name, int size = -1)
		{
			Wt::Dbo::field(action, Key.Name, "StyleName", 256);
			Wt::Dbo::belongsTo(action, Key.AuthorPtr, "Author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<Style> : public dbo_default_traits
		{
			typedef StyleKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		template<class Action>
		void field(Action &action, TemplateKeys &Key, const std::string &name, int size = -1)
		{
			Wt::Dbo::field(action, Key.Name, "TemplateName", 256);
			Wt::Dbo::belongsTo(action, Key.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<Template> : public dbo_default_traits
		{
			typedef TemplateKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		template<class Action>
		void field(Action &action, StyleTemplateKeys &Key, const std::string &name, int size = -1)
		{
			Wt::Dbo::belongsTo(action, Key.DerivingTemplatePtr, "Template", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(action, Key.StylePtr, "Style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<StyleTemplate> : public dbo_default_traits
		{
			typedef StyleTemplateKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
}

#endif

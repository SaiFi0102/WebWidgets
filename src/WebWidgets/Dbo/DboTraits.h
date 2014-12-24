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

//Overloaded dbo_traits for ConfigurationBool DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationBool> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationDouble DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationDouble> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationEnum DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationEnum> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationFloat DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationFloat> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationInt DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationInt> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationLongInt DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationLongInt> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};
//Overloaded dbo_traits for ConfigurationString DBO
template<>
struct Wt::Dbo::dbo_traits<ConfigurationString> : public Wt::Dbo::dbo_default_traits
{
	typedef Wt::Dbo::ptr<::Configuration> IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};

//Overloaded dbo_traits for Language DBO
template<>
struct Wt::Dbo::dbo_traits<Language> : public Wt::Dbo::dbo_default_traits
{
	typedef std::string IdType;
	static IdType invalidId() { return IdType(); }
	static const char *surrogateIdField() { return 0; }
};

class DataSurrogateKey
{
	public:
		long long id() const
		{
			return _id;
		}

	protected:
		DataSurrogateKey(long long id)
			: _id(id)
		{ };

		long long _id;
};

#endif

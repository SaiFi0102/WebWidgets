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
class NavigationMenu;
class NavigationMenuItem;
class Template;
class Language;
class SingularKey;
class SingularString;
class PluralKey;
class PluralString;
class Author;
class AccessHostName;
class AbstractAccessPath;
class PageAccessPath;
class LanguageAccessPath;
class Style;
class StyleCssRule;
class TemplateCssRule;
class StyleTemplate;
class StyleSection;

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
typedef Wt::Dbo::collection< Wt::Dbo::ptr<NavigationMenu> > NavigationMenuCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<NavigationMenuItem> > NavigationMenuItemCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Template> > TemplateCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Language> > LanguageCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<SingularKey> > SingularKeyCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<SingularString> > SingularStringCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<PluralKey> > PluralKeyCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<PluralString> > PluralStringCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Author> > AuthorCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<AccessHostName> > AccessHostNameCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<PageAccessPath> > PageAccessPathCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<LanguageAccessPath> > LanguageAccessPathCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Style> > StyleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<StyleCssRule> > StyleCssRuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<StyleTemplate> > StyleTemplateCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<StyleSection> > StyleSectionCollections;

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

//Overloaded dbo_traits for AccessHostName DBO
template<>
struct Wt::Dbo::dbo_traits<AccessHostName> : public Wt::Dbo::dbo_default_traits
{
	typedef std::string IdType;
	static IdType invalidId() { return "!"; }
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

#define MAPDBOCASSES(DboSession) \
	DboSession.mapClass<Author>(Author::TableName()); \
	DboSession.mapClass<Module>(Module::TableName()); \
	DboSession.mapClass<Configuration>(Configuration::TableName()); \
	DboSession.mapClass<ConfigurationBool>(ConfigurationBool::TableName()); \
	DboSession.mapClass<ConfigurationEnum>(ConfigurationEnum::TableName()); \
	DboSession.mapClass<ConfigurationEnumValue>(ConfigurationEnumValue::TableName()); \
	DboSession.mapClass<ConfigurationDouble>(ConfigurationDouble::TableName()); \
	DboSession.mapClass<ConfigurationFloat>(ConfigurationFloat::TableName()); \
	DboSession.mapClass<ConfigurationInt>(ConfigurationInt::TableName()); \
	DboSession.mapClass<ConfigurationLongInt>(ConfigurationLongInt::TableName()); \
	DboSession.mapClass<ConfigurationString>(ConfigurationString::TableName()); \
	DboSession.mapClass<Language>(Language::TableName()); \
	DboSession.mapClass<SingularKey>(SingularKey::TableName()); \
	DboSession.mapClass<SingularString>(SingularString::TableName()); \
	DboSession.mapClass<PluralKey>(PluralKey::TableName()); \
	DboSession.mapClass<PluralString>(PluralString::TableName()); \
	DboSession.mapClass<Page>(Page::TableName()); \
/*	DboSession.mapClass<NavigationMenu>(NavigationMenu::TableName()); \
//	DboSession.mapClass<NavigationMenuItem>(NavigationMenuItem::TableName()); */\
	DboSession.mapClass<Template>(Template::TableName()); \
	DboSession.mapClass<Style>(Style::TableName()); \
	DboSession.mapClass<StyleSection>(StyleSection::TableName()); \
	DboSession.mapClass<StyleTemplate>(StyleTemplate::TableName()); \
	DboSession.mapClass<StyleCssRule>(StyleCssRule::TableName()); \
	DboSession.mapClass<TemplateCssRule>(TemplateCssRule::TableName()); \
	DboSession.mapClass<AccessHostName>(AccessHostName::TableName()); \
	DboSession.mapClass<PageAccessPath>(PageAccessPath::TableName()); \
	DboSession.mapClass<LanguageAccessPath>(LanguageAccessPath::TableName());

#endif

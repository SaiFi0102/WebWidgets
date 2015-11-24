#ifndef DBOTRAITS_DBO_H
#define DBOTRAITS_DBO_H

#include <Wt/Dbo/Dbo>

struct MenuItemOnPageKey;
struct MenuItemOnPageAccessPathKey;

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
class MenuItemOnPage;
class MenuItemOnPageAccessPath;
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
typedef Wt::Dbo::collection< Wt::Dbo::ptr<MenuItemOnPage> > MenuItemOnPageCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<MenuItemOnPageAccessPath> > MenuItemOnPageAccessPathCollections;
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

namespace Wt
{
	namespace Dbo
	{
		template<>
		struct dbo_traits<Author> : public dbo_default_traits
		{
			static const char *surrogateIdField() { return 0; }
		};
		template<>
		struct dbo_traits<Module> : public dbo_default_traits
		{
			static const char *surrogateIdField() { return 0; }
		};

		//ConfigurationBool
		template<>
		struct dbo_traits<ConfigurationBool> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationDouble
		template<>
		struct dbo_traits<ConfigurationDouble> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationEnum
		template<>
		struct dbo_traits<ConfigurationEnum> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationFloat
		template<>
		struct dbo_traits<ConfigurationFloat> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationInt
		template<>
		struct dbo_traits<ConfigurationInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationLongInt
		template<>
		struct dbo_traits<ConfigurationLongInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};
		//ConfigurationString
		template<>
		struct dbo_traits<ConfigurationString> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};

		//Language
		template<>
		struct dbo_traits<Language> : public dbo_default_traits
		{
			typedef std::string IdType;
			static IdType invalidId() { return IdType(); }
			static const char *surrogateIdField() { return 0; }
		};

		//AccessHostName
		template<>
		struct dbo_traits<AccessHostName> : public dbo_default_traits
		{
			typedef std::string IdType;
			static IdType invalidId() { return "!"; }
			static const char *surrogateIdField() { return 0; }
		};

		//MenuItemPage custom ManyToMany
		template <class Action>
		void field(Action &action, MenuItemOnPageKey &key, const std::string &, int size = -1)
		{
			Wt::Dbo::belongsTo(action, key.MenuItemPtr, "NavigationMenuItem", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(action, key.PagePtr, "Page", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<MenuItemOnPage> : public dbo_default_traits
		{
			typedef MenuItemOnPageKey IdType;
			static IdType invalidId();
			static const char *surrogateIdField() { return 0; }
		};

		//MenuItemPageAccessPath custom ManyToMany
		template <class Action>
		void field(Action &action, MenuItemOnPageAccessPathKey &key, const std::string &, int size = -1)
		{
			Wt::Dbo::belongsTo(action, key.MenuItemPtr, "NavigationMenuItem", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(action, key.AccessPathPtr, "PageAccessPath", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		template<>
		struct dbo_traits<MenuItemOnPageAccessPath> : public dbo_default_traits
		{
			typedef MenuItemOnPageAccessPathKey IdType;
			static IdType invalidId();
			static const char *surrogateIdField() { return 0; }
		};
	}
}

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
	DboSession.mapClass<NavigationMenu>(NavigationMenu::TableName()); \
	DboSession.mapClass<NavigationMenuItem>(NavigationMenuItem::TableName()); \
	DboSession.mapClass<MenuItemOnPage>(MenuItemOnPage::TableName()); \
	DboSession.mapClass<MenuItemOnPageAccessPath>(MenuItemOnPageAccessPath::TableName()); \
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

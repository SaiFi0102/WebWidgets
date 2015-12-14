#ifndef WW_DBO_MODULETREE_H
#define WW_DBO_MODULETREE_H

#include "Dbo/DboDefs.h"

namespace WW
{
	namespace Dbo
	{
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
		//class StyleSection;

		struct MenuItemOnPageKey;
		struct MenuItemOnPageAccessPathKey;

		typedef Wt::Dbo::collection< ptr<Module> > ModuleCollection;
		typedef Wt::Dbo::collection< ptr<Configuration> > ConfigurationCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationBool> > BoolCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationEnum> > EnumCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationDouble> > DoubleCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationFloat> > FloatCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationInt> > IntCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationLongInt> > LongIntCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationString> > StringCollection;
		typedef Wt::Dbo::collection< ptr<ConfigurationEnumValue> > EnumValueCollection;
		typedef Wt::Dbo::collection< ptr<Page> > PageCollection;
		typedef Wt::Dbo::collection< ptr<NavigationMenu> > NavigationMenuCollection;
		typedef Wt::Dbo::collection< ptr<NavigationMenuItem> > NavigationMenuItemCollection;
		typedef Wt::Dbo::collection< ptr<MenuItemOnPage> > MenuItemOnPageCollection;
		typedef Wt::Dbo::collection< ptr<MenuItemOnPageAccessPath> > MenuItemOnPageAccessPathCollection;
		typedef Wt::Dbo::collection< ptr<Template> > TemplateCollection;
		typedef Wt::Dbo::collection< ptr<Language> > LanguageCollection;
		typedef Wt::Dbo::collection< ptr<SingularKey> > SingularKeyCollection;
		typedef Wt::Dbo::collection< ptr<SingularString> > SingularStringCollection;
		typedef Wt::Dbo::collection< ptr<PluralKey> > PluralKeyCollection;
		typedef Wt::Dbo::collection< ptr<PluralString> > PluralStringCollection;
		typedef Wt::Dbo::collection< ptr<Author> > AuthorCollection;
		typedef Wt::Dbo::collection< ptr<AccessHostName> > AccessHostNameCollection;
		typedef Wt::Dbo::collection< ptr<PageAccessPath> > PageAccessPathCollection;
		typedef Wt::Dbo::collection< ptr<LanguageAccessPath> > LanguageAccessPathCollection;
		typedef Wt::Dbo::collection< ptr<Style> > StyleCollection;
		typedef Wt::Dbo::collection< ptr<StyleCssRule> > StyleCssRuleCollection;
		typedef Wt::Dbo::collection< ptr<TemplateCssRule> > TemplateCssRuleCollection;
		typedef Wt::Dbo::collection< ptr<StyleTemplate> > StyleTemplateCollection;
		//typedef Wt::Dbo::collection< ptr<StyleSection> > StyleSectionCollection;
	}
}

#include "Dbo/ModuleTreeTraits.h"
#include "Dbo/Author.h"
#include "Dbo/Module.h"
#include "Dbo/Configuration.h"
#include "Dbo/Language.h"
#include "Dbo/AccessPath.h"
#include "Dbo/Style.h"
#include "Dbo/Page.h"
#include "Dbo/NavigationMenu.h"

#define MAP_MODULE_DBO_TREE(dboSession) \
	dboSession.mapClass<WW::Dbo::Author>(WW::Dbo::Author::tableName()); \
	dboSession.mapClass<WW::Dbo::Module>(WW::Dbo::Module::tableName()); \
	dboSession.mapClass<WW::Dbo::Configuration>(WW::Dbo::Configuration::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationBool>(WW::Dbo::ConfigurationBool::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationEnum>(WW::Dbo::ConfigurationEnum::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationEnumValue>(WW::Dbo::ConfigurationEnumValue::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationDouble>(WW::Dbo::ConfigurationDouble::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationFloat>(WW::Dbo::ConfigurationFloat::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationInt>(WW::Dbo::ConfigurationInt::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationLongInt>(WW::Dbo::ConfigurationLongInt::tableName()); \
	dboSession.mapClass<WW::Dbo::ConfigurationString>(WW::Dbo::ConfigurationString::tableName()); \
	dboSession.mapClass<WW::Dbo::Language>(WW::Dbo::Language::tableName()); \
	dboSession.mapClass<WW::Dbo::SingularKey>(WW::Dbo::SingularKey::tableName()); \
	dboSession.mapClass<WW::Dbo::SingularString>(WW::Dbo::SingularString::tableName()); \
	dboSession.mapClass<WW::Dbo::PluralKey>(WW::Dbo::PluralKey::tableName()); \
	dboSession.mapClass<WW::Dbo::PluralString>(WW::Dbo::PluralString::tableName()); \
	dboSession.mapClass<WW::Dbo::Page>(WW::Dbo::Page::tableName()); \
	dboSession.mapClass<WW::Dbo::NavigationMenu>(WW::Dbo::NavigationMenu::tableName()); \
	dboSession.mapClass<WW::Dbo::NavigationMenuItem>(WW::Dbo::NavigationMenuItem::tableName()); \
	dboSession.mapClass<WW::Dbo::MenuItemOnPage>(WW::Dbo::MenuItemOnPage::tableName()); \
	dboSession.mapClass<WW::Dbo::MenuItemOnPageAccessPath>(WW::Dbo::MenuItemOnPageAccessPath::tableName()); \
	dboSession.mapClass<WW::Dbo::Template>(WW::Dbo::Template::tableName()); \
	dboSession.mapClass<WW::Dbo::Style>(WW::Dbo::Style::tableName()); \
/*	dboSession.mapClass<WW::Dbo::StyleSection>(WW::Dbo::StyleSection::tableName());*/ \
	dboSession.mapClass<WW::Dbo::StyleTemplate>(WW::Dbo::StyleTemplate::tableName()); \
	dboSession.mapClass<WW::Dbo::StyleCssRule>(WW::Dbo::StyleCssRule::tableName()); \
	dboSession.mapClass<WW::Dbo::TemplateCssRule>(WW::Dbo::TemplateCssRule::tableName()); \
	dboSession.mapClass<WW::Dbo::AccessHostName>(WW::Dbo::AccessHostName::tableName()); \
	dboSession.mapClass<WW::Dbo::PageAccessPath>(WW::Dbo::PageAccessPath::tableName()); \
	dboSession.mapClass<WW::Dbo::LanguageAccessPath>(WW::Dbo::LanguageAccessPath::tableName());

#endif

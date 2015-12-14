#ifndef WW_DBO_STYLE_H
#define WW_DBO_STYLE_H

#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	namespace Dbo
	{

		class BaseStyle
		{
		protected:
			std::string _name;

			BaseStyle() = default;
			BaseStyle(const std::string &name)
				: _name(name)
			{ }

		public:
			std::string description;
			int compatibilityVersionSeries = -1;
			int compatibilityVersionMajor = -1;

			std::string name() const { return _name; }
		};
		class Style : public BaseStyle
		{
		protected:
			ptr<Author> _authorPtr;

		public:
			StyleCssRuleCollection cssRuleCollection;
			StyleTemplateCollection templateCollection;
			//StyleSectionCollections sectionCollection;
			AccessHostNameCollection accessHostNameCollection;
			NavigationMenuItemCollection shownOnPageMenuItemCollection;

			Style() = default;
			Style(const std::string &name, ptr<Author> authorPtr)
				: BaseStyle(name), _authorPtr(authorPtr)
			{ }

			ptr<Author> authorPtr() const { return _authorPtr; }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, _name, "styleName", 50);
				Wt::Dbo::belongsTo(a, _authorPtr, "author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, description, "description");
				Wt::Dbo::field(a, compatibilityVersionSeries, "compatibilityVersionSeries");
				Wt::Dbo::field(a, compatibilityVersionMajor, "compatibilityVersionMajor");

				Wt::Dbo::hasMany(a, cssRuleCollection, Wt::Dbo::ManyToOne, "style");
				Wt::Dbo::hasMany(a, templateCollection, Wt::Dbo::ManyToOne, "style");
				//Wt::Dbo::hasMany(a, sectionCollection, Wt::Dbo::ManyToOne, "style");
				Wt::Dbo::hasMany(a, accessHostNameCollection, Wt::Dbo::ManyToOne, "style");
				Wt::Dbo::hasMany(a, shownOnPageMenuItemCollection, Wt::Dbo::ManyToMany, "style_show_menuitem", "", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "style";
			}
		};

// 		class BaseStyleSection
// 		{
// 			public:
// 				enum VerticalOrientation
// 				{
// 					Top = 0,
// 					Middle = 1,
// 					Bottom = 2,
// 				};
// 				enum HorizontalOrientation
// 				{
// 					Left = 0,
// 					Center = 1,
// 					Right = 2
// 				};
// 				enum SectionType
// 				{
// 					PageContentSection = 0,
// 					FixedSection = 1,
// 				};
// 
// 				VerticalOrientation vOrientation;
// 				HorizontalOrientation hOrientation;
// 				SectionType type;
// 				int order;
// 		};
// 		class StyleSection : public BaseStyleSection
// 		{
// 			public:
// 				ptr<Style> stylePtr;
// 
// 				template<class Action>void persist(Action &a)
// 				{
// 					Wt::Dbo::belongsTo(a, stylePtr, "style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
// 					Wt::Dbo::field(a, vOrientation, "vOrientation");
// 					Wt::Dbo::field(a, hOrientation, "hOrientation");
// 					Wt::Dbo::field(a, type, "type");
// 					Wt::Dbo::field(a, order, "order");
// 				}
// 				constexpr static const char *tableName()
// 				{
// 					return "stylesection";
// 				}
// 		};

		class BaseCssRule
		{
		public:
			BaseCssRule() = default;
			BaseCssRule(const std::string &selector, const std::string &declarations)
				: selector(selector), declarations(declarations)
			{ }

			std::string selector;
			std::string declarations;
		};

		class StyleCssRule : public BaseCssRule
		{
		public:
			StyleCssRule() = default;
			StyleCssRule(ptr<Style> stylePtr)
				: stylePtr(stylePtr)
			{ }
			StyleCssRule(ptr<Style> stylePtr, const std::string &selector, const std::string &declarations)
				: stylePtr(stylePtr), BaseCssRule(selector, declarations)
			{ }

			ptr<Style> stylePtr;

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, selector, "selector");
				Wt::Dbo::field(a, declarations, "declarations");
				Wt::Dbo::belongsTo(a, stylePtr, "style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "stylecssrule";
			}
		};

		class TemplateCssRule : public BaseCssRule
		{
		public:
			TemplateCssRule() = default;
			TemplateCssRule(ptr<Template> templatePtr)
				: templatePtr(templatePtr)
			{ }
			TemplateCssRule(ptr<Template> templatePtr, const std::string &selector, const std::string &declarations)
				: templatePtr(templatePtr), BaseCssRule(selector, declarations)
			{ }

			ptr<Template> templatePtr;

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, selector, "selector");
				Wt::Dbo::field(a, declarations, "declarations");
				Wt::Dbo::belongsTo(a, templatePtr, "template", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			}
			constexpr static const char *tableName()
			{
				return "templatecssrule";
			}
		};

		class BaseTemplate
		{
		protected:
			std::string _name;

			BaseTemplate() = default;
			BaseTemplate(const std::string &name)
				: _name(name)
			{ }

		public:
			std::string description;
			std::string templateStr;

			std::string name() const { return _name; }
		};
		class Template : public BaseTemplate
		{
		protected:
			ptr<Module> _modulePtr;

		public:
			StyleTemplateCollection styleTemplateCollection;
			TemplateCssRuleCollection cssRuleCollection;

			Template() = default;
			Template(const std::string &name, ptr<Module> modulePtr)
				: BaseTemplate(name), _modulePtr(modulePtr)
			{ }

			ptr<Module> modulePtr() const { return _modulePtr; }

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::field(a, _name, "templateName", 50);
				Wt::Dbo::belongsTo(a, _modulePtr, "module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, description, "description");
				Wt::Dbo::field(a, templateStr, "templateStr");

				Wt::Dbo::hasMany(a, styleTemplateCollection, Wt::Dbo::ManyToOne, "template");
				Wt::Dbo::hasMany(a, cssRuleCollection, Wt::Dbo::ManyToOne, "template");
			}
			constexpr static const char *tableName()
			{
				return "template";
			}
		};

		class BaseStyleTemplate
		{
		public:
			std::string templateStr;
		};
		class StyleTemplate : public BaseStyleTemplate
		{
		protected:
			ptr<Template> _derivingTemplatePtr;
			ptr<Style> _stylePtr;

		public:
			StyleTemplate() = default;
			StyleTemplate(ptr<Style> stylePtr, ptr<Template> templatePtr)
				: _derivingTemplatePtr(templatePtr), _stylePtr(stylePtr)
			{ }

			ptr<Template> derivingTemplatePtr() const { return _derivingTemplatePtr; };
			ptr<Style> stylePtr() const { return _stylePtr; };

			template<class Action>void persist(Action &a)
			{
				Wt::Dbo::belongsTo(a, _derivingTemplatePtr, "template", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::belongsTo(a, _stylePtr, "style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
				Wt::Dbo::field(a, templateStr, "templateStr");
			}
			constexpr static const char *tableName()
			{
				return "styletemplate";
			}
		};

	}

	namespace Ddo
	{

		class StyleCssRule : public Dbo::BaseCssRule, public SurrogateKey
		{
		public:
			long long styleId;

			StyleCssRule(Dbo::ptr<Dbo::StyleCssRule> ptr)
				: SurrogateKey(ptr.id()), Dbo::BaseCssRule(*ptr), styleId(ptr->stylePtr.id())
			{ }
		};

		class Style : public Dbo::BaseStyle, public SurrogateKey
		{
		protected:
			long long _authorId;

		public:
			typedef std::set< cPtr<StyleCssRule> > StyleCssRuleSet;
			//typedef std::set< cPtr<StyleSectionData> > StyleSectionSet;

			StyleCssRuleSet styleCssRules;
			//StyleSectionSet styleSections;

			Style(Dbo::ptr<Dbo::Style> ptr)
				: SurrogateKey(ptr.id()), Dbo::BaseStyle(*ptr), _authorId(ptr->authorPtr().id())
			{ }

			long long authorId() const { return _authorId; }
		};

		class TemplateCssRule : public Dbo::BaseCssRule, public SurrogateKey
		{
		public:
			long long templateId;

			TemplateCssRule(Dbo::ptr<Dbo::TemplateCssRule> ptr)
				: SurrogateKey(ptr.id()), Dbo::BaseCssRule(*ptr), templateId(ptr->templatePtr.id())
			{ }
		};

		class Template : public Dbo::BaseTemplate, public SurrogateKey
		{
		protected:
			long long _moduleId;

		public:
			typedef std::set< cPtr<TemplateCssRule> > TemplateCssRuleSet;
			TemplateCssRuleSet templateCssRules;

			Template(Dbo::ptr<Dbo::Template> ptr)
				: SurrogateKey(ptr.id()), Dbo::BaseTemplate(*ptr), _moduleId(ptr->modulePtr().id())
			{ }

			long long moduleId() const { return _moduleId; }
		};

		class StyleTemplate : public Dbo::BaseStyleTemplate, public SurrogateKey
		{
		protected:
			long long _derivingTemplateId;
			long long _styleId;

		public:
			std::string templateName;
			long long moduleId;
			std::string styleName;
			long long authorId;

			StyleTemplate(Dbo::ptr<Dbo::StyleTemplate> ptr)
				: SurrogateKey(ptr.id()),
				Dbo::BaseStyleTemplate(*ptr),
				_derivingTemplateId(ptr->derivingTemplatePtr().id()),
				_styleId(ptr->stylePtr().id()),
				templateName(ptr->derivingTemplatePtr()->name()),
				moduleId(ptr->derivingTemplatePtr()->modulePtr().id()),
				styleName(ptr->stylePtr()->name()), authorId(ptr->stylePtr()->authorPtr().id())
			{ }

			long long derivingTemplateId() const { return _derivingTemplateId; }
			long long styleId() const { return _styleId; }
		};

// 		class StyleSectionData : public BaseStyleSection, public DataSurrogateKey
// 		{
// 			public:
// 				long long styleId;
// 
// 				StyleSectionData(Dbo::ptr<Dbo::StyleSection> ptr)
// 					: DataSurrogateKey(ptr.id()), BaseStyleSection(*ptr), styleId(ptr->stylePtr.id())
// 				{ }
// 		};

	}
}

#endif

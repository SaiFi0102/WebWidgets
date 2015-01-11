#ifndef STYLE_DBO_H
#define STYLE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"
#include "Dbo/Author.h"

class BaseStyle
{
	protected:
		std::string _Name;

		BaseStyle(const std::string &Name = "")
			: _Name(Name), CompatibilityVersionSeries(-1), CompatibilityVersionMajor(-1)
		{ }

	public:
		std::string Description;
		int CompatibilityVersionSeries;
		int CompatibilityVersionMajor;

		std::string Name() const { return _Name; }
};
class Style : public BaseStyle
{
	protected:
		Wt::Dbo::ptr<Author> _AuthorPtr;

	public:
		StyleCssRuleCollections CssRuleCollection;
		StyleTemplateCollections TemplateCollection;

		Style() { }
		Style(const std::string &Name, Wt::Dbo::ptr<Author> AuthorPtr)
			: BaseStyle(Name), _AuthorPtr(AuthorPtr)
		{ }

		Wt::Dbo::ptr<Author> AuthorPtr() const { return _AuthorPtr; }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _Name, "StyleName", 50);
			Wt::Dbo::belongsTo(a, _AuthorPtr, "Author", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Description, "Description");
			Wt::Dbo::field(a, CompatibilityVersionSeries, "CompatibilityVersionSeries");
			Wt::Dbo::field(a, CompatibilityVersionMajor, "CompatibilityVersionMajor");

			Wt::Dbo::hasMany(a, CssRuleCollection, Wt::Dbo::ManyToOne, "Style");
			Wt::Dbo::hasMany(a, TemplateCollection, Wt::Dbo::ManyToOne, "Style");
		}
		static const char *TableName()
		{
			return "styles";
		}
};
class StyleData : public BaseStyle, public DataSurrogateKey
{
	protected:
		long long _AuthorId;

	public:
		StyleData(Wt::Dbo::ptr<Style> Ptr)
			: DataSurrogateKey(Ptr.id()), BaseStyle(*Ptr), _AuthorId(Ptr->AuthorPtr().id())
		{ }

		long long AuthorId() const { return _AuthorId; }
};

class BaseStyleCssRule
{
	public:
		std::string Selector;
		std::string Declarations;
};
class StyleCssRule : public BaseStyleCssRule
{
	public:
		Wt::Dbo::ptr<Style>	StylePtr;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, Selector, "Selector");
			Wt::Dbo::field(a, Declarations, "Declarations");
			Wt::Dbo::belongsTo(a, StylePtr, "Style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		static const char *TableName()
		{
			return "stylecssrules";
		}
};
class StyleCssRuleData : public BaseStyleCssRule, public DataSurrogateKey
{
	public:
		long long StyleId;

		StyleCssRuleData(Wt::Dbo::ptr<StyleCssRule> Ptr)
			: DataSurrogateKey(Ptr.id()), BaseStyleCssRule(*Ptr), StyleId(Ptr->StylePtr.id())
		{ }
};

class BaseTemplateCssRule
{
	public:
		std::string Selector;
		std::string Declarations;
};
class TemplateCssRule : public BaseTemplateCssRule
{
	public:
		Wt::Dbo::ptr<Template> TemplatePtr;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, Selector, "Selector");
			Wt::Dbo::field(a, Declarations, "Declarations");
			Wt::Dbo::belongsTo(a, TemplatePtr, "Template", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		static const char *TableName()
		{
			return "templatecssrules";
		}
};
class TemplateCssRuleData : public BaseTemplateCssRule, public DataSurrogateKey
{
	public:
		long long TemplateId;

		TemplateCssRuleData(Wt::Dbo::ptr<TemplateCssRule> Ptr)
			: DataSurrogateKey(Ptr.id()), BaseTemplateCssRule(*Ptr), TemplateId(Ptr->TemplatePtr.id())
		{ }
};

class BaseTemplate
{
	protected:
		std::string _Name;

		BaseTemplate(const std::string &Name = "")
			: _Name(Name)
		{ }

	public:
		std::string Description;
		boost::optional<std::string> TemplateStr;

		std::string Name() const { return _Name; }
};
class Template : public BaseTemplate
{
	protected:
		Wt::Dbo::ptr<Module> _ModulePtr;

	public:
		StyleTemplateCollections StyleTemplateCollection;
		TemplateCssRuleCollections CssRuleCollection;

		Template() { }
		Template(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr)
			: BaseTemplate(Name), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; }

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, _Name, "TemplateName", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Description, "Description");
			Wt::Dbo::field(a, TemplateStr, "TemplateStr");

			Wt::Dbo::hasMany(a, StyleTemplateCollection, Wt::Dbo::ManyToOne, "Template");
			Wt::Dbo::hasMany(a, CssRuleCollection, Wt::Dbo::ManyToOne, "Template");
		}
		static const char *TableName()
		{
			return "templates";
		}
};
class TemplateData : public BaseTemplate, public DataSurrogateKey
{
	protected:
		long long _ModuleId;

	public:
		TemplateData(Wt::Dbo::ptr<Template> Ptr)
			: DataSurrogateKey(Ptr.id()), BaseTemplate(*Ptr), _ModuleId(Ptr->ModulePtr().id())
		{ }

		long long ModuleId() const { return _ModuleId; }
};

class BaseStyleTemplate
{
	public:
		std::string TemplateStr;
};
class StyleTemplate : public BaseStyleTemplate
{
	protected:
		Wt::Dbo::ptr<Template> _DerivingTemplatePtr;
		Wt::Dbo::ptr<Style> _StylePtr;

	public:
		StyleTemplate() { }
		StyleTemplate(Wt::Dbo::ptr<Style> StylePtr, Wt::Dbo::ptr<Template> TemplatePtr)
			: _DerivingTemplatePtr(TemplatePtr), _StylePtr(StylePtr)
		{ }

		Wt::Dbo::ptr<Template> DerivingTemplatePtr() const { return _DerivingTemplatePtr; };
		Wt::Dbo::ptr<Style> StylePtr() const { return _StylePtr; };

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, _DerivingTemplatePtr, "Template", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(a, _StylePtr, "Style", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, TemplateStr, "TemplateStr");
		}
		static const char *TableName()
		{
			return "styletemplates";
		}
};
class StyleTemplateData : public BaseStyleTemplate, public DataSurrogateKey
{
	protected:
		long long _DerivingTemplateId;
		long long _StyleId;

	public:
		std::string TemplateName;
		long long ModuleId;
		std::string StyleName;
		long long AuthorId;

		StyleTemplateData(Wt::Dbo::ptr<StyleTemplate> Ptr)
			: DataSurrogateKey(Ptr.id()),
			BaseStyleTemplate(*Ptr),
			_DerivingTemplateId(Ptr->DerivingTemplatePtr().id()),
			_StyleId(Ptr->StylePtr().id()),
			TemplateName(Ptr->DerivingTemplatePtr()->Name()),
			ModuleId(Ptr->DerivingTemplatePtr()->ModulePtr().id()),
			StyleName(Ptr->StylePtr()->Name()), AuthorId(Ptr->StylePtr()->AuthorPtr().id())
		{ }

		long long DerivingTemplateId() const { return _DerivingTemplateId; }
		long long StyleId() const { return _StyleId; }
};

#endif
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
class StyleData : public BaseStyle
{
	protected:
		long long _AuthorId;

	public:
		StyleData(const std::string &Name, long long AuthorId)
			: BaseStyle(Name), _AuthorId(AuthorId)
		{ }

		long long AuthorId() const { return _AuthorId; }
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
			Wt::Dbo::field(a, _Name, "StyleName", 256);
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

class BaseStyleCssRule
{
	public:
		std::string Selector;
		std::string Declarations;
};
class StyleCssRuleData : public BaseStyleCssRule
{
	public:
		long long StyleId;

		StyleCssRuleData()
			: StyleId(-1)
		{ }
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

class BaseTemplateCssRule
{
	public:
		std::string Selector;
		std::string Declarations;
};
class TemplateCssRuleData : public BaseTemplateCssRule
{
	public:
		long long TemplateId;

		TemplateCssRuleData()
			: TemplateId(-1)
		{ }
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
class TemplateData : public BaseTemplate
{
	protected:
		long long _ModuleId;

	public:
		TemplateData(long long ModuleId)
			: _ModuleId(ModuleId)
		{ }

		long long ModuleId() const { return _ModuleId; }
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
			Wt::Dbo::field(a, _Name, "TemplateName", 256);
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

class BaseStyleTemplate
{
	public:
		std::string TemplateStr;
};
class StyleTemplateData : public BaseStyleTemplate
{
	protected:
		long long _DerivingTemplateId;
		long long _StyleId;

	public:
		StyleTemplateData(long long StyleId, long long DerivingTemplateId)
			: _DerivingTemplateId(DerivingTemplateId), _StyleId(StyleId)
		{ }

		long long DerivingTemplateId() const { return _DerivingTemplateId; }
		long long StyleId() const { return _StyleId; }
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

#endif
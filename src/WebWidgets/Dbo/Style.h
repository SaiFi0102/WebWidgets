#ifndef STYLE_DBO_H
#define STYLE_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"
#include "Dbo/Author.h"
#include <Wt/WCssStyleSheet>

struct StyleKeys
{
	std::string Name;
	Wt::Dbo::ptr<Author> AuthorPtr;

	StyleKeys();
	StyleKeys(const std::string &Name, Wt::Dbo::ptr<Author> AuthorPtr);

	bool operator< (const StyleKeys &other) const;
	bool operator== (const StyleKeys &other) const;
};
std::ostream &operator<< (std::ostream &o, const StyleKeys &c);

class Style : public Wt::Dbo::Dbo<Style>
{
	public:
		std::string	Description;
		int			CompatibilityVersionSeries;
		int			CompatibilityVersionMajor;

		StyleCssRuleCollections		CssRuleCollection;
		StyleTemplateCollections	TemplateCollection;
		AuthorCollections			AuthorCollection;

		Style();
		Style(const std::string &Name, Wt::Dbo::ptr<Author> AuthorPtr = Wt::Dbo::ptr<Author>());

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Style");
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

	private:
		StyleKeys _Id;
};

struct TemplateKeys
{
	std::string Name;
	Wt::Dbo::ptr<Module> ModulePtr;

	TemplateKeys();
	TemplateKeys(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr);

	bool operator< (const TemplateKeys &other) const;
	bool operator== (const TemplateKeys &other) const;
};
std::ostream &operator<< (std::ostream &o, const TemplateKeys &c);

class StyleCssRule : public Wt::Dbo::Dbo<StyleCssRule>
{
	public:
		std::string			Selector;
		std::string			Declarations;
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

class TemplateCssRule : public Wt::Dbo::Dbo<TemplateCssRule>
{
	public:
		std::string				Selector;
		std::string				Declarations;
		Wt::Dbo::ptr<Template>	TemplatePtr;

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

class Template : public Wt::Dbo::Dbo<Template>
{
	public:
		std::string						Description;
		boost::optional<std::string>	TemplateStr;

		StyleTemplateCollections	StyleTemplateCollection;
		TemplateCssRuleCollections	CssRuleCollection;

		Template();
		Template(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>());

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "Template");
			Wt::Dbo::field(a, Description, "Description");
			Wt::Dbo::field(a, TemplateStr, "TemplateStr");

			Wt::Dbo::hasMany(a, StyleTemplateCollection, Wt::Dbo::ManyToOne, "Template");
			Wt::Dbo::hasMany(a, CssRuleCollection, Wt::Dbo::ManyToOne, "Template");
		}
		static const char *TableName()
		{
			return "templates";
		}

	private:
		TemplateKeys _Id;
};

struct StyleTemplateKeys
{
	Wt::Dbo::ptr<Template>	DerivingTemplatePtr;
	Wt::Dbo::ptr<Style>		StylePtr;

	StyleTemplateKeys();
	StyleTemplateKeys(Wt::Dbo::ptr<Template> DerivingTemplatePtr, Wt::Dbo::ptr<Style> StylePtr);

	bool operator< (const StyleTemplateKeys &other) const;
	bool operator== (const StyleTemplateKeys &other) const;
};
std::ostream &operator<< (std::ostream &o, const StyleTemplateKeys &c);

class StyleTemplate : public Wt::Dbo::Dbo<StyleTemplate>
{
	public:
		std::string TemplateStr;

		StyleTemplate(Wt::Dbo::ptr<Style> StylePtr = Wt::Dbo::ptr<Style>(), Wt::Dbo::ptr<Template> TemplatePtr = Wt::Dbo::ptr<Template>());

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id, "StyleTemplate");
			Wt::Dbo::field(a, TemplateStr, "TemplateStr");
		}
		static const char *TableName()
		{
			return "styletemplates";
		}

	private:
		StyleTemplateKeys _Id;
};

#endif
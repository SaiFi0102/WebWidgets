#include "Dbo/Style.h"

Style::Style()
{ }
Style::Style(const std::string &Name, Wt::Dbo::ptr<Author> AuthorPtr)
	: _Id(Name, AuthorPtr)
{ }

StyleKeys::StyleKeys()
{ }
StyleKeys::StyleKeys(const std::string &Name, Wt::Dbo::ptr<Author> AuthorPtr)
	: Name(Name), AuthorPtr(AuthorPtr)
{ }
bool StyleKeys::operator== (const StyleKeys &other) const
{
	return Name == other.Name && AuthorPtr == other.AuthorPtr;
}
bool StyleKeys::operator< (const StyleKeys &other) const
{
	if(Name < other.Name)
	{
		return true;
	}
	else if(Name == other.Name)
	{
		return AuthorPtr < other.AuthorPtr;
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const StyleKeys &c)
{
	return o << "(" << c.Name << ", " << c.AuthorPtr << ")";
}

Wt::Dbo::dbo_traits<Style>::IdType Wt::Dbo::dbo_traits<Style>::invalidId()
{
	return Wt::Dbo::dbo_traits<Style>::IdType();
}
const char* Wt::Dbo::dbo_traits<Style>::surrogateIdField()
{
	return 0;
}

TemplateKeys::TemplateKeys()
{ }
TemplateKeys::TemplateKeys(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr)
	: Name(Name), ModulePtr(ModulePtr)
{ }
bool TemplateKeys::operator== (const TemplateKeys &other) const
{
	return Name == other.Name && ModulePtr == other.ModulePtr;
}
bool TemplateKeys::operator< (const TemplateKeys &other) const
{
	if(Name < other.Name)
	{
		return true;
	}
	else if(Name == other.Name)
	{
		return ModulePtr < other.ModulePtr;
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const TemplateKeys &c)
{
	return o << "(" << c.Name << ", " << c.ModulePtr << ")";
}

Template::Template()
{ }
Template::Template(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr)
	: _Id(Name, ModulePtr)
{ }

Wt::Dbo::dbo_traits<Template>::IdType Wt::Dbo::dbo_traits<Template>::invalidId()
{
	return Wt::Dbo::dbo_traits<Template>::IdType();
}
const char* Wt::Dbo::dbo_traits<Template>::surrogateIdField()
{
	return 0;
}

StyleTemplate::StyleTemplate(Wt::Dbo::ptr<Style> StylePtr, Wt::Dbo::ptr<Template> TemplatePtr)
	: _Id(TemplatePtr, StylePtr)
{ }

StyleTemplateKeys::StyleTemplateKeys()
{ }
StyleTemplateKeys::StyleTemplateKeys(Wt::Dbo::ptr<Template> DerivingTemplatePtr, Wt::Dbo::ptr<Style> StylePtr)
	: DerivingTemplatePtr(DerivingTemplatePtr), StylePtr(StylePtr)
{ }
bool StyleTemplateKeys::operator<(const StyleTemplateKeys &other) const
{
	if(DerivingTemplatePtr < other.DerivingTemplatePtr)
	{
		return true;
	}
	else if(DerivingTemplatePtr == other.DerivingTemplatePtr)
	{
		return StylePtr < other.StylePtr;
	}
	else
	{
		return false;
	}
}
bool StyleTemplateKeys::operator==(const StyleTemplateKeys &other) const
{
	return DerivingTemplatePtr == other.DerivingTemplatePtr && StylePtr == other.StylePtr;
}
std::ostream &operator<<(std::ostream &o, const StyleTemplateKeys &c)
{
	return o << "(" << c.DerivingTemplatePtr << ", " << c.StylePtr << ")";
}

Wt::Dbo::dbo_traits<StyleTemplate>::IdType Wt::Dbo::dbo_traits<StyleTemplate>::invalidId()
{
	return Wt::Dbo::dbo_traits<StyleTemplate>::IdType();
}
const char* Wt::Dbo::dbo_traits<StyleTemplate>::surrogateIdField()
{
	return 0;
}
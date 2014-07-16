#include "Dbo/Style.h"

//Style key
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

//Template key
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

//StyleTemplate key
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

Wt::Dbo::dbo_traits<Style>::IdType Wt::Dbo::dbo_traits<Style>::invalidId()
{
	return Wt::Dbo::dbo_traits<Style>::IdType();
}
Wt::Dbo::dbo_traits<Template>::IdType Wt::Dbo::dbo_traits<Template>::invalidId()
{
	return Wt::Dbo::dbo_traits<Template>::IdType();
}
Wt::Dbo::dbo_traits<StyleTemplate>::IdType Wt::Dbo::dbo_traits<StyleTemplate>::invalidId()
{
	return Wt::Dbo::dbo_traits<StyleTemplate>::IdType();
}
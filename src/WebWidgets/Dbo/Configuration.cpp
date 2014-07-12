#include "Dbo/Configuration.h"

//Configuration Keys
bool ConfigurationKeys::operator< (const ConfigurationKeys &other) const
{
	if(ModulePtr < other.ModulePtr)
	{
		return true;
	}
	else if(ModulePtr == other.ModulePtr)
	{
		if(Name < other.Name)
		{
			return true;
		}
		else if(Name == other.Name)
		{
			return Type < other.Type;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
std::ostream &operator<< (std::ostream &o, const ConfigurationKeys &c)
{
	return o << "(" << c.Name << ", " << c.ModulePtr << ", " << c.Type << ")";
}

Wt::Dbo::dbo_traits<Configuration>::IdType Wt::Dbo::dbo_traits<Configuration>::invalidId()
{
	return Wt::Dbo::dbo_traits<Configuration>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationBool>::IdType Wt::Dbo::dbo_traits<ConfigurationBool>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationBool>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationDouble>::IdType Wt::Dbo::dbo_traits<ConfigurationDouble>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationDouble>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationEnum>::IdType Wt::Dbo::dbo_traits<ConfigurationEnum>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationEnum>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationFloat>::IdType Wt::Dbo::dbo_traits<ConfigurationFloat>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationFloat>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationInt>::IdType Wt::Dbo::dbo_traits<ConfigurationInt>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationInt>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationLongInt>::IdType Wt::Dbo::dbo_traits<ConfigurationLongInt>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationLongInt>::IdType();
}
Wt::Dbo::dbo_traits<ConfigurationString>::IdType Wt::Dbo::dbo_traits<ConfigurationString>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationString>::IdType();
}
#include "Common.h"
#include "Dbo/Configuration.h"

//Configuration Keys
ConfigurationKeys::ConfigurationKeys()
	: Name(std::string()), Application(std::string())
{ }
ConfigurationKeys::ConfigurationKeys(std::string Name, std::string Application)
	: Name(Name), Application(Application)
{ }
bool ConfigurationKeys::operator== (const ConfigurationKeys& other) const
{
	return Name == other.Name && Application == other.Application;
}
bool ConfigurationKeys::operator< (const ConfigurationKeys& other) const
{
	if(Application < other.Application)
	{
		return true;
	}
	else if(Application == other.Application)
	{
		return Name < other.Name;
	}
	else
	{
		return false;
	}
}
std::ostream& operator<< (std::ostream& o, const ConfigurationKeys& c)
{
	return o << "(" << c.Name << ", " << c.Application << ")";
}

//Overloaded dbo_trait for Configuration DBO
Wt::Dbo::dbo_traits<Configuration>::IdType Wt::Dbo::dbo_traits<Configuration>::invalidId()
{
	return Wt::Dbo::dbo_traits<Configuration>::IdType();
}
const char* Wt::Dbo::dbo_traits<Configuration>::surrogateIdField()
{
	return 0;
}

//Configuration Foreign Keys
ConfigurationForeignKeys::ConfigurationForeignKeys()
	: ConfigurationDbo(Wt::Dbo::ptr<Configuration>()), Name(std::string()), Application(std::string())
{ }
ConfigurationForeignKeys::ConfigurationForeignKeys(Wt::Dbo::ptr<Configuration> ConfigurationDbo, std::string Name, std::string Application)
	: ConfigurationDbo(ConfigurationDbo), Name(Name), Application(Application)
{ }
bool ConfigurationForeignKeys::operator== (const ConfigurationForeignKeys& other) const
{
	return Name == other.Name && Application == other.Application;
}
bool ConfigurationForeignKeys::operator< (const ConfigurationForeignKeys& other) const
{
	if(Application < other.Application)
	{
		return true;
	}
	else if(Application == other.Application)
	{
		return Name < other.Name;
	}
	else
	{
		return false;
	}
}
std::ostream& operator<< (std::ostream& o, const ConfigurationForeignKeys& c)
{
	return o << "(" << c.Name << ", " << c.Application << ")";
}

//ConfigurationBool traits
Wt::Dbo::dbo_traits<ConfigurationBool>::IdType Wt::Dbo::dbo_traits<ConfigurationBool>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationBool>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationBool>::surrogateIdField()
{
	return 0;
}

//ConfigurationDouble traits
Wt::Dbo::dbo_traits<ConfigurationDouble>::IdType Wt::Dbo::dbo_traits<ConfigurationDouble>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationDouble>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationDouble>::surrogateIdField()
{
	return 0;
}

//ConfigurationFloat traits
Wt::Dbo::dbo_traits<ConfigurationFloat>::IdType Wt::Dbo::dbo_traits<ConfigurationFloat>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationFloat>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationFloat>::surrogateIdField()
{
	return 0;
}

//ConfigurationInt traits
Wt::Dbo::dbo_traits<ConfigurationInt>::IdType Wt::Dbo::dbo_traits<ConfigurationInt>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationInt>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationInt>::surrogateIdField()
{
	return 0;
}

//ConfigurationBool traits
Wt::Dbo::dbo_traits<ConfigurationString>::IdType Wt::Dbo::dbo_traits<ConfigurationString>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationString>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationString>::surrogateIdField()
{
	return 0;
}
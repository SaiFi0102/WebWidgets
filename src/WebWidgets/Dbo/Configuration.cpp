#include "Dbo/Configuration.h"

//Configuration Keys
ConfigurationKeys::ConfigurationKeys()
	: Name(std::string()), ModulePtr(Wt::Dbo::ptr<Module>()), Type(ConfigurationKeys::Bool)
{ }
ConfigurationKeys::ConfigurationKeys(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ValueTypes Type)
	: Name(Name), ModulePtr(ModulePtr), Type(Type)
{ }
bool ConfigurationKeys::operator== (const ConfigurationKeys &other) const
{
	return Name == other.Name && ModulePtr == other.ModulePtr && Type == other.Type;
}
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

//Overloaded dbo_trait for Configuration DBO
Wt::Dbo::dbo_traits<Configuration>::IdType Wt::Dbo::dbo_traits<Configuration>::invalidId()
{
	return Wt::Dbo::dbo_traits<Configuration>::IdType();
}
const char* Wt::Dbo::dbo_traits<Configuration>::surrogateIdField()
{
	return 0;
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

//ConfigurationEnum traits
Wt::Dbo::dbo_traits<ConfigurationEnum>::IdType Wt::Dbo::dbo_traits<ConfigurationEnum>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationEnum>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationEnum>::surrogateIdField()
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

//ConfigurationLongInt traits
Wt::Dbo::dbo_traits<ConfigurationLongInt>::IdType Wt::Dbo::dbo_traits<ConfigurationLongInt>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationLongInt>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationLongInt>::surrogateIdField()
{
	return 0;
}

//ConfigurationString traits
Wt::Dbo::dbo_traits<ConfigurationString>::IdType Wt::Dbo::dbo_traits<ConfigurationString>::invalidId()
{
	return Wt::Dbo::dbo_traits<ConfigurationString>::IdType();
}
const char* Wt::Dbo::dbo_traits<ConfigurationString>::surrogateIdField()
{
	return 0;
}

Configuration::Configuration()
{ }
Configuration::Configuration(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ConfigurationKeys::ValueTypes Type)
	: _Id(Name, ModulePtr, Type)
{ }
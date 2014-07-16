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
#ifndef CONFIGURATION_DBO_H
#define CONFIGURATION_DBO_H

#include "Common.h"
#include <ostream>
#include <Wt/Dbo/Dbo>

//Post directives
class Configuration;
class ConfigurationBool;
class ConfigurationDouble;
class ConfigurationFloat;
class ConfigurationInt;
class ConfigurationString;

//Configuration Keys structure
struct ConfigurationKeys
{
	std::string Name;
	std::string Application;

	ConfigurationKeys();
	ConfigurationKeys(std::string Name, std::string Application);

	bool operator< (const ConfigurationKeys& other) const;
	bool operator== (const ConfigurationKeys& other) const;
};

//Overloaded operator<< for Configuration Keys structure
std::ostream& operator<< (std::ostream& o, const ConfigurationKeys& c);

namespace Wt
{
	namespace Dbo
	{
		//Overloaded Wt::Dbo::field() for Configuration Keys structure
		template<class Action>
		void field(Action& action, ConfigurationKeys& Keys, const std::string& name, int size = -1)
		{
			field(action, Keys.Name,	"Name", 255);
			field(action, Keys.Application,	"Application", 255);
		}

		//Overloaded dbo_traits for Configuration DBO
		template<>
		struct dbo_traits<::Configuration> : public dbo_default_traits
		{
			typedef ConfigurationKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
}

//Configuration Keys structure
struct ConfigurationForeignKeys
{
	Wt::Dbo::ptr<Configuration> ConfigurationDbo;
	std::string Name;
	std::string Application;

	ConfigurationForeignKeys();
	ConfigurationForeignKeys(Wt::Dbo::ptr<Configuration> ConfigurationDbo, std::string Name, std::string Application);

	bool operator< (const ConfigurationForeignKeys& other) const;
	bool operator== (const ConfigurationForeignKeys& other) const;
};

//Overloaded operator<< for Configuration Keys structure
std::ostream& operator<< (std::ostream& o, const ConfigurationForeignKeys& c);

namespace Wt
{
	namespace Dbo
	{
		//Overloaded Wt::Dbo::field() for Configuration Keys structure
		template<class Action>
		void field(Action& action, ConfigurationForeignKeys& Keys, const std::string& name, int size = -1)
		{
			belongsTo(action, Keys.ConfigurationDbo, "Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			field(action, Keys.Name, "Name", 255);
			field(action, Keys.Application, "Application", 255);
		}
		
		//Overloaded dbo_traits for ConfigurationBool DBO
		template<>
		struct dbo_traits<ConfigurationBool> : public dbo_default_traits
		{
			typedef ConfigurationForeignKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationDouble DBO
		template<>
		struct dbo_traits<ConfigurationDouble> : public dbo_default_traits
		{
			typedef ConfigurationForeignKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationFloat DBO
		template<>
		struct dbo_traits<ConfigurationFloat> : public dbo_default_traits
		{
			typedef ConfigurationForeignKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationInt DBO
		template<>
		struct dbo_traits<ConfigurationInt> : public dbo_default_traits
		{
			typedef ConfigurationForeignKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationString DBO
		template<>
		struct dbo_traits<ConfigurationString> : public dbo_default_traits
		{
			typedef ConfigurationForeignKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
};

//Configuration DBO class
class Configuration
{
	public:
		enum ValueTypes
		{
			Bool = 0,
			Double = 1,
			Float = 2,
			Int = 3,
			String = 4,
		};

		//hasMany relations
		Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationBool> >		ConfigurationBoolDbo;
		Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationDouble> >	ConfigurationDoubleDbo;
		Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationFloat> >		ConfigurationFloatDbo;
		Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationInt> >		ConfigurationIntDbo;
		Wt::Dbo::collection< Wt::Dbo::ptr<ConfigurationString> >	ConfigurationStringDbo;

		//Fields
		ConfigurationKeys				Id;
		std::string						Title;
		ValueTypes						Type;
		boost::optional<std::string>	Details;

		//Persistence Method
		template<class Action>
		void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Id");
			Wt::Dbo::field(a, Title,	"Title");
			Wt::Dbo::field(a, Type,		"Type");
			Wt::Dbo::field(a, Details,	"Details");
			
			Wt::Dbo::hasMany(a, ConfigurationBoolDbo, Wt::Dbo::ManyToOne, "Configuration");
			Wt::Dbo::hasMany(a, ConfigurationDoubleDbo, Wt::Dbo::ManyToOne, "Configuration");
			Wt::Dbo::hasMany(a, ConfigurationFloatDbo, Wt::Dbo::ManyToOne, "Configuration");
			Wt::Dbo::hasMany(a, ConfigurationIntDbo, Wt::Dbo::ManyToOne, "Configuration");
			Wt::Dbo::hasMany(a, ConfigurationStringDbo, Wt::Dbo::ManyToOne, "Configuration");
		}
};

//ConfigurationBool DBO Class
class ConfigurationBool
{
	public:
		//belongsTo
		ConfigurationForeignKeys Id;
		bool Value;

		template<class Action>void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Configuration");
			Wt::Dbo::field(a, Value, "Value");
		}
};

//ConfigurationDouble DBO Class
class ConfigurationDouble
{
	public:
		//belongsTo
		ConfigurationForeignKeys Id;
		double Value;

		template<class Action>void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Configuration");
			Wt::Dbo::field(a, Value, "Value");
		}
};

//ConfigurationFloat DBO Class
class ConfigurationFloat
{
	public:
		//belongsTo
		ConfigurationForeignKeys Id;
		float Value;

		template<class Action>void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Configuration");
			Wt::Dbo::field(a, Value, "Value");
		}
};

//ConfigurationInt DBO Class
class ConfigurationInt
{
	public:
		//belongsTo
		ConfigurationForeignKeys Id;
		int Value;

		template<class Action>void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Configuration");
			Wt::Dbo::field(a, Value, "Value");
		}
};

//ConfigurationString DBO Class
class ConfigurationString
{
	public:
		//belongsTo
		ConfigurationForeignKeys Id;
		boost::optional<std::string> Value;

		template<class Action>void persist(Action& a)
		{
			Wt::Dbo::id(a, Id, "Configuration");
			Wt::Dbo::field(a, Value, "Value");
		}
};

#endif
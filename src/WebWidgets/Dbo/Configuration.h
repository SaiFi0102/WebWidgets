#ifndef CONFIGURATION_DBO_H
#define CONFIGURATION_DBO_H

#include <ostream>
#include <Wt/Dbo/Dbo>
#include "Dbo/Module.h"

class Configuration;
class ConfigurationBool;
class ConfigurationDouble;
class ConfigurationEnum;
class ConfigurationFloat;
class ConfigurationInt;
class ConfigurationLongInt;
class ConfigurationString;
class ConfigurationEnumValue;

typedef Wt::Dbo::collection< Wt::Dbo::ptr< Configuration > > ConfigurationCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationBool > > BoolCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationEnum > > EnumCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationDouble > > DoubleCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationFloat > > FloatCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationInt > > IntCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationLongInt > > LongIntCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationString > > StringCollections;
typedef Wt::Dbo::collection< Wt::Dbo::ptr< ConfigurationEnumValue > > EnumValueCollections;

//Configuration Keys structure
struct ConfigurationKeys
{
	enum ValueTypes
	{
		Bool =		0,
		Double =	1,
		Enum =		2,
		Float =		3,
		Int =		4,
		LongInt =	5,
		String =	6,
	};

	std::string Name;
	Wt::Dbo::ptr<Module> ModulePtr;
	ValueTypes Type;

	ConfigurationKeys();
	ConfigurationKeys(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ValueTypes Type);

	bool operator< (const ConfigurationKeys &other) const;
	bool operator== (const ConfigurationKeys &other) const;
};
std::ostream &operator<< (std::ostream &o, const ConfigurationKeys &c);

namespace Wt
{
	namespace Dbo
	{
		//Overloaded Wt::Dbo::field() for Configuration Keys structure
		template<class Action>
		void field(Action &action, ConfigurationKeys &Keys, const std::string &name, int size = -1)
		{
			field(action, Keys.Name, "Name", 256);
			belongsTo(action, Keys.ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			field(action, Keys.Type, "Type");
		}

		//Overloaded dbo_traits for Configuration DBO
		template<>
		struct dbo_traits<::Configuration> : public dbo_default_traits
		{
			typedef ConfigurationKeys IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationBool DBO
		template<>
		struct dbo_traits<ConfigurationBool> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationDouble DBO
		template<>
		struct dbo_traits<ConfigurationDouble> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationEnum DBO
		template<>
		struct dbo_traits<ConfigurationEnum> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationFloat DBO
		template<>
		struct dbo_traits<ConfigurationFloat> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationInt DBO
		template<>
		struct dbo_traits<ConfigurationInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationLongInt DBO
		template<>
		struct dbo_traits<ConfigurationLongInt> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
		//Overloaded dbo_traits for ConfigurationString DBO
		template<>
		struct dbo_traits<ConfigurationString> : public dbo_default_traits
		{
			typedef Wt::Dbo::ptr<::Configuration> IdType;
			static IdType invalidId();
			static const char *surrogateIdField();
		};
	}
}

//Configuration DBO class
class Configuration : public Wt::Dbo::Dbo<Configuration>
{
	public:
		//hasOne relations
		Wt::Dbo::weak_ptr<ConfigurationBool>	BoolPtr;
		Wt::Dbo::weak_ptr<ConfigurationDouble>	DoublePtr;
		Wt::Dbo::weak_ptr<ConfigurationEnum>	EnumPtr;
		Wt::Dbo::weak_ptr<ConfigurationFloat>	FloatPtr;
		Wt::Dbo::weak_ptr<ConfigurationInt>		IntPtr;
		Wt::Dbo::weak_ptr<ConfigurationLongInt>	LongIntPtr;
		Wt::Dbo::weak_ptr<ConfigurationString>	StringPtr;

		//Fields
		std::string						Title;
		boost::optional<std::string>	Details;
		bool							RestartRequired;
		boost::optional<std::string>	ExpertWarning;

		Configuration();
		Configuration(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ConfigurationKeys::ValueTypes Type);

		//Persistence Method
		template<class Action>
		void persist(Action &a)
		{
			Wt::Dbo::id(a, _Id);
			Wt::Dbo::field(a, Title,			"Title");
			Wt::Dbo::field(a, Details,			"Details");
			Wt::Dbo::field(a, RestartRequired,	"RestartRequired");
			Wt::Dbo::field(a, ExpertWarning,	"ExpertWarning");

			Wt::Dbo::hasOne(a, BoolPtr, "Configuration");
			Wt::Dbo::hasOne(a, DoublePtr, "Configuration");
			Wt::Dbo::hasOne(a, EnumPtr, "Configuration");
			Wt::Dbo::hasOne(a, FloatPtr, "Configuration");
			Wt::Dbo::hasOne(a, IntPtr, "Configuration");
			Wt::Dbo::hasOne(a, LongIntPtr, "Configuration");
			Wt::Dbo::hasOne(a, StringPtr, "Configuration");
		}
		static const char *TableName()
		{
			return "configurations";
		}

	private:
		ConfigurationKeys _Id;
};

//ConfigurationBool DBO Class
class ConfigurationBool : public Wt::Dbo::Dbo<ConfigurationBool>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		bool Value;
		bool DefaultValue;
		boost::optional<bool> RecommendedValue;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");
		}
		static const char *TableName()
		{
			return "configurationbools";
		}
};

//ConfigurationDouble DBO Class
class ConfigurationDouble : public Wt::Dbo::Dbo<ConfigurationDouble>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		double Value;
		double DefaultValue;
		boost::optional<double> RecommendedValue;

		boost::optional<double> MinValue;
		boost::optional<double> MaxValue;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue,	"RecommendedValue");
			Wt::Dbo::field(a, MinValue,			"MinValue");
			Wt::Dbo::field(a, MaxValue,			"MaxValue");
		}
		static const char *TableName()
		{
			return "configurationdoubles";
		}
};

class ConfigurationEnumValue : public Wt::Dbo::Dbo<ConfigurationEnumValue>
{
	public:
		std::string Title;
		boost::optional<std::string> Details;
		int Value;

		Wt::Dbo::ptr<ConfigurationEnum> EnumPtr; //belongsTo

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::field(a, Title, "Title");
			Wt::Dbo::field(a, Details, "Details");
			Wt::Dbo::field(a, Value, "Value");

			Wt::Dbo::belongsTo(a, EnumPtr, "Enum", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		static const char *TableName()
		{
			return "configurationenumvalues";
		}
};

//ConfigurationEnum DBO Class
class ConfigurationEnum : public Wt::Dbo::Dbo<ConfigurationEnum>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

		//hasOne
		int Value;
		int DefaultValue;
		boost::optional<int> RecommendedValue;

		EnumValueCollections EnumValueCollection;
		
		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr, "Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value, "Value");
			Wt::Dbo::field(a, DefaultValue, "DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");

			Wt::Dbo::hasMany(a, EnumValueCollection, Wt::Dbo::ManyToOne, "Enum");
		}
		static const char *TableName()
		{
			return "configurationenums";
		}
};

//ConfigurationFloat DBO Class
class ConfigurationFloat : public Wt::Dbo::Dbo<ConfigurationFloat>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		float Value;
		float DefaultValue;
		boost::optional<float> RecommendedValue;

		boost::optional<float> MinValue;
		boost::optional<float> MaxValue;
		
		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue,	"RecommendedValue");
			Wt::Dbo::field(a, MinValue,			"MinValue");
			Wt::Dbo::field(a, MaxValue,			"MaxValue");
		}
		static const char *TableName()
		{
			return "configurationfloats";
		}
};

//ConfigurationInt DBO Class
class ConfigurationInt : public Wt::Dbo::Dbo<ConfigurationInt>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		int Value;
		boost::optional<int> DefaultValue;
		boost::optional<int> RecommendedValue;

		boost::optional<int> MinValue;
		boost::optional<int> MaxValue;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");
			Wt::Dbo::field(a, MinValue,			"MinValue");
			Wt::Dbo::field(a, MaxValue,			"MaxValue");
		}
		static const char *TableName()
		{
			return "configurationints";
		}
};

//ConfigurationLongInt DBO Class
class ConfigurationLongInt : public Wt::Dbo::Dbo<ConfigurationLongInt>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		long long Value;
		boost::optional<long long> DefaultValue;
		boost::optional<long long> RecommendedValue;

		boost::optional<long long> MinValue;
		boost::optional<long long> MaxValue;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");
			Wt::Dbo::field(a, MinValue,			"MinValue");
			Wt::Dbo::field(a, MaxValue,			"MaxValue");
		}
		static const char *TableName()
		{
			return "configurationlongints";
		}
};

//ConfigurationString DBO Class
class ConfigurationString : public Wt::Dbo::Dbo<ConfigurationString>
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		boost::optional<std::string> Value;
		boost::optional<std::string> DefaultValue;
		boost::optional<std::string> RecommendedValue;
		boost::optional<std::string> ExampleValue;

		boost::optional<int> MinLength;
		boost::optional<int> MaxLength;

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");
			Wt::Dbo::field(a, ExampleValue,		"ExampleValue");
			Wt::Dbo::field(a, MinLength,		"MinLength");
			Wt::Dbo::field(a, MaxLength,		"MaxLength");
		}
		static const char *TableName()
		{
			return "configurationstrings";
		}
};

#endif
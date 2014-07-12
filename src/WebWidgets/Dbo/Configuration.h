#ifndef CONFIGURATION_DBO_H
#define CONFIGURATION_DBO_H

#include <ostream>
#include "DboTraits.h"
#include "Dbo/Module.h"

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

	ConfigurationKeys()
		: Type(ConfigurationKeys::Bool)
	{ }
	ConfigurationKeys(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ValueTypes Type)
		: Name(Name), ModulePtr(ModulePtr), Type(Type)
	{ }

	bool operator< (const ConfigurationKeys &other) const;
	bool operator== (const ConfigurationKeys &other) const
	{
		return Name == other.Name && ModulePtr == other.ModulePtr && Type == other.Type;
	}
};
std::ostream &operator<< (std::ostream &o, const ConfigurationKeys &c);

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

		Configuration() { }
		Configuration(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ConfigurationKeys::ValueTypes Type)
			: _Id(Name, ModulePtr, Type)
		{ }

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
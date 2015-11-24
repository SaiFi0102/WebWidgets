#ifndef CONFIGURATION_DBO_H
#define CONFIGURATION_DBO_H

#include <ostream>
#include "DboTraits.h"
#include "Dbo/Module.h"

//Configuration DBO class
class Configuration
{
	public:
		enum ValueTypes
		{
			Bool	= 0,
			Double	= 1,
			Enum	= 2,
			Float	= 3,
			Int		= 4,
			LongInt	= 5,
			String	= 6,
		};

	protected:
		Wt::Dbo::ptr<Module> _ModulePtr;
		std::string _Name;
		ValueTypes _Type;
		
	public:
		Wt::Dbo::ptr<SingularKey> TitleKey;
		Wt::Dbo::ptr<SingularKey> DetailsKey;
		Wt::Dbo::ptr<SingularKey> ExpertWarningKey;
		bool RestartRequired;

		//hasOne relations
		Wt::Dbo::weak_ptr<ConfigurationBool>	BoolPtr;
		Wt::Dbo::weak_ptr<ConfigurationDouble>	DoublePtr;
		Wt::Dbo::weak_ptr<ConfigurationEnum>	EnumPtr;
		Wt::Dbo::weak_ptr<ConfigurationFloat>	FloatPtr;
		Wt::Dbo::weak_ptr<ConfigurationInt>		IntPtr;
		Wt::Dbo::weak_ptr<ConfigurationLongInt>	LongIntPtr;
		Wt::Dbo::weak_ptr<ConfigurationString>	StringPtr;

		Configuration()
			: _Type(Bool)
		{ }
		Configuration(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr, ValueTypes Type)
			: _Name(Name), _Type(Type), _ModulePtr(ModulePtr)
		{ }

		Wt::Dbo::ptr<Module> ModulePtr() const { return _ModulePtr; };
		std::string Name() const { return _Name; };
		ValueTypes Type() const { return _Type; };

		//Persistence Method
		template<class Action>
		void persist(Action &a)
		{
			Wt::Dbo::field(a, _Name, "Name", 50);
			Wt::Dbo::belongsTo(a, _ModulePtr, "Module", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, _Type, "Type");
			Wt::Dbo::belongsTo(a, TitleKey, "TitleKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(a, DetailsKey, "DetailsKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::belongsTo(a, ExpertWarningKey, "ExpertWarningKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade);
			Wt::Dbo::field(a, RestartRequired, "RestartRequired");

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
			return "configuration";
		}
};

class ConfigurationDataKey
{
	protected:
		std::string _Name;
		long long _ModuleId;

		ConfigurationDataKey(const std::string &Name, long long ModuleId)
			: _Name(Name), _ModuleId(ModuleId)
		{}

	public:
		std::string Name() const { return _Name; };
		long long ModuleId() const { return _ModuleId; };
};

//ConfigurationBool DBO Class
class BaseConfigurationBool
{
	public:
		bool Value;
		bool DefaultValue;
		boost::optional<bool> RecommendedValue;
};
class ConfigurationBool : public BaseConfigurationBool
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::id(a, ConfigurationPtr,	"Configuration", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value,			"Value");
			Wt::Dbo::field(a, DefaultValue,		"DefaultValue");
			Wt::Dbo::field(a, RecommendedValue, "RecommendedValue");
		}
		static const char *TableName()
		{
			return "configurationbool";
		}
};
class ConfigurationBoolData : public BaseConfigurationBool, public ConfigurationDataKey
{
	public:
		ConfigurationBoolData(Wt::Dbo::ptr<ConfigurationBool> Ptr)
			: BaseConfigurationBool(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

//ConfigurationDouble DBO Class
class BaseConfigurationDouble
{
	public:
		double Value;
		double DefaultValue;
		boost::optional<double> RecommendedValue;
		boost::optional<double> MinValue;
		boost::optional<double> MaxValue;
};
class ConfigurationDouble : public BaseConfigurationDouble
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

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
			return "configurationdouble";
		}
};
class ConfigurationDoubleData : public BaseConfigurationDouble, public ConfigurationDataKey
{
	public:
		ConfigurationDoubleData(Wt::Dbo::ptr<ConfigurationDouble> Ptr)
			: BaseConfigurationDouble(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

class ConfigurationEnumValue
{
	public:
		Wt::Dbo::ptr<SingularKey> TitleKey;
		Wt::Dbo::ptr<SingularKey> DetailsKey;
		int Value;

		Wt::Dbo::ptr<ConfigurationEnum> EnumPtr; //belongsTo

		template<class Action>void persist(Action &a)
		{
			Wt::Dbo::belongsTo(a, TitleKey, "TitleKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::belongsTo(a, DetailsKey, "DetailsKey", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
			Wt::Dbo::field(a, Value, "Value");

			Wt::Dbo::belongsTo(a, EnumPtr, "Enum", Wt::Dbo::OnDeleteCascade | Wt::Dbo::OnUpdateCascade | Wt::Dbo::NotNull);
		}
		static const char *TableName()
		{
			return "configurationenumvalue";
		}
};

//ConfigurationEnum DBO Class
class BaseConfigurationEnum
{
	public:
		int Value;
		int DefaultValue;
		boost::optional<int> RecommendedValue;
};
class ConfigurationEnum : public BaseConfigurationEnum
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
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
			return "configurationenum";
		}
};
class ConfigurationEnumData : public BaseConfigurationEnum, public ConfigurationDataKey
{
	public:
		ConfigurationEnumData(Wt::Dbo::ptr<ConfigurationEnum> Ptr)
			: BaseConfigurationEnum(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

//ConfigurationFloat DBO Class
class BaseConfigurationFloat
{
	public:
		float Value;
		float DefaultValue;
		boost::optional<float> RecommendedValue;
		boost::optional<float> MinValue;
		boost::optional<float> MaxValue;
};
class ConfigurationFloat : public BaseConfigurationFloat
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo
		
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
			return "configurationfloat";
		}
};
class ConfigurationFloatData : public BaseConfigurationFloat, public ConfigurationDataKey
{
	public:
		ConfigurationFloatData(Wt::Dbo::ptr<ConfigurationFloat> Ptr)
			: BaseConfigurationFloat(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

//ConfigurationInt DBO Class
class BaseConfigurationInt
{
	public:
		int Value;
		boost::optional<int> DefaultValue;
		boost::optional<int> RecommendedValue;
		boost::optional<int> MinValue;
		boost::optional<int> MaxValue;
};
class ConfigurationInt : public BaseConfigurationInt
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

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
			return "configurationint";
		}
};
class ConfigurationIntData : public BaseConfigurationInt, public ConfigurationDataKey
{
	public:
		ConfigurationIntData(Wt::Dbo::ptr<ConfigurationInt> Ptr)
			: BaseConfigurationInt(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

//ConfigurationLongInt DBO Class
class BaseConfigurationLongInt
{
	public:
		long long Value;
		boost::optional<long long> DefaultValue;
		boost::optional<long long> RecommendedValue;
		boost::optional<long long> MinValue;
		boost::optional<long long> MaxValue;
};
class ConfigurationLongInt : public BaseConfigurationLongInt
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

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
			return "configurationlongint";
		}
};
class ConfigurationLongIntData : public BaseConfigurationLongInt, public ConfigurationDataKey
{
	public:
		ConfigurationLongIntData(Wt::Dbo::ptr<ConfigurationLongInt> Ptr)
			: BaseConfigurationLongInt(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

//ConfigurationString DBO Class
class BaseConfigurationString
{
	public:
		std::string Value;
		std::string DefaultValue;
		std::string RecommendedValue;
		std::string ExampleValue;
		boost::optional<int> MinLength;
		boost::optional<int> MaxLength;
};
class ConfigurationString : public BaseConfigurationString
{
	public:
		Wt::Dbo::ptr<Configuration> ConfigurationPtr; //belongsTo

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
			return "configurationstring";
		}
};
class ConfigurationStringData : public BaseConfigurationString, public ConfigurationDataKey
{
	public:
		ConfigurationStringData(Wt::Dbo::ptr<ConfigurationString> Ptr)
			: BaseConfigurationString(*Ptr), ConfigurationDataKey(Ptr->ConfigurationPtr->Name(), Ptr->ConfigurationPtr->ModulePtr().id())
		{ }
};

#endif
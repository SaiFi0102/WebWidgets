#include "DboDatabase/ConfigurationsProxy.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/Application.h"

ConfigurationsProxy::ConfigurationsProxy(ConfigurationsDatabase *Database, Application *App)
	: _Database(Database), _App(App)
{
	ReadLock lock(_Database);
	BoolMap = BoolMaps(Database->BoolMap);
	DoubleMap = DoubleMaps(Database->DoubleMap);
	EnumMap = EnumMaps(Database->EnumMap);
	FloatMap = FloatMaps(Database->FloatMap);
	IntMap = IntMaps(Database->IntMap);
	LongIntMap = LongIntMaps(Database->LongIntMap);
	StringMap = StringMaps(Database->StringMap);
}

Wt::Dbo::ptr<ConfigurationBool> ConfigurationsProxy::GetBoolPtr(const std::string &Name, long long ModuleId) const
{
	BoolMaps::const_iterator itr = BoolMap.find(std::make_pair(ModuleId, Name));
	if(itr == BoolMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationBool>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationDouble> ConfigurationsProxy::GetDoublePtr(const std::string &Name, long long ModuleId) const
{
	DoubleMaps::const_iterator itr = DoubleMap.find(std::make_pair(ModuleId, Name));
	if(itr == DoubleMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationDouble>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationEnum> ConfigurationsProxy::GetEnumPtr(const std::string &Name, long long ModuleId) const
{
	EnumMaps::const_iterator itr = EnumMap.find(std::make_pair(ModuleId, Name));
	if(itr == EnumMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationEnum>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationFloat> ConfigurationsProxy::GetFloatPtr(const std::string &Name, long long ModuleId) const
{
	FloatMaps::const_iterator itr = FloatMap.find(std::make_pair(ModuleId, Name));
	if(itr == FloatMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationFloat>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationInt> ConfigurationsProxy::GetIntPtr(const std::string &Name, long long ModuleId) const
{
	IntMaps::const_iterator itr = IntMap.find(std::make_pair(ModuleId, Name));
	if(itr == IntMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationInt>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationLongInt> ConfigurationsProxy::GetLongIntPtr(const std::string &Name, long long ModuleId) const
{
	LongIntMaps::const_iterator itr = LongIntMap.find(std::make_pair(ModuleId, Name));
	if(itr == LongIntMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationLongInt>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationString> ConfigurationsProxy::GetStringPtr(const std::string &Name, long long ModuleId) const
{
	StringMaps::const_iterator itr = StringMap.find(std::make_pair(ModuleId, Name));
	if(itr == StringMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationString>();
	}
	return itr->second;
}

//Boolean getter
bool ConfigurationsProxy::GetBool(const std::string &Name, long long ModuleId, bool Default) const
{
	Wt::Dbo::ptr<ConfigurationBool> BoolPtr = GetBoolPtr(Name, ModuleId);
	if(!BoolPtr)
	{
		_App->log("warn") << "BoolPtr not found in ConfigurationsProxy in GetBool(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return BoolPtr->Value;
}

//Double getter
double ConfigurationsProxy::GetDouble(const std::string &Name, long long ModuleId, double Default) const
{
	Wt::Dbo::ptr<ConfigurationDouble> DoublePtr = GetDoublePtr(Name, ModuleId);
	if(!DoublePtr)
	{
		_App->log("warn") << "DoublePtr not found in ConfigurationsProxy in GetDouble(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return DoublePtr->Value;
}

//Enum getter
int ConfigurationsProxy::GetEnum(const std::string &Name, long long ModuleId, int Default) const
{
	Wt::Dbo::ptr<ConfigurationEnum> EnumPtr = GetEnumPtr(Name, ModuleId);
	if(!EnumPtr)
	{
		_App->log("warn") << "EnumPtr not found in ConfigurationsProxy in GetEnum(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return EnumPtr->Value;
}

//Float getter
float ConfigurationsProxy::GetFloat(const std::string &Name, long long ModuleId, float Default) const
{
	Wt::Dbo::ptr<ConfigurationFloat> FloatPtr = GetFloatPtr(Name, ModuleId);
	if(!FloatPtr)
	{
		_App->log("warn") << "FloatPtr not found in ConfigurationsProxy in GetFloat(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return FloatPtr->Value;
}

//Integer getter
int ConfigurationsProxy::GetInt(const std::string &Name, long long ModuleId, int Default) const
{
	Wt::Dbo::ptr<ConfigurationInt> IntPtr = GetIntPtr(Name, ModuleId);
	if(!IntPtr)
	{
		_App->log("warn") << "IntPtr not found in ConfigurationsProxy in GetInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return IntPtr->Value;
}

//Long integer getter
long long ConfigurationsProxy::GetLongInt(const std::string &Name, long long ModuleId, long long Default) const
{
	Wt::Dbo::ptr<ConfigurationLongInt> LongIntPtr = GetLongIntPtr(Name, ModuleId);
	if(!LongIntPtr)
	{
		_App->log("warn") << "LongIntPtr not found in ConfigurationsProxy in GetLongInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return LongIntPtr->Value;
}

//String getter
std::string ConfigurationsProxy::GetStr(const std::string &Name, long long ModuleId, std::string Default) const
{
	Wt::Dbo::ptr<ConfigurationString> StringPtr = GetStringPtr(Name, ModuleId);
	if(!StringPtr)
	{
		_App->log("warn") << "StringPtr not found in ConfigurationsProxy in GetString(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	if(!StringPtr->Value.is_initialized())
	{
		return std::string();
	}
	return *StringPtr->Value;
}

ConfigurationsDatabase *ConfigurationsProxy::Database()
{
	return _Database;
}

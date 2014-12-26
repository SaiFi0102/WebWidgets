#include "DboDatabase/ConfigurationsCache.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/Application.h"

ConfigurationsCache::ConfigurationsCache(ConfigurationsDatabase *Database)
	: _Database(Database)
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

boost::shared_ptr<ConfigurationBoolData> ConfigurationsCache::GetBoolPtr(const std::string &Name, long long ModuleId) const
{
	BoolMaps::const_iterator itr = BoolMap.find(std::make_pair(ModuleId, Name));
	if(itr == BoolMap.end())
	{
		return boost::shared_ptr<ConfigurationBoolData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationDoubleData> ConfigurationsCache::GetDoublePtr(const std::string &Name, long long ModuleId) const
{
	DoubleMaps::const_iterator itr = DoubleMap.find(std::make_pair(ModuleId, Name));
	if(itr == DoubleMap.end())
	{
		return boost::shared_ptr<ConfigurationDoubleData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationEnumData> ConfigurationsCache::GetEnumPtr(const std::string &Name, long long ModuleId) const
{
	EnumMaps::const_iterator itr = EnumMap.find(std::make_pair(ModuleId, Name));
	if(itr == EnumMap.end())
	{
		return boost::shared_ptr<ConfigurationEnumData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationFloatData> ConfigurationsCache::GetFloatPtr(const std::string &Name, long long ModuleId) const
{
	FloatMaps::const_iterator itr = FloatMap.find(std::make_pair(ModuleId, Name));
	if(itr == FloatMap.end())
	{
		return boost::shared_ptr<ConfigurationFloatData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationIntData> ConfigurationsCache::GetIntPtr(const std::string &Name, long long ModuleId) const
{
	IntMaps::const_iterator itr = IntMap.find(std::make_pair(ModuleId, Name));
	if(itr == IntMap.end())
	{
		return boost::shared_ptr<ConfigurationIntData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationLongIntData> ConfigurationsCache::GetLongIntPtr(const std::string &Name, long long ModuleId) const
{
	LongIntMaps::const_iterator itr = LongIntMap.find(std::make_pair(ModuleId, Name));
	if(itr == LongIntMap.end())
	{
		return boost::shared_ptr<ConfigurationLongIntData>();
	}
	return itr->second;
}

boost::shared_ptr<ConfigurationStringData> ConfigurationsCache::GetStringPtr(const std::string &Name, long long ModuleId) const
{
	StringMaps::const_iterator itr = StringMap.find(std::make_pair(ModuleId, Name));
	if(itr == StringMap.end())
	{
		return boost::shared_ptr<ConfigurationStringData>();
	}
	return itr->second;
}

//Boolean getter
bool ConfigurationsCache::GetBool(const std::string &Name, long long ModuleId, bool Default) const
{
	boost::shared_ptr<ConfigurationBoolData> BoolPtr = GetBoolPtr(Name, ModuleId);
	if(!BoolPtr)
	{
		Wt::log("warn") << "BoolPtr not found in ConfigurationsCache in GetBool(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return BoolPtr->Value;
}

//Double getter
double ConfigurationsCache::GetDouble(const std::string &Name, long long ModuleId, double Default) const
{
	boost::shared_ptr<ConfigurationDoubleData> DoublePtr = GetDoublePtr(Name, ModuleId);
	if(!DoublePtr)
	{
		Wt::log("warn") << "DoublePtr not found in ConfigurationsCache in GetDouble(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return DoublePtr->Value;
}

//Enum getter
int ConfigurationsCache::GetEnum(const std::string &Name, long long ModuleId, int Default) const
{
	boost::shared_ptr<ConfigurationEnumData> EnumPtr = GetEnumPtr(Name, ModuleId);
	if(!EnumPtr)
	{
		Wt::log("warn") << "EnumPtr not found in ConfigurationsCache in GetEnum(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return EnumPtr->Value;
}

//Float getter
float ConfigurationsCache::GetFloat(const std::string &Name, long long ModuleId, float Default) const
{
	boost::shared_ptr<ConfigurationFloatData> FloatPtr = GetFloatPtr(Name, ModuleId);
	if(!FloatPtr)
	{
		Wt::log("warn") << "FloatPtr not found in ConfigurationsCache in GetFloat(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return FloatPtr->Value;
}

//Integer getter
int ConfigurationsCache::GetInt(const std::string &Name, long long ModuleId, int Default) const
{
	boost::shared_ptr<ConfigurationIntData> IntPtr = GetIntPtr(Name, ModuleId);
	if(!IntPtr)
	{
		Wt::log("warn") << "IntPtr not found in ConfigurationsCache in GetInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return IntPtr->Value;
}

//Long integer getter
long long ConfigurationsCache::GetLongInt(const std::string &Name, long long ModuleId, long long Default) const
{
	boost::shared_ptr<ConfigurationLongIntData> LongIntPtr = GetLongIntPtr(Name, ModuleId);
	if(!LongIntPtr)
	{
		Wt::log("warn") << "LongIntPtr not found in ConfigurationsCache in GetLongInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return LongIntPtr->Value;
}

//String getter
std::string ConfigurationsCache::GetStr(const std::string &Name, long long ModuleId, std::string Default) const
{
	boost::shared_ptr<ConfigurationStringData> StringPtr = GetStringPtr(Name, ModuleId);
	if(!StringPtr)
	{
		Wt::log("warn") << "StringPtr not found in ConfigurationsCache in GetString(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	if(!StringPtr->Value.is_initialized())
	{
		return std::string();
	}
	return *StringPtr->Value;
}

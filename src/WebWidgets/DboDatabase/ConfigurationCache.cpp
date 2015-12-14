#include "DboDatabase/ConfigurationCache.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

namespace WW
{

ConfigurationCache::ConfigurationCache(ConfigurationsDatabase *database)
	: _database(database)
{
	ReadLock lock(_database->manager());
	_boolMap = BoolMap(database->_boolMap);
	_doubleMap = DoubleMap(database->_doubleMap);
	_enumMap = EnumMap(database->_enumMap);
	_floatMap = FloatMap(database->_floatMap);
	_intMap = IntMap(database->_intMap);
	_longIntMap = LongIntMap(database->_longIntMap);
	_stringMap = StringMap(database->_stringMap);
}

Ddo::ptr<Ddo::ConfigurationBool> ConfigurationCache::getBoolPtr(const std::string &name, long long moduleId) const
{
	BoolMap::const_iterator itr = _boolMap.find(std::make_pair(moduleId, name));
	if(itr == _boolMap.end())
		return Ddo::ptr<Ddo::ConfigurationBool>();

	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationDouble> ConfigurationCache::getDoublePtr(const std::string &name, long long moduleId) const
{
	DoubleMap::const_iterator itr = _doubleMap.find(std::make_pair(moduleId, name));
	if(itr == _doubleMap.end())
		return Ddo::ptr<Ddo::ConfigurationDouble>();

	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationEnum> ConfigurationCache::getEnumPtr(const std::string &name, long long moduleId) const
{
	EnumMap::const_iterator itr = _enumMap.find(std::make_pair(moduleId, name));
	if(itr == _enumMap.end())
		return Ddo::ptr<Ddo::ConfigurationEnum>();

	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationFloat> ConfigurationCache::getFloatPtr(const std::string &name, long long moduleId) const
{
	FloatMap::const_iterator itr = _floatMap.find(std::make_pair(moduleId, name));
	if(itr == _floatMap.end())
	{
		return Ddo::ptr<Ddo::ConfigurationFloat>();
	}
	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationInt> ConfigurationCache::getIntPtr(const std::string &name, long long moduleId) const
{
	IntMap::const_iterator itr = _intMap.find(std::make_pair(moduleId, name));
	if(itr == _intMap.end())
		return Ddo::ptr<Ddo::ConfigurationInt>();

	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationLongInt> ConfigurationCache::getLongIntPtr(const std::string &name, long long moduleId) const
{
	LongIntMap::const_iterator itr = _longIntMap.find(std::make_pair(moduleId, name));
	if(itr == _longIntMap.end())
		return Ddo::ptr<Ddo::ConfigurationLongInt>();

	return itr->second;
}

Ddo::ptr<Ddo::ConfigurationString> ConfigurationCache::getStringPtr(const std::string &name, long long moduleId) const
{
	StringMap::const_iterator itr = _stringMap.find(std::make_pair(moduleId, name));
	if(itr == _stringMap.end())
		return Ddo::ptr<Ddo::ConfigurationString>();

	return itr->second;
}

//Boolean getter
bool ConfigurationCache::getBool(const std::string &name, long long moduleId, bool defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationBool> boolPtr = getBoolPtr(name, moduleId);
	if(!boolPtr)
	{
		Wt::log("warn") << "BoolPtr not found in ConfigurationsCache in GetBool(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return boolPtr->value;
}

//Double getter
double ConfigurationCache::getDouble(const std::string &name, long long moduleId, double defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationDouble> doublePtr = getDoublePtr(name, moduleId);
	if(!doublePtr)
	{
		Wt::log("warn") << "DoublePtr not found in ConfigurationsCache in GetDouble(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return doublePtr->value;
}

//Enum getter
int ConfigurationCache::getEnum(const std::string &name, long long moduleId, int defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationEnum> enumPtr = getEnumPtr(name, moduleId);
	if(!enumPtr)
	{
		Wt::log("warn") << "EnumPtr not found in ConfigurationsCache in GetEnum(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return enumPtr->value;
}

//Float getter
float ConfigurationCache::getFloat(const std::string &name, long long moduleId, float defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationFloat> floatPtr = getFloatPtr(name, moduleId);
	if(!floatPtr)
	{
		Wt::log("warn") << "FloatPtr not found in ConfigurationsCache in GetFloat(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return floatPtr->value;
}

//Integer getter
int ConfigurationCache::getInt(const std::string &name, long long moduleId, int defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationInt> intPtr = getIntPtr(name, moduleId);
	if(!intPtr)
	{
		Wt::log("warn") << "IntPtr not found in ConfigurationsCache in GetInt(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return intPtr->value;
}

//Long integer getter
long long ConfigurationCache::getLongInt(const std::string &name, long long moduleId, long long defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationLongInt> longIntPtr = getLongIntPtr(name, moduleId);
	if(!longIntPtr)
	{
		Wt::log("warn") << "LongIntPtr not found in ConfigurationsCache in GetLongInt(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return longIntPtr->value;
}

//String getter
std::string ConfigurationCache::getStr(const std::string &name, long long moduleId, std::string defaultValue) const
{
	Ddo::ptr<Ddo::ConfigurationString> stringPtr = getStringPtr(name, moduleId);
	if(!stringPtr)
	{
		Wt::log("warn") << "StringPtr not found in ConfigurationsCache in GetString(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return stringPtr->value;
}

}

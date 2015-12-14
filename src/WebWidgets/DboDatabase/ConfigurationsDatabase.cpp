#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

namespace WW
{

ConfigurationsDatabase::ConfigurationsDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

void ConfigurationsDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary objects first
	BoolMap boolmap;
	DoubleMap doublemap;
	EnumMap enummap;
	FloatMap floatmap;
	IntMap intmap;
	LongIntMap longintmap;
	StringMap stringmap;
	std::size_t count = 0;

	Dbo::BoolCollection boolCollection;
	Dbo::DoubleCollection doubleCollection;
	Dbo::EnumCollection enumCollection;
	Dbo::FloatCollection floatCollection;
	Dbo::IntCollection intCollection;
	Dbo::LongIntCollection longIntCollection;
	Dbo::StringCollection stringCollection;

	//Fetch em all
	Wt::Dbo::Transaction transaction(dboSession);
	boolCollection = dboSession.find<Dbo::ConfigurationBool>();
	doubleCollection = dboSession.find<Dbo::ConfigurationDouble>();
	enumCollection = dboSession.find<Dbo::ConfigurationEnum>();
	floatCollection = dboSession.find<Dbo::ConfigurationFloat>();
	intCollection = dboSession.find<Dbo::ConfigurationInt>();
	longIntCollection = dboSession.find<Dbo::ConfigurationLongInt>();
	stringCollection = dboSession.find<Dbo::ConfigurationString>();

	//Bool
	for(Dbo::ptr<Dbo::ConfigurationBool> &ptr : boolCollection)
	{
		boolmap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationBool>(new Ddo::ConfigurationBool(ptr));
		_count++;
	}

	//Double
	for(Dbo::ptr<Dbo::ConfigurationDouble> &ptr : doubleCollection)
	{
		doublemap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationDouble>(new Ddo::ConfigurationDouble(ptr));
		_count++;
	}

	//Enum
	for(Dbo::ptr<Dbo::ConfigurationEnum> &ptr : enumCollection)
	{
		enummap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationEnum>(new Ddo::ConfigurationEnum(ptr));
		_count++;
	}

	//Float
	for(Dbo::ptr<Dbo::ConfigurationFloat> &ptr : floatCollection)
	{
		floatmap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationFloat>(new Ddo::ConfigurationFloat(ptr));
		_count++;
	}

	//Int
	for(Dbo::ptr<Dbo::ConfigurationInt> &ptr : intCollection)
	{
		intmap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationInt>(new Ddo::ConfigurationInt(ptr));
		_count++;
	}

	//LongInt
	for(Dbo::ptr<Dbo::ConfigurationLongInt> &ptr : longIntCollection)
	{
		longintmap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationLongInt>(new Ddo::ConfigurationLongInt(ptr));
		_count++;
	}

	//String
	for(Dbo::ptr<Dbo::ConfigurationString> &ptr : stringCollection)
	{
		stringmap[std::make_pair(ptr.id()->modulePtr().id(), ptr.id()->name())] = Ddo::ptr<Ddo::ConfigurationString>(new Ddo::ConfigurationString(ptr));
		_count++;
	}

	transaction.commit();
	_boolMap.swap(boolmap);
	_doubleMap.swap(doublemap);
	_enumMap.swap(enummap);
	_floatMap.swap(floatmap);
	_intMap.swap(intmap);
	_longIntMap.swap(longintmap);
	_stringMap.swap(stringmap);
	std::swap(_count, count);

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = ptEnd - ptStart;

	Wt::log("info") << name() << ": " << _count << " entries successfully loaded in " << _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::ConfigurationBool> ConfigurationsDatabase::getBoolPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	BoolMap::const_iterator itr = _boolMap.find(std::make_pair(moduleId, name));
	if(itr == _boolMap.end())
		return Ddo::cPtr<Ddo::ConfigurationBool>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationDouble> ConfigurationsDatabase::getDoublePtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	DoubleMap::const_iterator itr = _doubleMap.find(std::make_pair(moduleId, name));
	if(itr == _doubleMap.end())
		return Ddo::cPtr<Ddo::ConfigurationDouble>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationEnum> ConfigurationsDatabase::getEnumPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	EnumMap::const_iterator itr = _enumMap.find(std::make_pair(moduleId, name));
	if(itr == _enumMap.end())
		return Ddo::cPtr<Ddo::ConfigurationEnum>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationFloat> ConfigurationsDatabase::getFloatPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	FloatMap::const_iterator itr = _floatMap.find(std::make_pair(moduleId, name));
	if(itr == _floatMap.end())
		return Ddo::cPtr<Ddo::ConfigurationFloat>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationInt> ConfigurationsDatabase::getIntPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	IntMap::const_iterator itr = _intMap.find(std::make_pair(moduleId, name));
	if(itr == _intMap.end())
		return Ddo::cPtr<Ddo::ConfigurationInt>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationLongInt> ConfigurationsDatabase::getLongIntPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	LongIntMap::const_iterator itr = _longIntMap.find(std::make_pair(moduleId, name));
	if(itr == _longIntMap.end())
		return Ddo::cPtr<Ddo::ConfigurationLongInt>();

	return itr->second;
}

Ddo::cPtr<Ddo::ConfigurationString> ConfigurationsDatabase::getStringPtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	StringMap::const_iterator itr = _stringMap.find(std::make_pair(moduleId, name));
	if(itr == _stringMap.end())
		return Ddo::cPtr<Ddo::ConfigurationString>();

	return itr->second;
}

//Boolean getter
bool ConfigurationsDatabase::getBool(const std::string &name, long long moduleId, bool defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationBool> boolPtr = getBoolPtr(name, moduleId);
	if(!boolPtr)
	{
		Wt::log("warn") << "BoolPtr not found in ConfigurationsDatabase in GetBool(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return boolPtr->value;
}

//Double getter
double ConfigurationsDatabase::getDouble(const std::string &name, long long moduleId, double defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationDouble> doublePtr = getDoublePtr(name, moduleId);
	if(!doublePtr)
	{
		Wt::log("warn") << "DoublePtr not found in ConfigurationsDatabase in GetDouble(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return doublePtr->value;
}

//Enum getter
int ConfigurationsDatabase::getEnum(const std::string &name, long long moduleId, int defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationEnum> enumPtr = getEnumPtr(name, moduleId);
	if(!enumPtr)
	{
		Wt::log("warn") << "EnumPtr not found in ConfigurationsDatabase in GetEnum(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return enumPtr->value;
}

//Float getter
float ConfigurationsDatabase::getFloat(const std::string &name, long long moduleId, float defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationFloat> floatPtr = getFloatPtr(name, moduleId);
	if(!floatPtr)
	{
		Wt::log("warn") << "FloatPtr not found in ConfigurationsDatabase in GetFloat(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return floatPtr->value;
}

//Integer getter
int ConfigurationsDatabase::getInt(const std::string &name, long long moduleId, int defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationInt> intPtr = getIntPtr(name, moduleId);
	if(!intPtr)
	{
		Wt::log("warn") << "IntPtr not found in ConfigurationsDatabase in GetInt(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return intPtr->value;
}

//Long integer getter
long long ConfigurationsDatabase::getLongInt(const std::string &name, long long moduleId, long long defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationLongInt> longIntPtr = getLongIntPtr(name, moduleId);
	if(!longIntPtr)
	{
		Wt::log("warn") << "LongIntPtr not found in ConfigurationsDatabase in GetLongInt(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return longIntPtr->value;
}

//String getter
std::string ConfigurationsDatabase::getStr(const std::string &name, long long moduleId, std::string defaultValue) const
{
	Ddo::cPtr<Ddo::ConfigurationString> stringPtr = getStringPtr(name, moduleId);
	if(!stringPtr)
	{
		Wt::log("warn") << "StringPtr not found in ConfigurationsDatabase in GetString(...). Name: " << name << ", ModuleId: " << moduleId << ", Default Value: " << defaultValue;
		return defaultValue;
	}
	return stringPtr->value;
}

long long ConfigurationsDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}

std::size_t ConfigurationsDatabase::configurationsCount() const
{
	ReadLock lock(manager());
	return _count;
}

void ConfigurationsDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	fetchAll(dboSession);
}

}

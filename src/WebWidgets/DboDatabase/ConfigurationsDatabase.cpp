#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

ConfigurationsDatabase::ConfigurationsDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager)
{ }

void ConfigurationsDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	BoolMaps boolmap;
	boolmap.swap(BoolMap);

	DoubleMaps doublemap;
	doublemap.swap(DoubleMap);

	EnumMaps enummap;
	enummap.swap(EnumMap);

	FloatMaps floatmap;
	floatmap.swap(FloatMap);

	IntMaps intmap;
	intmap.swap(IntMap);

	LongIntMaps longintmap;
	longintmap.swap(LongIntMap);

	StringMaps stringmap;
	stringmap.swap(StringMap);

	std::size_t count = 0;
	boost::swap(count, Count);

	//Strong transaction like exception safety
	try
	{
		BoolCollections BoolCollection;
		DoubleCollections DoubleCollection;
		EnumCollections EnumCollection;
		FloatCollections FloatCollection;
		IntCollections IntCollection;
		LongIntCollections LongIntCollection;
		StringCollections StringCollection;

		//Fetch em all
		Wt::Dbo::Transaction transaction(DboSession);
		BoolCollection = DboSession.find<ConfigurationBool>();
		DoubleCollection = DboSession.find<ConfigurationDouble>();
		EnumCollection = DboSession.find<ConfigurationEnum>();
		FloatCollection = DboSession.find<ConfigurationFloat>();
		IntCollection = DboSession.find<ConfigurationInt>();
		LongIntCollection = DboSession.find<ConfigurationLongInt>();
		StringCollection = DboSession.find<ConfigurationString>();

		//Bool
		for(BoolCollections::const_iterator itr = BoolCollection.begin();
			itr != BoolCollection.end();
			++itr, Count++)
		{
			BoolMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationBoolData>(new ConfigurationBoolData(*itr));
		}

		//Double
		for(DoubleCollections::const_iterator itr = DoubleCollection.begin();
			itr != DoubleCollection.end();
			++itr, Count++)
		{
			DoubleMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationDoubleData>(new ConfigurationDoubleData(*itr));
		}

		//Enum
		for(EnumCollections::const_iterator itr = EnumCollection.begin();
			itr != EnumCollection.end();
			++itr, Count++)
		{
			EnumMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationEnumData>(new ConfigurationEnumData(*itr));
		}

		//Float
		for(FloatCollections::const_iterator itr = FloatCollection.begin();
			itr != FloatCollection.end();
			++itr, Count++)
		{
			FloatMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationFloatData>(new ConfigurationFloatData(*itr));;
		}

		//Int
		for(IntCollections::const_iterator itr = IntCollection.begin();
			itr != IntCollection.end();
			++itr, Count++)
		{
			IntMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationIntData>(new ConfigurationIntData(*itr));;
		}

		//LongInt
		for(LongIntCollections::const_iterator itr = LongIntCollection.begin();
			itr != LongIntCollection.end();
			++itr, Count++)
		{
			LongIntMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationLongIntData>(new ConfigurationLongIntData(*itr));
		}

		//String
		for(StringCollections::const_iterator itr = StringCollection.begin();
			itr != StringCollection.end();
			++itr, Count++)
		{
			StringMap[std::make_pair(itr->id()->ModulePtr().id(), itr->id()->Name())] = boost::shared_ptr<ConfigurationStringData>(new ConfigurationStringData(*itr));
		}

		transaction.commit();
	}
	catch(...)
	{
		//If error occurs return back to the previous state
		BoolMap.swap(boolmap);
		DoubleMap.swap(doublemap);
		EnumMap.swap(enummap);
		FloatMap.swap(floatmap);
		IntMap.swap(intmap);
		LongIntMap.swap(longintmap);
		StringMap.swap(stringmap);
		boost::swap(Count, count);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << Count << " entries successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
}

boost::shared_ptr<const ConfigurationBoolData> ConfigurationsDatabase::GetBoolPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	BoolMaps::const_iterator itr = BoolMap.find(std::make_pair(ModuleId, Name));
	if(itr == BoolMap.end())
	{
		return boost::shared_ptr<const ConfigurationBoolData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationDoubleData> ConfigurationsDatabase::GetDoublePtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	DoubleMaps::const_iterator itr = DoubleMap.find(std::make_pair(ModuleId, Name));
	if(itr == DoubleMap.end())
	{
		return boost::shared_ptr<const ConfigurationDoubleData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationEnumData> ConfigurationsDatabase::GetEnumPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	EnumMaps::const_iterator itr = EnumMap.find(std::make_pair(ModuleId, Name));
	if(itr == EnumMap.end())
	{
		return boost::shared_ptr<const ConfigurationEnumData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationFloatData> ConfigurationsDatabase::GetFloatPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	FloatMaps::const_iterator itr = FloatMap.find(std::make_pair(ModuleId, Name));
	if(itr == FloatMap.end())
	{
		return boost::shared_ptr<const ConfigurationFloatData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationIntData> ConfigurationsDatabase::GetIntPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	IntMaps::const_iterator itr = IntMap.find(std::make_pair(ModuleId, Name));
	if(itr == IntMap.end())
	{
		return boost::shared_ptr<const ConfigurationIntData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationLongIntData> ConfigurationsDatabase::GetLongIntPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	LongIntMaps::const_iterator itr = LongIntMap.find(std::make_pair(ModuleId, Name));
	if(itr == LongIntMap.end())
	{
		return boost::shared_ptr<const ConfigurationLongIntData>();
	}
	return itr->second;
}

boost::shared_ptr<const ConfigurationStringData> ConfigurationsDatabase::GetStringPtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	StringMaps::const_iterator itr = StringMap.find(std::make_pair(ModuleId, Name));
	if(itr == StringMap.end())
	{
		return boost::shared_ptr<const ConfigurationStringData>();
	}
	return itr->second;
}

//Boolean getter
bool ConfigurationsDatabase::GetBool(const std::string &Name, long long ModuleId, bool Default) const
{
	boost::shared_ptr<const ConfigurationBoolData> BoolPtr = GetBoolPtr(Name, ModuleId);
	if(!BoolPtr)
	{
		Wt::log("warn") << "BoolPtr not found in ConfigurationsDatabase in GetBool(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return BoolPtr->Value;
}

//Double getter
double ConfigurationsDatabase::GetDouble(const std::string &Name, long long ModuleId, double Default) const
{
	boost::shared_ptr<const ConfigurationDoubleData> DoublePtr = GetDoublePtr(Name, ModuleId);
	if(!DoublePtr)
	{
		Wt::log("warn") << "DoublePtr not found in ConfigurationsDatabase in GetDouble(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return DoublePtr->Value;
}

//Enum getter
int ConfigurationsDatabase::GetEnum(const std::string &Name, long long ModuleId, int Default) const
{
	boost::shared_ptr<const ConfigurationEnumData> EnumPtr = GetEnumPtr(Name, ModuleId);
	if(!EnumPtr)
	{
		Wt::log("warn") << "EnumPtr not found in ConfigurationsDatabase in GetEnum(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return EnumPtr->Value;
}

//Float getter
float ConfigurationsDatabase::GetFloat(const std::string &Name, long long ModuleId, float Default) const
{
	boost::shared_ptr<const ConfigurationFloatData> FloatPtr = GetFloatPtr(Name, ModuleId);
	if(!FloatPtr)
	{
		Wt::log("warn") << "FloatPtr not found in ConfigurationsDatabase in GetFloat(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return FloatPtr->Value;
}

//Integer getter
int ConfigurationsDatabase::GetInt(const std::string &Name, long long ModuleId, int Default) const
{
	boost::shared_ptr<const ConfigurationIntData> IntPtr = GetIntPtr(Name, ModuleId);
	if(!IntPtr)
	{
		Wt::log("warn") << "IntPtr not found in ConfigurationsDatabase in GetInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return IntPtr->Value;
}

//Long integer getter
long long ConfigurationsDatabase::GetLongInt(const std::string &Name, long long ModuleId, long long Default) const
{
	boost::shared_ptr<const ConfigurationLongIntData> LongIntPtr = GetLongIntPtr(Name, ModuleId);
	if(!LongIntPtr)
	{
		Wt::log("warn") << "LongIntPtr not found in ConfigurationsDatabase in GetLongInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return LongIntPtr->Value;
}

//String getter
std::string ConfigurationsDatabase::GetStr(const std::string &Name, long long ModuleId, std::string Default) const
{
	boost::shared_ptr<const ConfigurationStringData> StringPtr = GetStringPtr(Name, ModuleId);
	if(!StringPtr)
	{
		Wt::log("warn") << "StringPtr not found in ConfigurationsDatabase in GetString(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return StringPtr->Value;
}

long long ConfigurationsDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}

std::size_t ConfigurationsDatabase::CountConfigurations() const
{
	ReadLock lock(Manager());
	return Count;
}

void ConfigurationsDatabase::Load(Wt::Dbo::Session &DboSession)
{
	DboSession.mapClass<Author>(Author::TableName());
	DboSession.mapClass<Module>(Module::TableName());
	DboSession.mapClass<Configuration>(Configuration::TableName());
	DboSession.mapClass<ConfigurationBool>(ConfigurationBool::TableName());
	DboSession.mapClass<ConfigurationEnum>(ConfigurationEnum::TableName());
	DboSession.mapClass<ConfigurationEnumValue>(ConfigurationEnumValue::TableName());
	DboSession.mapClass<ConfigurationDouble>(ConfigurationDouble::TableName());
	DboSession.mapClass<ConfigurationFloat>(ConfigurationFloat::TableName());
	DboSession.mapClass<ConfigurationInt>(ConfigurationInt::TableName());
	DboSession.mapClass<ConfigurationLongInt>(ConfigurationLongInt::TableName());
	DboSession.mapClass<ConfigurationString>(ConfigurationString::TableName());
	DboSession.mapClass<Language>(Language::TableName());
	DboSession.mapClass<LanguageSingle>(LanguageSingle::TableName());
	DboSession.mapClass<LanguagePlural>(LanguagePlural::TableName());
	DboSession.mapClass<Page>(Page::TableName());
	DboSession.mapClass<Template>(Template::TableName());
	DboSession.mapClass<Style>(Style::TableName());
	DboSession.mapClass<StyleTemplate>(StyleTemplate::TableName());
	DboSession.mapClass<StyleCssRule>(StyleCssRule::TableName());
	DboSession.mapClass<TemplateCssRule>(TemplateCssRule::TableName());
	DboSession.mapClass<AccessHostName>(AccessHostName::TableName());
	DboSession.mapClass<PageAccessPath>(PageAccessPath::TableName());
	DboSession.mapClass<LanguageAccessPath>(LanguageAccessPath::TableName());

	FetchAll(DboSession);
}

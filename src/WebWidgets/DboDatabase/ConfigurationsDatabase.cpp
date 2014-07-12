#include "DboDatabase/ConfigurationsDatabase.h"
#include "Application/WServer.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

ConfigurationsDatabase::ConfigurationsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: Count(0), _Server(Server)
{
	MapClasses();
	DboSession.setConnectionPool(SQLPool);
}

ConfigurationsDatabase::ConfigurationsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: Count(0), _Server(Server)
{
	MapClasses();
	DboSession.setConnection(SQLConnection);
}

void ConfigurationsDatabase::FetchAll()
{
	WRITE_LOCK;

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
		DboSession.disconnectAll();

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
			BoolMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//Double
		for(DoubleCollections::const_iterator itr = DoubleCollection.begin();
			itr != DoubleCollection.end();
			++itr, Count++)
		{
			DoubleMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//Enum
		for(EnumCollections::const_iterator itr = EnumCollection.begin();
			itr != EnumCollection.end();
			++itr, Count++)
		{
			EnumMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//Float
		for(FloatCollections::const_iterator itr = FloatCollection.begin();
			itr != FloatCollection.end();
			++itr, Count++)
		{
			FloatMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//Int
		for(IntCollections::const_iterator itr = IntCollection.begin();
			itr != IntCollection.end();
			++itr, Count++)
		{
			IntMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//LongInt
		for(LongIntCollections::const_iterator itr = LongIntCollection.begin();
			itr != LongIntCollection.end();
			++itr, Count++)
		{
			LongIntMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
		}

		//String
		for(StringCollections::const_iterator itr = StringCollection.begin();
			itr != StringCollection.end();
			++itr, Count++)
		{
			StringMap[std::make_pair(itr->id().id().ModulePtr.id(), itr->id().id().Name)] = *itr;
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
}

void ConfigurationsDatabase::MapClasses()
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
	DboSession.mapClass<AccessPath>(AccessPath::TableName());
}

Wt::Dbo::ptr<ConfigurationBool> ConfigurationsDatabase::GetBoolPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	BoolMaps::const_iterator itr = BoolMap.find(std::make_pair(ModuleId, Name));
	if(itr == BoolMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationBool>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationDouble> ConfigurationsDatabase::GetDoublePtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	DoubleMaps::const_iterator itr = DoubleMap.find(std::make_pair(ModuleId, Name));
	if(itr == DoubleMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationDouble>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationEnum> ConfigurationsDatabase::GetEnumPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	EnumMaps::const_iterator itr = EnumMap.find(std::make_pair(ModuleId, Name));
	if(itr == EnumMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationEnum>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationFloat> ConfigurationsDatabase::GetFloatPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	FloatMaps::const_iterator itr = FloatMap.find(std::make_pair(ModuleId, Name));
	if(itr == FloatMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationFloat>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationInt> ConfigurationsDatabase::GetIntPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	IntMaps::const_iterator itr = IntMap.find(std::make_pair(ModuleId, Name));
	if(itr == IntMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationInt>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationLongInt> ConfigurationsDatabase::GetLongIntPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	LongIntMaps::const_iterator itr = LongIntMap.find(std::make_pair(ModuleId, Name));
	if(itr == LongIntMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationLongInt>();
	}
	return itr->second;
}

Wt::Dbo::ptr<ConfigurationString> ConfigurationsDatabase::GetStringPtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	StringMaps::const_iterator itr = StringMap.find(std::make_pair(ModuleId, Name));
	if(itr == StringMap.end())
	{
		return Wt::Dbo::ptr<ConfigurationString>();
	}
	return itr->second;
}

//Boolean getter
bool ConfigurationsDatabase::GetBool(const std::string &Name, long long ModuleId, bool Default) const
{
	Wt::Dbo::ptr<ConfigurationBool> BoolPtr = GetBoolPtr(Name, ModuleId);
	if(!BoolPtr)
	{
		_Server.log("warn") << "BoolPtr not found in ConfigurationsDatabase in GetBool(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return BoolPtr->Value;
}

//Double getter
double ConfigurationsDatabase::GetDouble(const std::string &Name, long long ModuleId, double Default) const
{
	Wt::Dbo::ptr<ConfigurationDouble> DoublePtr = GetDoublePtr(Name, ModuleId);
	if(!DoublePtr)
	{
		_Server.log("warn") << "DoublePtr not found in ConfigurationsDatabase in GetDouble(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return DoublePtr->Value;
}

//Enum getter
int ConfigurationsDatabase::GetEnum(const std::string &Name, long long ModuleId, int Default) const
{
	Wt::Dbo::ptr<ConfigurationEnum> EnumPtr = GetEnumPtr(Name, ModuleId);
	if(!EnumPtr)
	{
		_Server.log("warn") << "EnumPtr not found in ConfigurationsDatabase in GetEnum(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return EnumPtr->Value;
}

//Float getter
float ConfigurationsDatabase::GetFloat(const std::string &Name, long long ModuleId, float Default) const
{
	Wt::Dbo::ptr<ConfigurationFloat> FloatPtr = GetFloatPtr(Name, ModuleId);
	if(!FloatPtr)
	{
		_Server.log("warn") << "FloatPtr not found in ConfigurationsDatabase in GetFloat(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return FloatPtr->Value;
}

//Integer getter
int ConfigurationsDatabase::GetInt(const std::string &Name, long long ModuleId, int Default) const
{
	Wt::Dbo::ptr<ConfigurationInt> IntPtr = GetIntPtr(Name, ModuleId);
	if(!IntPtr)
	{
		_Server.log("warn") << "IntPtr not found in ConfigurationsDatabase in GetInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return IntPtr->Value;
}

//Long integer getter
long long ConfigurationsDatabase::GetLongInt(const std::string &Name, long long ModuleId, long long Default) const
{
	Wt::Dbo::ptr<ConfigurationLongInt> LongIntPtr = GetLongIntPtr(Name, ModuleId);
	if(!LongIntPtr)
	{
		_Server.log("warn") << "LongIntPtr not found in ConfigurationsDatabase in GetLongInt(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	return LongIntPtr->Value;
}

//String getter
std::string ConfigurationsDatabase::GetStr(const std::string &Name, long long ModuleId, std::string Default) const
{
	Wt::Dbo::ptr<ConfigurationString> StringPtr = GetStringPtr(Name, ModuleId);
	if(!StringPtr)
	{
		_Server.log("warn") << "StringPtr not found in ConfigurationsDatabase in GetString(...). Name: " << Name << ", ModuleId: " << ModuleId << ", Default Value: " << Default;
		return Default;
	}
	if(!StringPtr->Value.is_initialized())
	{
		return std::string();
	}
	return *StringPtr->Value;
}

long long ConfigurationsDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}

std::size_t ConfigurationsDatabase::CountConfigurations() const
{
	READ_LOCK;
	return Count;
}

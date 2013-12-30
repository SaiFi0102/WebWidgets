#include "DboDatabase/AccessPathsDatabase.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

AccessPathsDatabase::AccessPathsDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnectionPool(SQLPool);
	MapClasses();
}

AccessPathsDatabase::AccessPathsDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnection(SQLConnection);
	MapClasses();
}

void AccessPathsDatabase::MapClasses()
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

void AccessPathsDatabase::FetchAll()
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	AccessPathContainers accesspathcontainer;
	accesspathcontainer.swap(AccessPathContainer);

	//Strong transaction like exception safety
	try
	{
		Wt::Dbo::Transaction transaction(DboSession);
		AccessPathCollections AccessPathCollection = DboSession.find<AccessPath>();
	
		//All AccessPaths
		for(AccessPathCollections::const_iterator itr = AccessPathCollection.begin();
			itr != AccessPathCollection.end();
			++itr)
		{
			AccessPathContainer.insert(*itr);
		}

		transaction.commit();
	}
	catch(...)
	{
		AccessPathContainer.swap(accesspathcontainer);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;
}

Wt::Dbo::ptr<AccessPath> AccessPathsDatabase::GetPtr(long long Id) const
{
	READ_LOCK;
	AccessPathById::const_iterator itr = AccessPathContainer.get<ById>().find(Id);
	if(itr == AccessPathContainer.get<ById>().end())
	{
		return Wt::Dbo::ptr<AccessPath>();
	}
	return *itr;
}
Wt::Dbo::ptr<AccessPath> AccessPathsDatabase::GetPtr(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	AccessPathByURL::const_iterator itr = AccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	if(itr == AccessPathContainer.get<ByURL>().end())
	{
		return Wt::Dbo::ptr<AccessPath>();
	}
	return *itr;
}

AccessPath AccessPathsDatabase::GetDbo(Wt::Dbo::dbo_traits<AccessPath>::IdType Id) const
{
	READ_LOCK;
	Wt::Dbo::ptr<AccessPath> Ptr = GetPtr(Id);
	if(!Ptr)
	{
		return AccessPath();
	}
	return *Ptr;
}
AccessPath AccessPathsDatabase::GetDbo(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	Wt::Dbo::ptr<AccessPath> Ptr = GetPtr(HostName, InternalPath);
	if(!Ptr)
	{
		return AccessPath();
	}
	return *Ptr;
}

bool AccessPathsDatabase::AccessPathExists(long long Id) const
{
	return GetPtr(Id) != 0;
}
bool AccessPathsDatabase::AccessPathExists(const std::string &HostName, const std::string &InternalPath) const
{
	return GetPtr(HostName, InternalPath) != 0;
}

bool AccessPathsDatabase::LanguageAccessPathExists(long long Id) const
{
	Wt::Dbo::ptr<AccessPath> Ptr = GetPtr(Id);
	return Ptr ? Ptr->LanguagePtr != 0 : false;
}
bool AccessPathsDatabase::LanguageAccessPathExists(const std::string &HostName, const std::string &InternalPath) const
{
	Wt::Dbo::ptr<AccessPath> Ptr = GetPtr(HostName, InternalPath);
	return Ptr ? Ptr->LanguagePtr != 0 : false;
}

std::size_t AccessPathsDatabase::CountAccessPaths() const
{
	READ_LOCK;
	return AccessPathContainer.size();
}
long long AccessPathsDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}

std::string AccessPathsDatabase::FirstAccessPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const
{
	READ_LOCK;
	AccessPathByLanguageHostname::const_iterator itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName));
	if(itr == AccessPathContainer.get<ByLanguageHostname>().end() && HostName.substr(0, 4) == "www.")
	{
		itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName.substr(4)));
	}
	if(itr == AccessPathContainer.get<ByLanguageHostname>().end()
		&& (!LanguageFromHostname || _Server.GetConfigurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false)))
	{
		itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, ""));
	}
	if(itr == AccessPathContainer.get<ByLanguageHostname>().end())
	{
		return "/";
	}
	return std::string("/") + (*itr)->InternalPath;
}

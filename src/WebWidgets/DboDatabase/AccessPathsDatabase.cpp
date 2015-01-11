#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ConfigurationsDatabase.h"
#include "DboDatabase/ModulesDatabase.h"
#include "Application/WServer.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::unique_lock<boost::shared_mutex> lock(mutex)

AccessPathsDatabase::AccessPathsDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager)
{ }

void AccessPathsDatabase::Load(Wt::Dbo::Session &DboSession)
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

	FetchAll(DboSession);
}

void AccessPathsDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	AccessPathContainers accesspathcontainer;
	accesspathcontainer.swap(AccessPathContainer);

	//Strong transaction like exception safety
	AccessPathCollections AccessPathCollection;
	try
	{
		DboSession.disconnectAll();
		Wt::Dbo::Transaction transaction(DboSession);
		AccessPathCollection = DboSession.find<AccessPath>().orderBy("CHAR_LENGTH(\"InternalPath\") ASC");
		transaction.commit();
	}
	catch(Wt::Dbo::Exception &)
	{
		try
		{
			Wt::Dbo::Transaction transaction(DboSession);
			AccessPathCollection = DboSession.find<AccessPath>().orderBy("LENGTH(\"InternalPath\") ASC");
			transaction.commit();
		}
		catch(...)
		{
			AccessPathContainer.swap(accesspathcontainer);
			throw;
		}
	}

	try
	{
		//All AccessPaths
		Wt::Dbo::Transaction transaction(DboSession);
		for(AccessPathCollections::const_iterator itr = AccessPathCollection.begin();
			itr != AccessPathCollection.end();
			++itr)
		{
			AccessPathContainer.insert(boost::shared_ptr<AccessPathData>(new AccessPathData(*itr)));
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

	lock.unlock();
	Wt::log("info") << Name() << ": " << CountAccessPaths() << " Access Path entires successfully loaded in "
		<< GetLoadDurationinMS() << " ms";
}

boost::shared_ptr<const AccessPathData> AccessPathsDatabase::GetPtr(long long Id) const
{
	READ_LOCK;
	AccessPathById::const_iterator itr = AccessPathContainer.get<ById>().find(Id);
	if(itr == AccessPathContainer.get<ById>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}
boost::shared_ptr<const AccessPathData> AccessPathsDatabase::GetPtr(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	AccessPathByURL::const_iterator itr = AccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	if(itr == AccessPathContainer.get<ByURL>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}

boost::shared_ptr<const AccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(long long Id) const
{
	READ_LOCK;
	AccessPathById::const_iterator itr = AccessPathContainer.get<ById>().find(Id);
	if(itr == AccessPathContainer.get<ById>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	if((*itr)->LanguageCode.empty())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}
boost::shared_ptr<const AccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	AccessPathByURL::const_iterator itr = AccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	if(itr == AccessPathContainer.get<ByURL>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	if((*itr)->LanguageCode.empty())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}

boost::shared_ptr<const AccessPathData> AccessPathsDatabase::PageAccessPathPtr(long long Id) const
{
	READ_LOCK;
	AccessPathById::const_iterator itr = AccessPathContainer.get<ById>().find(Id);
	if(itr == AccessPathContainer.get<ById>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	if((*itr)->PageId == -1)
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}

boost::shared_ptr<const AccessPathData> AccessPathsDatabase::PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	AccessPathByURL::const_iterator itr = AccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	if (itr == AccessPathContainer.get<ByURL>().end())
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	if((*itr)->PageId == -1)
	{
		return boost::shared_ptr<const AccessPathData>();
	}
	return *itr;
}

bool AccessPathsDatabase::AccessPathExists(long long Id) const
{
	READ_LOCK;
	AccessPathById::const_iterator itr = AccessPathContainer.get<ById>().find(Id);
	return itr != AccessPathContainer.get<ById>().end();
}
bool AccessPathsDatabase::AccessPathExists(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	AccessPathByURL::const_iterator itr = AccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	return itr != AccessPathContainer.get<ByURL>().end();
}

bool AccessPathsDatabase::LanguageAccessPathExists(long long Id) const
{
	boost::shared_ptr<const AccessPathData> Ptr = GetPtr(Id);
	return Ptr ? !Ptr->LanguageCode.empty() : false;
}
bool AccessPathsDatabase::LanguageAccessPathExists(const std::string &HostName, const std::string &InternalPath) const
{
	boost::shared_ptr<const AccessPathData> Ptr = GetPtr(HostName, InternalPath);
	return Ptr ? !Ptr->LanguageCode.empty() : false;
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

std::string AccessPathsDatabase::FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const
{
	READ_LOCK;
	AccessPathByLanguageHostname::const_iterator itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName));
	AccessPathByLanguageHostname::const_iterator enditr = AccessPathContainer.get<ByLanguageHostname>().end();
	if(itr == enditr && HostName.substr(0, 4) == "www.")
	{
		itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName.substr(4)));
	}
	if(itr == enditr
		&& (!LanguageFromHostname || Server()->Configurations()->GetBool("HostUnspecificLanguage", ModulesDatabase::Localization, false)))
	{
		itr = AccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, ""));
	}
	if(itr == enditr)
	{
		return "/";
	}
	return std::string("/") + (*itr)->InternalPath;
}

boost::shared_ptr<const AccessPathData> AccessPathsDatabase::HomePageAccessPathPtr() const
{
	return PageAccessPathPtr(Server()->Configurations()->GetLongInt("HomePageAccessPathId", ModulesDatabase::Navigation, 3));
}

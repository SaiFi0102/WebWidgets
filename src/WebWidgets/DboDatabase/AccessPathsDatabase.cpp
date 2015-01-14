#include "DboDatabase/AccessPathsDatabase.h"
#include <Wt/WLogger>

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
	DboSession.mapClass<AccessHostName>(AccessHostName::TableName());
	DboSession.mapClass<PageAccessPath>(PageAccessPath::TableName());
	DboSession.mapClass<LanguageAccessPath>(LanguageAccessPath::TableName());

	FetchAll(DboSession);
}

void AccessPathsDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	AccessHostNameContainers accesshostnamecontainer;
	accesshostnamecontainer.swap(AccessHostNameContainer);

	PageAccessPathContainers pageaccesspathcontainer;
	pageaccesspathcontainer.swap(PageAccessPathContainer);

	LanguageAccessPathContainers languageaccesspathcontainer;
	languageaccesspathcontainer.swap(LanguageAccessPathContainer);

	//Strong transaction like exception safety
	AccessHostNameCollections AccessHostNameCollection;
	PageAccessPathCollections PageAccessPathCollection;
	LanguageAccessPathCollections LanguageAccessPathCollection;
	try
	{
		Wt::Dbo::Transaction transaction(DboSession);
		LanguageAccessPathCollection = DboSession.find<LanguageAccessPath>().orderBy("CHAR_LENGTH(\"InternalPath\") ASC");
		AccessHostNameCollection = DboSession.find<AccessHostName>();
		PageAccessPathCollection = DboSession.find<PageAccessPath>();
		transaction.commit();
	}
	catch(Wt::Dbo::Exception &)
	{
		try
		{
			Wt::Dbo::Transaction transaction(DboSession);
			LanguageAccessPathCollection = DboSession.find<LanguageAccessPath>().orderBy("LENGTH(\"InternalPath\") ASC");
			AccessHostNameCollection = DboSession.find<AccessHostName>();
			PageAccessPathCollection = DboSession.find<PageAccessPath>();
			transaction.commit();
		}
		catch(...)
		{
			AccessHostNameContainer.swap(accesshostnamecontainer);
			PageAccessPathContainer.swap(pageaccesspathcontainer);
			LanguageAccessPathContainer.swap(languageaccesspathcontainer);
			throw;
		}
	}

	try
	{
		//All AccessPaths
		Wt::Dbo::Transaction transaction(DboSession);

		for(AccessHostNameCollections::const_iterator itr = AccessHostNameCollection.begin();
			itr != AccessHostNameCollection.end();
			++itr)
		{
			AccessHostNameContainer.insert(boost::shared_ptr<AccessHostNameData>(new AccessHostNameData(*itr)));
		}
		for(PageAccessPathCollections::const_iterator itr = PageAccessPathCollection.begin();
			itr != PageAccessPathCollection.end();
			++itr)
		{
			PageAccessPathContainer.insert(boost::shared_ptr<PageAccessPathData>(new PageAccessPathData(*itr)));
		}
		for(LanguageAccessPathCollections::const_iterator itr = LanguageAccessPathCollection.begin();
			itr != LanguageAccessPathCollection.end();
			++itr)
		{
			LanguageAccessPathContainer.insert(boost::shared_ptr<LanguageAccessPathData>(new LanguageAccessPathData(*itr)));
		}

		if(AccessHostNameContainer.find("") == AccessHostNameContainer.end())
		{
			Wt::log("error") << Name() << ": Global AccessHostName was not found. The website may not function properly without it";

			AccessHostNameType::const_iterator pitr = accesshostnamecontainer.find("");
			if(pitr != accesshostnamecontainer.end())
			{
				AccessHostNameContainer.insert(*pitr);
				Wt::log("warn") << Name() << ": Using previously loaded global AccessHostName. The website may not function after restarting unless global AccessHostName is restored";
			}
		}

		transaction.commit();
	}
	catch(...)
	{
		AccessHostNameContainer.swap(accesshostnamecontainer);
		PageAccessPathContainer.swap(pageaccesspathcontainer);
		LanguageAccessPathContainer.swap(languageaccesspathcontainer);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	lock.unlock();
	Wt::log("info") << Name() << ": " << CountAccessHostNames() << " Access Host Names, " << CountPageAccessPaths()
		<< " Page Access Paths and " << CountLanguageAccessPaths() << " Language Access Paths successfully loaded in "
		<< GetLoadDurationinMS() << " ms";
}

boost::shared_ptr<const AccessHostNameData> AccessPathsDatabase::AccessHostNamePtr(const std::string &HostName) const
{
	READ_LOCK;
	AccessHostNameType::const_iterator itr = AccessHostNameContainer.find(HostName);
	if(itr != AccessHostNameContainer.end())
	{
		return *itr;
	}
	if(HostName.substr(0, 4) == "www.")
	{
		itr = AccessHostNameContainer.find(HostName.substr(4));
		if(itr != AccessHostNameContainer.end())
		{
			return *itr;
		}
	}
	std::string::size_type ColonPosition = HostName.find(':');
	if(ColonPosition != std::string::npos)
	{
		itr = AccessHostNameContainer.find(HostName.substr(0, ColonPosition));
		if(itr != AccessHostNameContainer.end())
		{
			return *itr;
		}
	}
	return boost::shared_ptr<const AccessHostNameData>();
}
boost::shared_ptr<const AccessHostNameData> AccessPathsDatabase::AccessHostOrGlobalPtr(const std::string &HostName) const
{
	READ_LOCK;
	AccessHostNameType::const_iterator itr = AccessHostNameContainer.find(HostName);
	if(itr != AccessHostNameContainer.end())
	{
		return *itr;
	}
	if(HostName.substr(0, 4) == "www.")
	{
		itr = AccessHostNameContainer.find(HostName.substr(4));
		if(itr != AccessHostNameContainer.end())
		{
			return *itr;
		}
	}
	std::string::size_type ColonPosition = HostName.find(':');
	if(ColonPosition != std::string::npos)
	{
		itr = AccessHostNameContainer.find(HostName.substr(0, ColonPosition));
		if(itr != AccessHostNameContainer.end())
		{
			return *itr;
		}
	}
	if(!HostName.empty())
	{
		itr = AccessHostNameContainer.find("");
		if(itr != AccessHostNameContainer.end())
		{
			return *itr;
		}
	}
	return boost::shared_ptr<const AccessHostNameData>();
}

boost::shared_ptr<const LanguageAccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(long long Id) const
{
	READ_LOCK;
	LanguageAccessPathById::const_iterator itr = LanguageAccessPathContainer.get<ById>().find(Id);
	if(itr != LanguageAccessPathContainer.get<ById>().end())
	{
		return *itr;
	}
	return boost::shared_ptr<const LanguageAccessPathData>();
}
boost::shared_ptr<const LanguageAccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const
{
	READ_LOCK;
	LanguageAccessPathByURL::const_iterator itr = LanguageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath));
	if(itr != LanguageAccessPathContainer.get<ByURL>().end())
	{
		return *itr;
	}
	if(HostName.substr(0, 4) == "www.")
	{
		itr = LanguageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName.substr(4), InternalPath));
		if(itr != LanguageAccessPathContainer.get<ByURL>().end())
		{
			return *itr;
		}
	}
	std::string::size_type ColonPosition = HostName.find(':');
	if(ColonPosition != std::string::npos)
	{
		itr = LanguageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName.substr(0, ColonPosition), InternalPath));
		if(itr != LanguageAccessPathContainer.get<ByURL>().end())
		{
			return *itr;
		}
	}
	return boost::shared_ptr<const LanguageAccessPathData>();
}

boost::shared_ptr<const PageAccessPathData> AccessPathsDatabase::PageAccessPathPtr(long long Id) const
{
	READ_LOCK;
	PageAccessPathById::const_iterator itr = PageAccessPathContainer.get<ById>().find(Id);
	if(itr != PageAccessPathContainer.get<ById>().end())
	{
		return *itr;
	}
	return boost::shared_ptr<const PageAccessPathData>();
}

boost::shared_ptr<const PageAccessPathData> AccessPathsDatabase::PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath, long long ParentAccessPathId) const
{
	READ_LOCK;
	PageAccessPathByURL::const_iterator itr = PageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName, InternalPath, ParentAccessPathId));
	if(itr != PageAccessPathContainer.get<ByURL>().end())
	{
		return *itr;
	}
	if(HostName.substr(0, 4) == "www.")
	{
		itr = PageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName.substr(4), InternalPath, ParentAccessPathId));
		if(itr != PageAccessPathContainer.get<ByURL>().end())
		{
			return *itr;
		}
	}
	std::string::size_type ColonPosition = HostName.find(':');
	if(ColonPosition != std::string::npos)
	{
		itr = PageAccessPathContainer.get<ByURL>().find(boost::make_tuple(HostName.substr(0, ColonPosition), InternalPath, ParentAccessPathId));
		if(itr != PageAccessPathContainer.get<ByURL>().end())
		{
			return *itr;
		}
	}
	return boost::shared_ptr<const PageAccessPathData>();
}

std::size_t AccessPathsDatabase::CountAccessHostNames() const
{
	READ_LOCK;
	return AccessHostNameContainer.size();
}
std::size_t AccessPathsDatabase::CountPageAccessPaths() const
{
	READ_LOCK;
	return PageAccessPathContainer.size();
}
std::size_t AccessPathsDatabase::CountLanguageAccessPaths() const
{
	READ_LOCK;
	return LanguageAccessPathContainer.size();
}
long long AccessPathsDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}

std::string AccessPathsDatabase::FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const
{
	READ_LOCK;
	LanguageAccessPathByLH::const_iterator itr = LanguageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName));
	LanguageAccessPathByLH::const_iterator enditr = LanguageAccessPathContainer.get<ByLanguageHostname>().end();
	if(itr != enditr)
	{
		return std::string("/") + (*itr)->InternalPath;
	}
	if(HostName.substr(0, 4) == "www.")
	{
		itr = LanguageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName.substr(4)));
		if(itr != enditr)
		{
			return std::string("/") + (*itr)->InternalPath;
		}
	}
	std::string::size_type ColonPosition = HostName.find(':');
	if(ColonPosition != std::string::npos)
	{
		itr = LanguageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, HostName.substr(0, ColonPosition)));
		if(itr != enditr)
		{
			return std::string("/") + (*itr)->InternalPath;
		}
	}
	if(!LanguageFromHostname)
	{
		itr = LanguageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(LanguageCode, ""));
		if(itr != enditr)
		{
			return std::string("/") + (*itr)->InternalPath;
		}
	}
	return "/";
}

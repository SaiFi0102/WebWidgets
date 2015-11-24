#include "DboDatabase/AccessPathsDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

AccessPathsDatabase::AccessPathsDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager)
{ }

void AccessPathsDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}

void AccessPathsDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary objects first
	AccessHostNameContainers accesshostnamecontainer;
	PageAccessPathContainers pageaccesspathcontainer;
	LanguageAccessPathContainers languageaccesspathcontainer;

	AccessHostNameCollections AccessHostNameCollection;
	PageAccessPathCollections PageAccessPathCollection;
	LanguageAccessPathCollections LanguageAccessPathCollection;

	//CHAR_LENGTH is not supported in all database backends
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
		Wt::Dbo::Transaction transaction(DboSession);
		LanguageAccessPathCollection = DboSession.find<LanguageAccessPath>().orderBy("LENGTH(\"InternalPath\") ASC");
		AccessHostNameCollection = DboSession.find<AccessHostName>();
		PageAccessPathCollection = DboSession.find<PageAccessPath>();
		transaction.commit();
	}

	//All AccessPaths/Hostnames
	Wt::Dbo::Transaction transaction(DboSession);

	for(AccessHostNameCollections::const_iterator itr = AccessHostNameCollection.begin();
		itr != AccessHostNameCollection.end();
		++itr)
	{
		accesshostnamecontainer.insert(std::shared_ptr<AccessHostNameData>(new AccessHostNameData(*itr)));
	}
	for(PageAccessPathCollections::const_iterator itr = PageAccessPathCollection.begin();
		itr != PageAccessPathCollection.end();
		++itr)
	{
		pageaccesspathcontainer.insert(std::shared_ptr<PageAccessPathData>(new PageAccessPathData(*itr)));
	}
	for(LanguageAccessPathCollections::const_iterator itr = LanguageAccessPathCollection.begin();
		itr != LanguageAccessPathCollection.end();
		++itr)
	{
		languageaccesspathcontainer.insert(std::shared_ptr<LanguageAccessPathData>(new LanguageAccessPathData(*itr)));
	}

	//If global access hostname not found
	if(accesshostnamecontainer.find("") == accesshostnamecontainer.end())
	{
		Wt::log("error") << Name() << ": Global AccessHostName was not found. The website may not function properly without it";

		AccessHostNameType::const_iterator pitr = AccessHostNameContainer.find("");
		if(pitr != AccessHostNameContainer.end())
		{
			accesshostnamecontainer.insert(*pitr);
			Wt::log("warn") << Name() << ": Using previously loaded global AccessHostName. The website may not function after restarting unless global AccessHostName is restored";
		}
	}

	transaction.commit();
	AccessHostNameContainer.swap(accesshostnamecontainer);
	PageAccessPathContainer.swap(pageaccesspathcontainer);
	LanguageAccessPathContainer.swap(languageaccesspathcontainer);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << AccessHostNameContainer.size() << " Access Host Names, " << PageAccessPathContainer.size()
		<< " Page Access Paths and " << LanguageAccessPathContainer.size() << " Language Access Paths successfully loaded in "
		<< LoadDuration.total_milliseconds() << " ms";
}

std::shared_ptr<const AccessHostNameData> AccessPathsDatabase::AccessHostNamePtr(const std::string &HostName) const
{
	ReadLock lock(Manager());
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
	return std::shared_ptr<const AccessHostNameData>();
}
std::shared_ptr<const AccessHostNameData> AccessPathsDatabase::AccessHostOrGlobalPtr(const std::string &HostName) const
{
	ReadLock lock(Manager());
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
	return std::shared_ptr<const AccessHostNameData>();
}

std::shared_ptr<const LanguageAccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(long long Id) const
{
	ReadLock lock(Manager());
	LanguageAccessPathById::const_iterator itr = LanguageAccessPathContainer.get<ById>().find(Id);
	if(itr != LanguageAccessPathContainer.get<ById>().end())
	{
		return *itr;
	}
	return std::shared_ptr<const LanguageAccessPathData>();
}
std::shared_ptr<const LanguageAccessPathData> AccessPathsDatabase::LanguageAccessPathPtr(const std::string &HostName, const std::string &InternalPath) const
{
	ReadLock lock(Manager());
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
	return std::shared_ptr<const LanguageAccessPathData>();
}

std::shared_ptr<const PageAccessPathData> AccessPathsDatabase::PageAccessPathPtr(long long Id) const
{
	ReadLock lock(Manager());
	PageAccessPathById::const_iterator itr = PageAccessPathContainer.get<ById>().find(Id);
	if(itr != PageAccessPathContainer.get<ById>().end())
	{
		return *itr;
	}
	return std::shared_ptr<const PageAccessPathData>();
}

std::shared_ptr<const PageAccessPathData> AccessPathsDatabase::PageAccessPathPtr(const std::string &HostName, const std::string &InternalPath, long long ParentAccessPathId) const
{
	ReadLock lock(Manager());
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
	return std::shared_ptr<const PageAccessPathData>();
}

std::size_t AccessPathsDatabase::CountAccessHostNames() const
{
	ReadLock lock(Manager());
	return AccessHostNameContainer.size();
}
std::size_t AccessPathsDatabase::CountPageAccessPaths() const
{
	ReadLock lock(Manager());
	return PageAccessPathContainer.size();
}
std::size_t AccessPathsDatabase::CountLanguageAccessPaths() const
{
	ReadLock lock(Manager());
	return LanguageAccessPathContainer.size();
}
long long AccessPathsDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}

std::string AccessPathsDatabase::FirstInternalPath(const std::string &LanguageCode, const std::string &HostName, bool LanguageFromHostname) const
{
	ReadLock lock(Manager());
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

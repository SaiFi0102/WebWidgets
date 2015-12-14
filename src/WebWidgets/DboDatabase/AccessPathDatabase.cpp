#include "DboDatabase/AccessPathDatabase.h"
#include "DboDatabase/ReadLock.h"
#include <Wt/WLogger>

namespace WW
{

AccessPathDatabase::AccessPathDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

void AccessPathDatabase::load(Wt::Dbo::Session &dboSession)
{
	MAP_MODULE_DBO_TREE(dboSession)

	fetchAll(dboSession);
}

void AccessPathDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary objects first
	AccessHostNameContainer accesshostnamecontainer;
	PageAccessPathContainer pageaccesspathcontainer;
	LanguageAccessPathContainer languageaccesspathcontainer;

	Dbo::AccessHostNameCollection accessHostNameCollection;
	Dbo::PageAccessPathCollection pageAccessPathCollection;
	Dbo::LanguageAccessPathCollection languageAccessPathCollection;

	//CHAR_LENGTH is not supported in all database backends
	try
	{
		Wt::Dbo::Transaction transaction(dboSession);
		languageAccessPathCollection = dboSession.find<Dbo::LanguageAccessPath>().orderBy("CHAR_LENGTH(\"InternalPath\") ASC");
		accessHostNameCollection = dboSession.find<Dbo::AccessHostName>();
		pageAccessPathCollection = dboSession.find<Dbo::PageAccessPath>();
		transaction.commit();
	}
	catch(Wt::Dbo::Exception &)
	{
		Wt::Dbo::Transaction transaction(dboSession);
		languageAccessPathCollection = dboSession.find<Dbo::LanguageAccessPath>().orderBy("LENGTH(\"InternalPath\") ASC");
		accessHostNameCollection = dboSession.find<Dbo::AccessHostName>();
		pageAccessPathCollection = dboSession.find<Dbo::PageAccessPath>();
		transaction.commit();
	}

	//All AccessPaths/Hostnames
	Wt::Dbo::Transaction transaction(dboSession);

	for(Dbo::ptr<Dbo::AccessHostName> &ptr : accessHostNameCollection)
	{
		accesshostnamecontainer.insert(Ddo::ptr<Ddo::AccessHostName>(new Ddo::AccessHostName(ptr)));
	}
	for(Dbo::ptr<Dbo::PageAccessPath> &ptr : pageAccessPathCollection)
	{
		pageaccesspathcontainer.insert(Ddo::ptr<Ddo::PageAccessPath>(new Ddo::PageAccessPath(ptr)));
	}
	for(Dbo::ptr<Dbo::LanguageAccessPath> &ptr : languageAccessPathCollection)
	{
		languageaccesspathcontainer.insert(Ddo::ptr<Ddo::LanguageAccessPath>(new Ddo::LanguageAccessPath(ptr)));
	}

	//If global access hostname not found
	if(accesshostnamecontainer.find("") == accesshostnamecontainer.end())
	{
		Wt::log("error") << name() << ": Global AccessHostName was not found. The website may not function properly without it";

		AccessHostNameType::const_iterator pitr = _accessHostNameContainer.find("");
		if(pitr != _accessHostNameContainer.end())
		{
			accesshostnamecontainer.insert(*pitr);
			Wt::log("warn") << name() << ": Using previously loaded global AccessHostName. The website may not function after restarting unless global AccessHostName is restored";
		}
	}

	transaction.commit();
	_accessHostNameContainer.swap(accesshostnamecontainer);
	_pageAccessPathContainer.swap(pageaccesspathcontainer);
	_languageAccessPathContainer.swap(languageaccesspathcontainer);

	//Time at end
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = ptEnd - ptStart;

	Wt::log("info") << name() << ": " << _accessHostNameContainer.size() << " Access Host Names, " << _pageAccessPathContainer.size()
		<< " Page Access Paths and " << _languageAccessPathContainer.size() << " Language Access Paths successfully loaded in "
		<< _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::AccessHostName> AccessPathDatabase::accessHostNamePtr(const std::string &hostName) const
{
	ReadLock lock(manager());
	AccessHostNameType::const_iterator itr = _accessHostNameContainer.find(hostName);
	if(itr != _accessHostNameContainer.end())
		return *itr;

	if(hostName.substr(0, 4) == "www.")
	{
		itr = _accessHostNameContainer.find(hostName.substr(4));
		if(itr != _accessHostNameContainer.end())
		{
			return *itr;
		}
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _accessHostNameContainer.find(hostName.substr(0, colonPosition));
		if(itr != _accessHostNameContainer.end())
			return *itr;
	}
	return Ddo::cPtr<Ddo::AccessHostName>();
}
Ddo::cPtr<Ddo::AccessHostName> AccessPathDatabase::accessHostOrGlobalPtr(const std::string &hostName) const
{
	ReadLock lock(manager());
	AccessHostNameType::const_iterator itr = _accessHostNameContainer.find(hostName);
	if(itr != _accessHostNameContainer.end())
		return *itr;

	if(hostName.substr(0, 4) == "www.")
	{
		itr = _accessHostNameContainer.find(hostName.substr(4));
		if(itr != _accessHostNameContainer.end())
			return *itr;
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _accessHostNameContainer.find(hostName.substr(0, colonPosition));
		if(itr != _accessHostNameContainer.end())
			return *itr;
	}
	if(!hostName.empty())
	{
		itr = _accessHostNameContainer.find("");
		if(itr != _accessHostNameContainer.end())
			return *itr;
	}
	return Ddo::cPtr<Ddo::AccessHostName>();
}

Ddo::cPtr<Ddo::LanguageAccessPath> AccessPathDatabase::languageAccessPathPtr(long long id) const
{
	ReadLock lock(manager());
	LanguageAccessPathById::const_iterator itr = _languageAccessPathContainer.get<ById>().find(id);
	if(itr != _languageAccessPathContainer.get<ById>().end())
		return *itr;

	return Ddo::cPtr<Ddo::LanguageAccessPath>();
}
Ddo::cPtr<Ddo::LanguageAccessPath> AccessPathDatabase::languageAccessPathPtr(const std::string &hostName, const std::string &internalPath) const
{
	ReadLock lock(manager());
	LanguageAccessPathByURL::const_iterator itr = _languageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName, internalPath));
	if(itr != _languageAccessPathContainer.get<ByURL>().end())
		return *itr;

	if(hostName.substr(0, 4) == "www.")
	{
		itr = _languageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName.substr(4), internalPath));
		if(itr != _languageAccessPathContainer.get<ByURL>().end())
			return *itr;
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _languageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName.substr(0, colonPosition), internalPath));
		if(itr != _languageAccessPathContainer.get<ByURL>().end())
			return *itr;
	}
	return Ddo::cPtr<Ddo::LanguageAccessPath>();
}

Ddo::cPtr<Ddo::PageAccessPath> AccessPathDatabase::pageAccessPathPtr(long long id) const
{
	ReadLock lock(manager());
	PageAccessPathById::const_iterator itr = _pageAccessPathContainer.get<ById>().find(id);
	if(itr != _pageAccessPathContainer.get<ById>().end())
		return *itr;

	return Ddo::cPtr<Ddo::PageAccessPath>();
}

Ddo::cPtr<Ddo::PageAccessPath> AccessPathDatabase::pageAccessPathPtr(const std::string &hostName, const std::string &internalPath, long long parentAccessPathId) const
{
	ReadLock lock(manager());
	PageAccessPathByURL::const_iterator itr = _pageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName, internalPath, parentAccessPathId));
	if(itr != _pageAccessPathContainer.get<ByURL>().end())
		return *itr;

	if(hostName.substr(0, 4) == "www.")
	{
		itr = _pageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName.substr(4), internalPath, parentAccessPathId));
		if(itr != _pageAccessPathContainer.get<ByURL>().end())
			return *itr;
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _pageAccessPathContainer.get<ByURL>().find(boost::make_tuple(hostName.substr(0, colonPosition), internalPath, parentAccessPathId));
		if(itr != _pageAccessPathContainer.get<ByURL>().end())
			return *itr;
	}
	return Ddo::cPtr<Ddo::PageAccessPath>();
}

std::size_t AccessPathDatabase::accessHostNamesCount() const
{
	ReadLock lock(manager());
	return _accessHostNameContainer.size();
}
std::size_t AccessPathDatabase::pageAccessPathsCount() const
{
	ReadLock lock(manager());
	return _pageAccessPathContainer.size();
}
std::size_t AccessPathDatabase::languageAccessPathsCount() const
{
	ReadLock lock(manager());
	return _languageAccessPathContainer.size();
}
long long AccessPathDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}

std::string AccessPathDatabase::firstLanguagePath(const std::string &languageCode, const std::string &hostName, bool languageFromHostname) const
{
	ReadLock lock(manager());
	LanguageAccessPathByLH::const_iterator itr = _languageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(languageCode, hostName));
	LanguageAccessPathByLH::const_iterator enditr = _languageAccessPathContainer.get<ByLanguageHostname>().end();
	if(itr != enditr)
		return std::string("/") + (*itr)->internalPath;

	if(hostName.substr(0, 4) == "www.")
	{
		itr = _languageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(languageCode, hostName.substr(4)));
		if(itr != enditr)
			return std::string("/") + (*itr)->internalPath;
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _languageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(languageCode, hostName.substr(0, colonPosition)));
		if(itr != enditr)
			return std::string("/") + (*itr)->internalPath;
	}
	if(!languageFromHostname)
	{
		itr = _languageAccessPathContainer.get<ByLanguageHostname>().find(boost::make_tuple(languageCode, ""));
		if(itr != enditr)
			return std::string("/") + (*itr)->internalPath;
	}
	return "/";
}

std::string AccessPathDatabase::firstPagePath(long long pageId, const std::string &hostName) const
{
	ReadLock lock(manager());
	PageAccessPathByPage::const_iterator itr = _pageAccessPathContainer.get<ByPage>().find(boost::make_tuple(pageId, hostName));
	PageAccessPathByPage::const_iterator enditr = _pageAccessPathContainer.get<ByPage>().end();
	if(itr != enditr)
	{
		return std::string("/") + (*itr)->fullPath;
	}
	if(hostName.substr(0, 4) == "www.")
	{
		itr = _pageAccessPathContainer.get<ByPage>().find(boost::make_tuple(pageId, hostName.substr(4)));
		if(itr != enditr)
		{
			return std::string("/") + (*itr)->fullPath;
		}
	}
	std::string::size_type colonPosition = hostName.find(':');
	if(colonPosition != std::string::npos)
	{
		itr = _pageAccessPathContainer.get<ByPage>().find(boost::make_tuple(pageId, hostName.substr(0, colonPosition)));
		if(itr != enditr)
		{
			return std::string("/") + (*itr)->fullPath;
		}
	}
	itr = _pageAccessPathContainer.get<ByPage>().find(boost::make_tuple(pageId, ""));
	if(itr != enditr)
	{
		return std::string("/") + (*itr)->internalPath;
	}
	return "/";
}

}

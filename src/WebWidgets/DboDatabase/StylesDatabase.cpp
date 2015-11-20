#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"

StylesDatabase::StylesDatabase(DboDatabaseManager *Manager)
: AbstractDboDatabase(Manager), _CountStyleCssRules(0), _CountTemplateCssRules(0)
{ }

void StylesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary first
	StyleContainers stylecontainer;
	TemplateMaps templatemap;
	StyleTemplateContainers styletemplatecontainer;

	Wt::Dbo::Transaction transaction(DboSession);
	StyleCollections StyleCollection = DboSession.find<Style>();
	TemplateCollections TemplateCollection = DboSession.find<Template>();
	StyleTemplateCollections StyleTemplateCollection = DboSession.find<StyleTemplate>();
	StyleCssRuleCollections StyleCssRuleCollection = DboSession.find<StyleCssRule>();
	TemplateCssRuleCollections TemplateCssRuleCollection = DboSession.find<TemplateCssRule>();

	//Styles
	for(StyleCollections::const_iterator itr = StyleCollection.begin();
		itr != StyleCollection.end();
		++itr)
	{
		stylecontainer.insert(boost::shared_ptr<StyleData>(new StyleData(*itr)));
	}

	//Templates
	for(TemplateCollections::const_iterator itr = TemplateCollection.begin();
		itr != TemplateCollection.end();
		++itr)
	{
		templatemap[std::make_pair((*itr)->Name(), (*itr)->ModulePtr().id())] = boost::shared_ptr<TemplateData>(new TemplateData(*itr));
	}

	//StyleTemplates
	for(StyleTemplateCollections::const_iterator itr = StyleTemplateCollection.begin();
		itr != StyleTemplateCollection.end();
		++itr)
	{
		styletemplatecontainer.get<0>().insert(boost::shared_ptr<StyleTemplateData>(new StyleTemplateData(*itr)));
	}

	//StyleCssRules
	for(StyleCssRuleCollections::const_iterator itr = StyleCssRuleCollection.begin();
		itr != StyleCssRuleCollection.end();
		++itr)
	{
		StyleByNameAuthor::const_iterator i = stylecontainer.get<ByNameAuthor>().find(boost::make_tuple((*itr)->StylePtr->Name(), (*itr)->StylePtr->AuthorPtr().id()));
		if(i != stylecontainer.get<ByNameAuthor>().end())
		{
			(*i)->StyleCssRules.insert(boost::shared_ptr<StyleCssRuleData>(new StyleCssRuleData(*itr)));
		}
		else
		{
			Wt::log("warn") << Name() << ": " << "StyleCssRule with Selector(" << (*itr)->Selector << ") unknown StyleName(" << (*itr)->StylePtr->Name() << ") and AuthorId(" << (*itr)->StylePtr->AuthorPtr().id() << "). This StyleCssRule will not be loaded.";
		}
	}

	//TemplateCssRules
	for(TemplateCssRuleCollections::const_iterator itr = TemplateCssRuleCollection.begin();
		itr != TemplateCssRuleCollection.end();
		++itr)
	{
		//TemplateCssRuleMap[std::make_pair((*itr)->TemplatePtr->Name(), (*itr)->TemplatePtr->ModulePtr().id())].insert(boost::shared_ptr<TemplateCssRuleData>(new TemplateCssRuleData(*itr)));
		TemplateMaps::const_iterator i = templatemap.find(std::make_pair((*itr)->TemplatePtr->Name(), (*itr)->TemplatePtr->ModulePtr().id()));
		if(i != templatemap.end())
		{
			i->second->TemplateCssRules.insert(boost::shared_ptr<TemplateCssRuleData>(new TemplateCssRuleData(*itr)));
		}
		else
		{
			Wt::log("warn") << Name() << ": " << "TemplateCssRule with Selector(" << (*itr)->Selector << ") unknown TemplateName(" << (*itr)->TemplatePtr->Name() << ") and ModuleId(" << (*itr)->TemplatePtr->ModulePtr().id() << "). This TemplateCssRule will not be loaded.";
		}
	}

	transaction.commit();
	StyleContainer.swap(stylecontainer);
	TemplateMap.swap(templatemap);
	StyleTemplateContainer.swap(styletemplatecontainer);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << StyleContainer.size() << " Styles, " << TemplateMap.size() << " Templates, "
		<< StyleTemplateContainer.size() << " Styled Templates, " << _CountStyleCssRules << " Style CSS Rules and "
		<< _CountTemplateCssRules << " Template CSS Rules successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
}

boost::shared_ptr<const StyleData> StylesDatabase::GetStylePtr(long long StyleId) const
{
	ReadLock lock(Manager());
	StyleById::const_iterator itr = StyleContainer.get<ById>().find(StyleId);
	if(itr == StyleContainer.get<ById>().end())
	{
		return boost::shared_ptr<const StyleData>();
	}
	return *itr;
}
boost::shared_ptr<const StyleData> StylesDatabase::GetStylePtr(const std::string &Name, long long AuthorId) const
{
	ReadLock lock(Manager());
	StyleByNameAuthor::const_iterator itr = StyleContainer.get<ByNameAuthor>().find(boost::make_tuple(Name, AuthorId));
	if(itr == StyleContainer.get<ByNameAuthor>().end())
	{
		return boost::shared_ptr<const StyleData>();
	}
	return *itr;
}

boost::shared_ptr<const TemplateData> StylesDatabase::GetTemplatePtr(const std::string &Name, long long ModuleId) const
{
	ReadLock lock(Manager());
	TemplateMaps::const_iterator itr = TemplateMap.find(std::make_pair(Name, ModuleId));
	if(itr == TemplateMap.end())
	{
		return boost::shared_ptr<const TemplateData>();
	}
	return itr->second;
}


boost::shared_ptr<const StyleTemplateData> StylesDatabase::GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const
{
	ReadLock lock(Manager());
	StyleTemplateType::const_iterator itr = StyleTemplateContainer.find(boost::make_tuple(TemplateName, ModuleId, StyleName, StyleAuthorId));
	if(itr == StyleTemplateContainer.end())
	{
		return boost::shared_ptr<const StyleTemplateData>();
	}
	return *itr;
}

bool StylesDatabase::GetTemplateStr(const std::string &Name, long long ModuleId, std::string &result) const
{
	boost::shared_ptr<const TemplateData> TemplatePtr = GetTemplatePtr(Name, ModuleId);
	if(!TemplatePtr)
	{
		Wt::log("warn") << "TemplatePtr not found in StylesDatabase in GetTemplateStr(...). Name: " << Name << ", ModuleId: " << ModuleId;
		return false;
	}
	result = TemplatePtr->TemplateStr;
	return true;
}

bool StylesDatabase::GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId, std::string &result) const
{
	boost::shared_ptr<const StyleTemplateData> StyleTemplatePtr = GetStyleTemplatePtr(TemplateName, ModuleId, StyleName, StyleAuthorId);
	if(!StyleTemplatePtr)
	{
		return false;
	}
	result = StyleTemplatePtr->TemplateStr;
	return true;
}

boost::shared_ptr<const StyleData> StylesDatabase::FirstStylePtr() const
{
	ReadLock lock(Manager());
	StyleById::const_iterator itr = StyleContainer.get<ById>().begin();
	if(itr == StyleContainer.get<ById>().end())
	{
		return boost::shared_ptr<const StyleData>();
	}
	return *itr;
}

long long StylesDatabase::GetLoadDurationinMS() const
{
	ReadLock lock(Manager());
	return LoadDuration.total_milliseconds();
}
std::size_t StylesDatabase::CountStyles() const
{
	ReadLock lock(Manager());
	return StyleContainer.size();
}
std::size_t StylesDatabase::CountTemplates() const
{
	ReadLock lock(Manager());
	return TemplateMap.size();
}
std::size_t StylesDatabase::CountStyleTemplates() const
{
	ReadLock lock(Manager());
	return StyleTemplateContainer.size();
}
std::size_t StylesDatabase::CountStyleCssRules() const
{
	ReadLock lock(Manager());
	return _CountStyleCssRules;
}
std::size_t StylesDatabase::CountTemplateCssRules() const
{
	ReadLock lock(Manager());
	return _CountTemplateCssRules;
}

void StylesDatabase::Load(Wt::Dbo::Session &DboSession)
{
	MAPDBOCASSES(DboSession)

	FetchAll(DboSession);
}
void StylesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
}
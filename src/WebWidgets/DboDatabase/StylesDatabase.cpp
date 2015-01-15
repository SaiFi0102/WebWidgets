#include "DboDatabase/StylesDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"

StylesDatabase::StylesDatabase(DboDatabaseManager *Manager)
	: AbstractDboDatabase(Manager)
{ }

void StylesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	StyleContainers stylecontainer;
	stylecontainer.swap(StyleContainer);

	TemplateMaps templatemap;
	templatemap.swap(TemplateMap);

	StyleTemplateContainers styletemplatecontainer;
	styletemplatecontainer.swap(StyleTemplateContainer);

	StyleCssRuleMaps stylecssrulemap;
	stylecssrulemap.swap(StyleCssRuleMap);

	TemplateCssRuleMaps templatecssrulemap;
	templatecssrulemap.swap(TemplateCssRuleMap);

	//Strong transaction like exception safety
	try
	{
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
			StyleContainer.insert(boost::shared_ptr<StyleData>(new StyleData(*itr)));
		}

		//Templates
		for(TemplateCollections::const_iterator itr = TemplateCollection.begin();
			itr != TemplateCollection.end();
			++itr)
		{
			TemplateMap[std::make_pair((*itr)->Name(), (*itr)->ModulePtr().id())] = boost::shared_ptr<TemplateData>(new TemplateData(*itr));
		}

		//StyleTemplates
		for(StyleTemplateCollections::const_iterator itr = StyleTemplateCollection.begin();
			itr != StyleTemplateCollection.end();
			++itr)
		{
			StyleTemplateContainer.get<0>().insert(boost::shared_ptr<StyleTemplateData>(new StyleTemplateData(*itr)));
		}

		//StyleCssRules
		for(StyleCssRuleCollections::const_iterator itr = StyleCssRuleCollection.begin();
			itr != StyleCssRuleCollection.end();
			++itr)
		{
			StyleCssRuleMap[std::make_pair((*itr)->StylePtr->Name(), (*itr)->StylePtr->AuthorPtr().id())].insert(boost::shared_ptr<StyleCssRuleData>(new StyleCssRuleData(*itr)));
		}

		//TemplateCssRules
		for(TemplateCssRuleCollections::const_iterator itr = TemplateCssRuleCollection.begin();
			itr != TemplateCssRuleCollection.end();
			++itr)
		{
			TemplateCssRuleMap[std::make_pair((*itr)->TemplatePtr->Name(), (*itr)->TemplatePtr->ModulePtr().id())].insert(boost::shared_ptr<TemplateCssRuleData>(new TemplateCssRuleData(*itr)));
		}

		transaction.commit();
	}
	catch(...)
	{
		StyleContainer.swap(stylecontainer);
		TemplateMap.swap(templatemap);
		StyleTemplateContainer.swap(styletemplatecontainer);
		StyleCssRuleMap.swap(stylecssrulemap);
		TemplateCssRuleMap.swap(templatecssrulemap);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	Wt::log("info") << Name() << ": " << StyleContainer.size() << " Styles, " << TemplateMap.size() << " Templates, "
		<< StyleTemplateContainer.size() << " Styled Templates, " << StyleCssRuleMap.size() << " Style CSS Rules and "
		<< TemplateCssRuleMap.size() << " Template CSS Rules successfully loaded in " << LoadDuration.total_milliseconds() << " ms";
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
		//Wt::log("warn") << "StyleTemplatePtr not found in StylesDatabase in GetStyleTemplateStr(...). TemplateName: " << TemplateName << ", ModuleId: " << ModuleId << ", StyleName: " << StyleName << ", StyleAuthorId: " << StyleAuthorId;
		return false;
	}
	result = StyleTemplatePtr->TemplateStr;
	return true;
}

StylesDatabase::StyleCssRuleList StylesDatabase::GetStyleCssRules(const std::string &StyleName, long long AuthorId)
{
	ReadLock lock(Manager());
	StyleCssRuleMaps::const_iterator itr = StyleCssRuleMap.find(std::make_pair(StyleName, AuthorId));
	if(itr == StyleCssRuleMap.end())
	{
		return StyleCssRuleList();
	}
	return itr->second;
}

StylesDatabase::TemplateCssRuleList StylesDatabase::GetTemplateCssRules(const std::string &TemplateName, long long ModuleId)
{
	ReadLock lock(Manager());
	TemplateCssRuleMaps::const_iterator itr = TemplateCssRuleMap.find(std::make_pair(TemplateName, ModuleId));
	if(itr == TemplateCssRuleMap.end())
	{
		return TemplateCssRuleList();
	}
	return itr->second;
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
	return StyleCssRuleMap.size();
}
std::size_t StylesDatabase::CountTemplateCssRules() const
{
	ReadLock lock(Manager());
	return TemplateCssRuleMap.size();
}

void StylesDatabase::Load(Wt::Dbo::Session &DboSession)
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
void StylesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
}
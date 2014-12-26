#include "DboDatabase/StylesDatabase.h"
#include "Dbo/Style.h"
#include "Application/WServer.h"
#include <Wt/WString>
#include "Application/Application.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::unique_lock<boost::shared_mutex> lock(mutex)

StylesDatabase::StylesDatabase(DboDatabaseManager *Manager)
	: AbstractDboDatabase(Manager)
{ }

void StylesDatabase::FetchAll(Wt::Dbo::Session &DboSession)
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	StyleMaps stylemap;
	stylemap.swap(StyleMap);

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
			StyleMap[std::make_pair((*itr)->Name(), (*itr)->AuthorPtr().id())] = boost::shared_ptr<StyleData>(new StyleData(*itr));
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
		StyleMap.swap(stylemap);
		TemplateMap.swap(templatemap);
		StyleTemplateContainer.swap(styletemplatecontainer);
		StyleCssRuleMap.swap(stylecssrulemap);
		TemplateCssRuleMap.swap(templatecssrulemap);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;

	lock.unlock();
	Wt::log("info") << Name() << ": " << CountStyles() << " Styles, " << CountTemplates() << " Templates, "
		<< CountStyleTemplates() << " Styled Templates, " << CountStyleCssRules() << " Style CSS Rules and "
		<< CountTemplateCssRules() << " Template CSS Rules successfully loaded in " << GetLoadDurationinMS() << " ms";
}

boost::shared_ptr<StyleData> StylesDatabase::GetStylePtr(const std::string &Name, long long AuthorId) const
{
	READ_LOCK;
	StyleMaps::const_iterator itr = StyleMap.find(std::make_pair(Name, AuthorId));
	if(itr == StyleMap.end())
	{
		return boost::shared_ptr<StyleData>();
	}
	return itr->second;
}


boost::shared_ptr<TemplateData> StylesDatabase::GetTemplatePtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	TemplateMaps::const_iterator itr = TemplateMap.find(std::make_pair(Name, ModuleId));
	if(itr == TemplateMap.end())
	{
		return boost::shared_ptr<TemplateData>();
	}
	return itr->second;
}


boost::shared_ptr<StyleTemplateData> StylesDatabase::GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const
{
	READ_LOCK;
	StyleTemplateContainers::nth_index<0>::type::const_iterator itr = StyleTemplateContainer.get<0>().find(boost::make_tuple(TemplateName, ModuleId, StyleName, StyleAuthorId));
	if(itr == StyleTemplateContainer.get<0>().end())
	{
		return boost::shared_ptr<StyleTemplateData>();
	}
	return *itr;
}

bool StylesDatabase::GetTemplateStr(const std::string &Name, long long ModuleId, std::string &result) const
{
	boost::shared_ptr<TemplateData> TemplatePtr = GetTemplatePtr(Name, ModuleId);
	if(!TemplatePtr)
	{
		Wt::log("warn") << "TemplatePtr not found in StylesDatabase in GetTemplateStr(...). Name: " << Name << ", ModuleId: " << ModuleId;
		return false;
	}
	if(!TemplatePtr->TemplateStr.is_initialized())
	{
		result = "";
		return true;
	}
	result = *TemplatePtr->TemplateStr;
	return true;
}

bool StylesDatabase::GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId, std::string &result) const
{
	boost::shared_ptr<StyleTemplateData> StyleTemplatePtr = GetStyleTemplatePtr(TemplateName, ModuleId, StyleName, StyleAuthorId);
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
	READ_LOCK;
	StyleCssRuleMaps::const_iterator itr = StyleCssRuleMap.find(std::make_pair(StyleName, AuthorId));
	if(itr == StyleCssRuleMap.end())
	{
		return StyleCssRuleList();
	}
	return itr->second;
}

StylesDatabase::TemplateCssRuleList StylesDatabase::GetTemplateCssRules(const std::string &TemplateName, long long ModuleId)
{
	READ_LOCK;
	TemplateCssRuleMaps::const_iterator itr = TemplateCssRuleMap.find(std::make_pair(TemplateName, ModuleId));
	if(itr == TemplateCssRuleMap.end())
	{
		return TemplateCssRuleList();
	}
	return itr->second;
}

// std::string StylesDatabase::GetStyleSheetFileName()
// {
// 	//READ_LOCK;
// 	return "style.css";
// }
// 
// std::string StylesDatabase::GetStyleSheetFolder(const std::string &StyleName, long long StyleAuthorId)
// {
// 	return StyleName + "_" + boost::lexical_cast<std::string>(StyleAuthorId) + "/";
// }
// 
// std::string StylesDatabase::GetTemplateStyleSheetFileName(const std::string &TemplateName, long long ModuleId)
// {
// 	//READ_LOCK;
// 	return "template_" + TemplateName + "_" + boost::lexical_cast<std::string>(ModuleId) + ".css";
// }

// void StylesDatabase::CreateCssStyleSheets()
// {
// 	Wt::WApplication *app = Wt::WApplication::instance();
// 
// 	//Style CssStyleSheets
// 	for(StyleCssRuleMaps::const_iterator itr = StyleCssRuleMap.begin();
// 		itr != StyleCssRuleMap.end();
// 		++itr)
// 	{
// 		if(itr->second.empty())
// 		{
// 			continue;
// 		}
// 
// 		//Create and open folder and file
// 		boost::filesystem::path FileName;
// 		if(app)
// 		{
// 			FileName = app->docRoot();
// 		}
// 		FileName /= _Server.GetConfigurations()->GetStr("StylesURL", ModulesDatabase::Styles, "/styles") + "/" + StylesDatabase::GetStyleSheetFolder(itr->first.first, itr->first.second) + StylesDatabase::GetStyleSheetFileName();
// 		if(!FileName.is_absolute()) FileName = boost::filesystem::absolute(FileName);
// 
// 		boost::filesystem::create_directories(FileName);
// 		std::ofstream CssFile(FileName.string(), std::ios::trunc);
// 
// 		//Write to file
// 		for(StyleCssRuleVector::const_iterator i = itr->second.begin();
// 			i != itr->second.end();
// 			++i)
// 		{
// 			CssFile << (*i)->Selector << " { "
// 				<< (*i)->Declarations << " }" << std::endl;
// 		}
// 		CssFile.close();
// 	}
// 	
// 	//Template CssStyleSheets
// 	for(TemplateCssRuleMaps::const_iterator itr = TemplateCssRuleMap.begin();
// 		itr != TemplateCssRuleMap.end();
// 		++itr)
// 	{
// 		if(itr->second.empty())
// 		{
// 			continue;
// 		}
// 
// 		//Create and open folder and file
// 		boost::filesystem::path FileName;
// 		if(app)
// 		{
// 			FileName = app->docRoot();
// 		}
// 		FileName /= _Server.GetConfigurations()->GetStr("StylesURL", ModulesDatabase::Styles, "/styles") + "/" + StylesDatabase::GetTemplateStyleSheetFileName(itr->first.first, itr->first.second);
// 		if(!FileName.is_absolute()) FileName = boost::filesystem::absolute(FileName);
// 
// 		boost::filesystem::create_directories(FileName);
// 		std::ofstream CssFile(FileName.string(), std::ios::trunc);
// 
// 		//Write to file
// 		for(TemplateCssRuleVector::const_iterator i = itr->second.begin();
// 			i != itr->second.end();
// 			++i)
// 		{
// 			CssFile << (*i)->Selector << " { "
// 				<< (*i)->Declarations << " }" << std::endl;
// 		}
// 		CssFile.close();
// 	}
// }

long long StylesDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}
std::size_t StylesDatabase::CountStyles() const
{
	READ_LOCK;
	return StyleMap.size();
}
std::size_t StylesDatabase::CountTemplates() const
{
	READ_LOCK;
	return TemplateMap.size();
}
std::size_t StylesDatabase::CountStyleTemplates() const
{
	READ_LOCK;
	return StyleTemplateContainer.size();
}
std::size_t StylesDatabase::CountStyleCssRules() const
{
	READ_LOCK;
	return StyleCssRuleMap.size();
}
std::size_t StylesDatabase::CountTemplateCssRules() const
{
	READ_LOCK;
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
	DboSession.mapClass<AccessPath>(AccessPath::TableName());

	FetchAll(DboSession);
}
void StylesDatabase::Reload(Wt::Dbo::Session &DboSession)
{
	FetchAll(DboSession);
	Server()->RefreshStyleStrings();
}
#include "DboDatabase/StylesDatabase.h"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

StylesDatabase::StylesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnectionPool(SQLPool);
	MapClasses();
}
StylesDatabase::StylesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server)
	: _Server(Server)
{
	DboSession.setConnection(SQLConnection);
	MapClasses();
}

void StylesDatabase::MapClasses()
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
}

void StylesDatabase::FetchAll(bool FetchCss)
{
	WRITE_LOCK;

	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Copy into temporary objects and reset the original
	StyleMaps stylemap;
	stylemap.swap(StyleMap);

	TemplateMaps templatemap;
	templatemap.swap(TemplateMap);

	StyleTemplateMaps styletemplatemap;
	styletemplatemap.swap(StyleTemplateMap);

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
			StyleMap[std::make_pair(itr->id().Name, itr->id().AuthorPtr.id())] = *itr;
		}

		//Templates
		for(TemplateCollections::const_iterator itr = TemplateCollection.begin();
			itr != TemplateCollection.end();
			++itr)
		{
			TemplateMap[std::make_pair(itr->id().Name, itr->id().ModulePtr.id())] = *itr;
		}

		//StyleTemplates
		for(StyleTemplateCollections::const_iterator itr = StyleTemplateCollection.begin();
			itr != StyleTemplateCollection.end();
			++itr)
		{
			StyleTemplateMap[boost::make_tuple(itr->id().DerivingTemplatePtr->id().Name,
				itr->id().DerivingTemplatePtr->id().ModulePtr.id(),
				itr->id().StylePtr->id().Name,
				itr->id().StylePtr->id().AuthorPtr.id())] = *itr;
		}

		if(FetchCss)
		{
			//StyleCssRules
			for(StyleCssRuleCollections::const_iterator itr = StyleCssRuleCollection.begin();
				itr != StyleCssRuleCollection.end();
				++itr)
			{
				StyleCssRuleMap[std::make_pair((*itr)->StylePtr->id().Name, (*itr)->StylePtr->id().AuthorPtr->id())].push_back(*itr);
			}

			//TemplateCssRules
			for(TemplateCssRuleCollections::const_iterator itr = TemplateCssRuleCollection.begin();
				itr != TemplateCssRuleCollection.end();
				++itr)
			{
				TemplateCssRuleMap[std::make_pair((*itr)->TemplatePtr->id().Name, (*itr)->TemplatePtr->id().ModulePtr->id())].push_back(*itr);
			}
		}
		else
		{
			StyleCssRuleMap.clear();
			TemplateCssRuleMap.clear();
		}

		transaction.commit();
	}
	catch(...)
	{
		StyleMap.swap(stylemap);
		TemplateMap.swap(templatemap);
		StyleTemplateMap.swap(styletemplatemap);
		StyleCssRuleMap.swap(stylecssrulemap);
		TemplateCssRuleMap.swap(templatecssrulemap);
		throw;
	}

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	LoadDuration = PTEnd - PTStart;
}

Style StylesDatabase::GetStyleDbo(const std::string &Name, long long AuthorId) const
{
	READ_LOCK;
	Wt::Dbo::ptr<Style> Ptr = GetStylePtr(Name, AuthorId);
	if(!Ptr)
	{
		return Style();
	}
	return *Ptr;
}
Wt::Dbo::ptr<Style> StylesDatabase::GetStylePtr(const std::string &Name, long long AuthorId) const
{
	READ_LOCK;
	StyleMaps::const_iterator itr = StyleMap.find(std::make_pair(Name, AuthorId));
	if(itr == StyleMap.end())
	{
		return Wt::Dbo::ptr<Style>();
	}
	return itr->second;
}

Template StylesDatabase::GetTemplateDbo(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	Wt::Dbo::ptr<Template> Ptr = GetTemplatePtr(Name, ModuleId);
	if(!Ptr)
	{
		return Template();
	}
	return *Ptr;
}
Wt::Dbo::ptr<Template> StylesDatabase::GetTemplatePtr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;
	TemplateMaps::const_iterator itr = TemplateMap.find(std::make_pair(Name, ModuleId));
	if(itr == TemplateMap.end())
	{
		return Wt::Dbo::ptr<Template>();
	}
	return itr->second;
}

StyleTemplate StylesDatabase::GetStyleTemplateDbo(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const
{
	READ_LOCK;
	Wt::Dbo::ptr<StyleTemplate> Ptr = GetStyleTemplatePtr(TemplateName, ModuleId, StyleName, StyleAuthorId);
	if(!Ptr)
	{
		return StyleTemplate();
	}
	return *Ptr;
}
Wt::Dbo::ptr<StyleTemplate> StylesDatabase::GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const
{
	READ_LOCK;
	StyleTemplateMaps::const_iterator itr = StyleTemplateMap.find(boost::make_tuple(TemplateName, ModuleId, StyleName, StyleAuthorId));
	if(itr == StyleTemplateMap.end())
	{
		return Wt::Dbo::ptr<StyleTemplate>();
	}
	return itr->second;
}

const StylesDatabase::TemplateCssRuleVector &StylesDatabase::GetTemplateCssRulePtrs(const std::string &TemplateName, long long ModuleId) const
{
	READ_LOCK;

	TemplateCssRuleMaps::const_iterator itr = TemplateCssRuleMap.find(std::make_pair(TemplateName, ModuleId));
	if(itr == TemplateCssRuleMap.end())
	{
		return TemplateCssRuleVector();
	}
	return itr->second;
}

std::string StylesDatabase::GetTemplateStr(const std::string &Name, long long ModuleId) const
{
	READ_LOCK;

	Wt::Dbo::ptr<Template> TemplatePtr = GetTemplatePtr(Name, ModuleId);
	if(!TemplatePtr)
	{
		return std::string();
	}
	if(!TemplatePtr->TemplateStr.is_initialized())
	{
		return std::string();
	}
	return *TemplatePtr->TemplateStr;
}

std::string StylesDatabase::GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const
{
	READ_LOCK;

	Wt::Dbo::ptr<StyleTemplate> StyleTemplatePtr = GetStyleTemplatePtr(TemplateName, ModuleId, StyleName, StyleAuthorId);
	if(!StyleTemplatePtr)
	{
		return std::string();
	}
	return StyleTemplatePtr->TemplateStr;
}

std::string StylesDatabase::GetStyleSheetFileName(const std::string &StyleName, long long StyleAuthorId) const
{
	//READ_LOCK;
	return "style_" + StyleName + "_" + boost::lexical_cast<std::string>(StyleAuthorId) + ".css";
}

std::string StylesDatabase::GetTemplateStyleSheetFileName(const std::string &TemplateName, long long ModuleId) const
{
	//READ_LOCK;
	return "template_" + TemplateName + "_" + boost::lexical_cast<std::string>(ModuleId) + ".css";
}

void StylesDatabase::CreateCssStyleSheets()
{
	//Style CssStyleSheet
	if(!StyleCssRuleMap.empty())
	{
		for(StyleCssRuleMaps::const_iterator itr = StyleCssRuleMap.begin();
			itr != StyleCssRuleMap.end();
			++itr)
		{
			if(itr->second.empty())
			{
				continue;
			}
			
		}
		
	}
	
	//Template CssStyleSheet
	if(!TemplateCssRuleMap.empty())
	{
}

long long StylesDatabase::GetLoadDurationinMS() const
{
	READ_LOCK;
	return LoadDuration.total_milliseconds();
}
long long StylesDatabase::CountStyles() const
{
	READ_LOCK;
	return StyleMap.size();
}
long long StylesDatabase::CountTemplates() const
{
	READ_LOCK;
	return TemplateMap.size();
}
long long StylesDatabase::CountStyleTemplates() const
{
	READ_LOCK;
	return StyleTemplateMap.size();
}
long long StylesDatabase::CountStyleCssRules() const
{
	READ_LOCK;
	return StyleCssRuleMap.size();
}
long long StylesDatabase::CountTemplateCssRules() const
{
	READ_LOCK;
	return TemplateCssRuleMap.size();
}

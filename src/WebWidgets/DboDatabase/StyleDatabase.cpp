#include "DboDatabase/StyleDatabase.h"
#include "DboDatabase/ReadLock.h"
#include "Application/WServer.h"

namespace WW
{

StyleDatabase::StyleDatabase(DboDatabaseManager *manager)
: AbstractDboDatabase(manager)
{ }

void StyleDatabase::fetchAll(Wt::Dbo::Session &dboSession)
{
	//Time at start
	boost::posix_time::ptime PTStart = boost::posix_time::microsec_clock::local_time();

	//Insert into temporary first
	StyleContainer stylecontainer;
	TemplateMap templatemap;
	StyleTemplateContainer styletemplatecontainer;

	Wt::Dbo::Transaction transaction(dboSession);
	Dbo::StyleCollection styleCollection = dboSession.find<Dbo::Style>();
	Dbo::TemplateCollection templateCollection = dboSession.find<Dbo::Template>();
	Dbo::StyleTemplateCollection styleTemplateCollection = dboSession.find<Dbo::StyleTemplate>();
	Dbo::StyleCssRuleCollection styleCssRuleCollection = dboSession.find<Dbo::StyleCssRule>();
	Dbo::TemplateCssRuleCollection templateCssRuleCollection = dboSession.find<Dbo::TemplateCssRule>();

	//Styles
	for(Dbo::ptr<Dbo::Style> &ptr : styleCollection)
	{
		stylecontainer.insert(Ddo::ptr<Ddo::Style>(new Ddo::Style(ptr)));
	}

	//Templates
	for(Dbo::ptr<Dbo::Template> &ptr : templateCollection)
	{
		templatemap[std::make_pair(ptr->name(), ptr->modulePtr().id())] = Ddo::ptr<Ddo::Template>(new Ddo::Template(ptr));
	}

	//StyleTemplates
	for(Dbo::ptr<Dbo::StyleTemplate> &ptr : styleTemplateCollection)
	{
		styletemplatecontainer.get<0>().insert(Ddo::ptr<Ddo::StyleTemplate>(new Ddo::StyleTemplate(ptr)));
	}

	//StyleCssRules
	for(Dbo::ptr<Dbo::StyleCssRule> &ptr : styleCssRuleCollection)
	{
		StyleByNameAuthor::const_iterator i = stylecontainer.get<ByNameAuthor>().find(boost::make_tuple(ptr->stylePtr->name(), ptr->stylePtr->authorPtr().id()));
		if(i != stylecontainer.get<ByNameAuthor>().end())
			(*i)->styleCssRules.insert(Ddo::ptr<Ddo::StyleCssRule>(new Ddo::StyleCssRule(ptr)));
		else
		{
			Wt::log("warn") << name() << ": " << "StyleCssRule with Selector(" << ptr->selector << ") unknown styleName("
				<< ptr->stylePtr->name() << ") and authorId(" << ptr->stylePtr->authorPtr().id() << "). This StyleCssRule will not be loaded.";
		}
	}

	//TemplateCssRules
	for(Dbo::ptr<Dbo::TemplateCssRule> &ptr : templateCssRuleCollection)
	{
		//TemplateCssRuleMap[std::make_pair(ptr->TemplatePtr->Name(), ptr->TemplatePtr->ModulePtr().id())].insert(Ddo::ptr<Ddo::TemplateCssRuleData>(new TemplateCssRuleDataptr));
		TemplateMap::const_iterator i = templatemap.find(std::make_pair(ptr->templatePtr->name(), ptr->templatePtr->modulePtr().id()));
		if(i != templatemap.end())
			i->second->templateCssRules.insert(Ddo::ptr<Ddo::TemplateCssRule>(new Ddo::TemplateCssRule(ptr)));
		else
			Wt::log("warn") << name() << ": " << "TemplateCssRule with Selector(" << ptr->selector << ") unknown TemplateName(" << ptr->templatePtr->name() << ") and ModuleId(" << ptr->templatePtr->modulePtr().id() << "). This TemplateCssRule will not be loaded.";
	}

	transaction.commit();
	_styleContainer.swap(stylecontainer);
	_templateMap.swap(templatemap);
	_styleTemplateContainer.swap(styletemplatecontainer);

	//Time at end
	boost::posix_time::ptime PTEnd = boost::posix_time::microsec_clock::local_time();
	_loadDuration = PTEnd - PTStart;

	Wt::log("info") << name() << ": " << _styleContainer.size() << " Styles, " << _templateMap.size() << " Templates, "
		<< _styleTemplateContainer.size() << " Styled Templates, " << _styleCssRulesCount << " Style CSS Rules and "
		<< _templateCssRulesCount << " Template CSS Rules successfully loaded in " << _loadDuration.total_milliseconds() << " ms";
}

Ddo::cPtr<Ddo::Style> StyleDatabase::getStylePtr(long long styleId) const
{
	ReadLock lock(manager());
	StyleById::const_iterator itr = _styleContainer.get<ById>().find(styleId);
	if(itr == _styleContainer.get<ById>().end())
		return Ddo::cPtr<Ddo::Style>();

	return *itr;
}
Ddo::cPtr<Ddo::Style> StyleDatabase::getStylePtr(const std::string &name, long long authorId) const
{
	ReadLock lock(manager());
	StyleByNameAuthor::const_iterator itr = _styleContainer.get<ByNameAuthor>().find(boost::make_tuple(name, authorId));
	if(itr == _styleContainer.get<ByNameAuthor>().end())
		return Ddo::cPtr<Ddo::Style>();

	return *itr;
}

Ddo::cPtr<Ddo::Template> StyleDatabase::getTemplatePtr(const std::string &name, long long moduleId) const
{
	ReadLock lock(manager());
	TemplateMap::const_iterator itr = _templateMap.find(std::make_pair(name, moduleId));
	if(itr == _templateMap.end())
		return Ddo::cPtr<Ddo::Template>();
	
	return itr->second;
}

Ddo::cPtr<Ddo::StyleTemplate> StyleDatabase::getStyleTemplatePtr(const std::string &templateName, long long moduleId, const std::string &styleName, long long styleAuthorId) const
{
	ReadLock lock(manager());
	StyleTemplateType::const_iterator itr = _styleTemplateContainer.find(boost::make_tuple(templateName, moduleId, styleName, styleAuthorId));
	if(itr == _styleTemplateContainer.end())
		return Ddo::cPtr<Ddo::StyleTemplate>();

	return *itr;
}

bool StyleDatabase::getTemplateStr(const std::string &name, long long moduleId, std::string &result) const
{
	Ddo::cPtr<Ddo::Template> templatePtr = getTemplatePtr(name, moduleId);
	if(!templatePtr)
	{
		Wt::log("warn") << "TemplatePtr not found in StylesDatabase in GetTemplateStr(...). Name: " << name << ", ModuleId: " << moduleId;
		return false;
	}
	result = templatePtr->templateStr;
	return true;
}

bool StyleDatabase::getStyleTemplateStr(const std::string &templateName, long long moduleId, const std::string &styleName, long long styleAuthorId, std::string &result) const
{
	Ddo::cPtr<Ddo::StyleTemplate> styleTemplatePtr = getStyleTemplatePtr(templateName, moduleId, styleName, styleAuthorId);
	if(!styleTemplatePtr)
		return false;

	result = styleTemplatePtr->templateStr;
	return true;
}

Ddo::cPtr<Ddo::Style> StyleDatabase::firstStylePtr() const
{
	ReadLock lock(manager());
	StyleById::const_iterator itr = _styleContainer.get<ById>().begin();
	if(itr == _styleContainer.get<ById>().end())
		return Ddo::cPtr<Ddo::Style>();

	return *itr;
}

long long StyleDatabase::getLoadDurationinMS() const
{
	ReadLock lock(manager());
	return _loadDuration.total_milliseconds();
}
std::size_t StyleDatabase::stylesCount() const
{
	ReadLock lock(manager());
	return _styleContainer.size();
}
std::size_t StyleDatabase::templatesCount() const
{
	ReadLock lock(manager());
	return _templateMap.size();
}
std::size_t StyleDatabase::styleTemplatesCount() const
{
	ReadLock lock(manager());
	return _styleTemplateContainer.size();
}
std::size_t StyleDatabase::styleCssRulesCount() const
{
	ReadLock lock(manager());
	return _styleCssRulesCount;
}
std::size_t StyleDatabase::templateCssRulesCount() const
{
	ReadLock lock(manager());
	return _templateCssRulesCount;
}

void StyleDatabase::load(Wt::Dbo::Session &DboSession)
{
	MAP_MODULE_DBO_TREE(DboSession)

	fetchAll(DboSession);
}
void StyleDatabase::reload(Wt::Dbo::Session &DboSession)
{
	fetchAll(DboSession);
}

}

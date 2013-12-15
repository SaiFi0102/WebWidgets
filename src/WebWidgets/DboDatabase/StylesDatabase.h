#ifndef STYLES_DATABASE_H
#define STYLES_DATABASE_H

#include "Dbo/Style.h"
#include "Application/WServer.h"
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple_comparison.hpp>

namespace boost
{
	namespace tuples
	{
		std::size_t hash_value(const boost::tuple<std::string, long long, std::string, long long> &e)
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, e.get<0>());
			boost::hash_combine(seed, e.get<1>());
			boost::hash_combine(seed, e.get<2>());
			boost::hash_combine(seed, e.get<3>());
			return seed;
		}
	}
}

class StylesDatabase
{
	protected:
		typedef std::vector< Wt::Dbo::ptr<StyleCssRule> > StyleCssRuleVector;
		typedef std::vector< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleVector;

		typedef boost::unordered_map< std::pair<std::string, long long>, Wt::Dbo::ptr<Style> > StyleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, Wt::Dbo::ptr<Template> > TemplateMaps;
		typedef boost::unordered_map< boost::tuple<std::string, long long, std::string, long long>, Wt::Dbo::ptr<StyleTemplate> > StyleTemplateMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, StyleCssRuleVector > StyleCssRuleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, TemplateCssRuleVector > TemplateCssRuleMaps;

	public:
		StylesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		StylesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void FetchAll(bool FetchCss = false);

		Style GetStyleDbo(const std::string &Name, long long AuthorId) const;
		Template GetTemplateDbo(const std::string &Name, long long ModuleId) const;
		StyleTemplate GetStyleTemplateDbo(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;

		std::string GetTemplateStr(const std::string &Name, long long ModuleId) const;
		std::string GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;
		
		std::string GetStyleSheetFileName(const std::string &StyleName, long long StyleAuthorId) const;
		std::string GetTemplateStyleSheetFileName(const std::string &TemplateName, long long ModuleId) const;

		void CreateCssStyleSheets();

		long long GetLoadDurationinMS() const;
		long long CountStyles() const;
		long long CountTemplates() const;
		long long CountStyleTemplates() const;
		long long CountStyleCssRules() const;
		long long CountTemplateCssRules() const;

	protected:
		virtual void MapClasses();

		Wt::Dbo::ptr<Style> GetStylePtr(const std::string &Name, long long AuthorId) const;
		Wt::Dbo::ptr<Template> GetTemplatePtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<StyleTemplate> GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;

		StyleMaps StyleMap;
		TemplateMaps TemplateMap;
		StyleTemplateMaps StyleTemplateMap;
		StyleCssRuleMaps StyleCssRuleMap;
		TemplateCssRuleMaps TemplateCssRuleMap;

		mutable boost::shared_mutex mutex;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;
};

#endif
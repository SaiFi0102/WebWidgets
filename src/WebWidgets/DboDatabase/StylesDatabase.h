#ifndef STYLES_DATABASE_H
#define STYLES_DATABASE_H

#include "Dbo/Style.h"
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class WServer;

class StylesDatabase
{
	protected:
		struct StyleTemplate_key_TemplateName
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<StyleTemplate> StyleTemplatePtr) const
			{
				return StyleTemplatePtr.id().DerivingTemplatePtr.id().Name;
			}
		};
		struct StyleTemplate_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<StyleTemplate> StyleTemplatePtr) const
			{
				return StyleTemplatePtr.id().DerivingTemplatePtr.id().ModulePtr.id();
			}
		};
		struct StyleTemplate_key_StyleName
		{
			typedef std::string result_type;
			result_type operator()(const Wt::Dbo::ptr<StyleTemplate> StyleTemplatePtr) const
			{
				return StyleTemplatePtr.id().StylePtr.id().Name;
			}
		};
		struct StyleTemplate_key_AuthorId
		{
			typedef long long result_type;
			result_type operator()(const Wt::Dbo::ptr<StyleTemplate> StyleTemplatePtr) const
			{
				return StyleTemplatePtr.id().StylePtr.id().AuthorPtr.id();
			}
		};

		typedef boost::multi_index_container<
			Wt::Dbo::ptr<StyleTemplate>,

			boost::multi_index::indexed_by<
				//Index by Page Id and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Wt::Dbo::ptr<StyleTemplate>,
						StyleTemplate_key_TemplateName,
						StyleTemplate_key_ModuleId,
						StyleTemplate_key_StyleName,
						StyleTemplate_key_AuthorId
					>
				>
			>
		> StyleTemplateContainers;

		typedef std::list< Wt::Dbo::ptr<StyleCssRule> > StyleCssRuleList;
		typedef std::list< Wt::Dbo::ptr<TemplateCssRule> > TemplateCssRuleList;

		typedef boost::unordered_map< std::pair<std::string, long long>, Wt::Dbo::ptr<Style> > StyleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, Wt::Dbo::ptr<Template> > TemplateMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, StyleCssRuleList > StyleCssRuleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, TemplateCssRuleList > TemplateCssRuleMaps;

	public:
		StylesDatabase(Wt::Dbo::SqlConnectionPool &SQLPool, WServer &Server);
		StylesDatabase(Wt::Dbo::SqlConnection &SQLConnection, WServer &Server);

		void Load();
		void Reload();

		Wt::Dbo::ptr<Style> GetStylePtr(const std::string &Name, long long AuthorId) const;
		Wt::Dbo::ptr<Template> GetTemplatePtr(const std::string &Name, long long ModuleId) const;
		Wt::Dbo::ptr<StyleTemplate> GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;

		std::string GetTemplateStr(const std::string &Name, long long ModuleId) const;
		std::string GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;
		
		StyleCssRuleList GetStyleCssRules(const std::string &StyleName, long long AuthorId);
		TemplateCssRuleList GetTemplateCssRules(const std::string &TemplateName, long long ModuleId);

		long long GetLoadDurationinMS() const;
		std::size_t CountStyles() const;
		std::size_t CountTemplates() const;
		std::size_t CountStyleTemplates() const;
		std::size_t CountStyleCssRules() const;
		std::size_t CountTemplateCssRules() const;

	protected:
		void MapClasses();
		void FetchAll();

		StyleMaps StyleMap;
		TemplateMaps TemplateMap;
		StyleTemplateContainers StyleTemplateContainer;
		StyleCssRuleMaps StyleCssRuleMap;
		TemplateCssRuleMaps TemplateCssRuleMap;

		mutable boost::shared_mutex mutex;
		boost::posix_time::time_duration LoadDuration;
		Wt::Dbo::Session DboSession;
		WServer &_Server;

	private:
		friend class ReadLock;
};

#endif
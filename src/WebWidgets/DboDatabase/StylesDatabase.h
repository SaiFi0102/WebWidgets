#ifndef STYLES_DATABASE_H
#define STYLES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Style.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>

class StylesDatabase : public AbstractDboDatabase
{
	protected:
		struct StyleTemplate_key_TemplateName
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<StyleTemplateData> &StyleTemplatePtr) const
			{
				return StyleTemplatePtr->TemplateName;
			}
		};
		struct StyleTemplate_key_ModuleId
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<StyleTemplateData> &StyleTemplatePtr) const
			{
				return StyleTemplatePtr->ModuleId;
			}
		};
		struct StyleTemplate_key_StyleName
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<StyleTemplateData> &StyleTemplatePtr) const
			{
				return StyleTemplatePtr->StyleName;
			}
		};
		struct StyleTemplate_key_AuthorId
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<StyleTemplateData> &StyleTemplatePtr) const
			{
				return StyleTemplatePtr->AuthorId;
			}
		};

		typedef boost::multi_index_container<
			boost::shared_ptr<StyleTemplateData>,

			boost::multi_index::indexed_by<
				//Index by Page Id and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						boost::shared_ptr<StyleTemplateData>,
						StyleTemplate_key_TemplateName,
						StyleTemplate_key_ModuleId,
						StyleTemplate_key_StyleName,
						StyleTemplate_key_AuthorId
					>
				>
			>
		> StyleTemplateContainers;

		typedef std::set< boost::shared_ptr<const StyleCssRuleData> > StyleCssRuleList;
		typedef std::set< boost::shared_ptr<const TemplateCssRuleData> > TemplateCssRuleList;

		typedef boost::unordered_map< std::pair<std::string, long long>, boost::shared_ptr<StyleData> > StyleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, boost::shared_ptr<TemplateData> > TemplateMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, StyleCssRuleList > StyleCssRuleMaps;
		typedef boost::unordered_map< std::pair<std::string, long long>, TemplateCssRuleList > TemplateCssRuleMaps;

	public:
		StylesDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const StyleData> GetStylePtr(const std::string &Name, long long AuthorId) const;
		boost::shared_ptr<const TemplateData> GetTemplatePtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const StyleTemplateData> GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;

		bool GetTemplateStr(const std::string &Name, long long ModuleId, std::string &result) const;
		bool GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId, std::string &result) const;
		
		StyleCssRuleList GetStyleCssRules(const std::string &StyleName, long long AuthorId);
		TemplateCssRuleList GetTemplateCssRules(const std::string &TemplateName, long long ModuleId);

		long long GetLoadDurationinMS() const;
		std::size_t CountStyles() const;
		std::size_t CountTemplates() const;
		std::size_t CountStyleTemplates() const;
		std::size_t CountStyleCssRules() const;
		std::size_t CountTemplateCssRules() const;

		virtual std::string Name() const { return "StylesDatabase"; }

	protected:
		virtual void Load(Wt::Dbo::Session &DboSession);
		virtual void Reload(Wt::Dbo::Session &DboSession);
		void FetchAll(Wt::Dbo::Session &DboSession);

		virtual bool IsVital() const { return true; }

		StyleMaps StyleMap;
		TemplateMaps TemplateMap;
		StyleTemplateContainers StyleTemplateContainer;
		StyleCssRuleMaps StyleCssRuleMap;
		TemplateCssRuleMaps TemplateCssRuleMap;

		boost::posix_time::time_duration LoadDuration;
};

#endif
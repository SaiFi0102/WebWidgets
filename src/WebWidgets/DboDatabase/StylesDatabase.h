#ifndef STYLES_DATABASE_H
#define STYLES_DATABASE_H

#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/Style.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/unordered_map.hpp>

class StylesDatabase : public AbstractDboDatabase
{
	protected:
		struct Style_key_id
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<StyleData> &StylePtr) const
			{
				return StylePtr->id();
			}
		};
		struct Style_key_Name
		{
			typedef std::string result_type;
			result_type operator()(const boost::shared_ptr<StyleData> &StylePtr) const
			{
				return StylePtr->Name();
			}
		};
		struct Style_key_AuthorId
		{
			typedef long long result_type;
			result_type operator()(const boost::shared_ptr<StyleData> &StylePtr) const
			{
				return StylePtr->AuthorId();
			}
		};
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

		struct ById{};
		struct ByNameAuthor{};
		typedef boost::multi_index_container<
			boost::shared_ptr<StyleData>,

			boost::multi_index::indexed_by<
				//Index by Stlye id
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<ById>,
					Style_key_id
				>,
				//Index by Style Name and Author ID
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<ByNameAuthor>,
					boost::multi_index::composite_key<
						boost::shared_ptr<StyleData>,
						Style_key_Name,
						Style_key_AuthorId
					>
				>
			>
		> StyleContainers;

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

		typedef StyleContainers::index<ById>::type StyleById;
		typedef StyleContainers::index<ByNameAuthor>::type StyleByNameAuthor;
		typedef StyleTemplateContainers::nth_index<0>::type StyleTemplateType;

		typedef boost::unordered_map< std::pair<std::string, long long>, boost::shared_ptr<TemplateData> > TemplateMaps;

	public:
		StylesDatabase(DboDatabaseManager *Manager);

		boost::shared_ptr<const StyleData> GetStylePtr(long long StyleId) const;
		boost::shared_ptr<const StyleData> GetStylePtr(const std::string &Name, long long AuthorId) const;
		boost::shared_ptr<const TemplateData> GetTemplatePtr(const std::string &Name, long long ModuleId) const;
		boost::shared_ptr<const StyleTemplateData> GetStyleTemplatePtr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId) const;

		bool GetTemplateStr(const std::string &Name, long long ModuleId, std::string &result) const;
		bool GetStyleTemplateStr(const std::string &TemplateName, long long ModuleId, const std::string &StyleName, long long StyleAuthorId, std::string &result) const;
		
		boost::shared_ptr<const StyleData> FirstStylePtr() const;

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

		StyleContainers StyleContainer;
		TemplateMaps TemplateMap;
		StyleTemplateContainers StyleTemplateContainer;
		std::size_t _CountStyleCssRules;
		std::size_t _CountTemplateCssRules;

		boost::posix_time::time_duration LoadDuration;
};

#endif
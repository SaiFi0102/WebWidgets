#ifndef WW_DBODATABASE_STYLE_H
#define WW_DBODATABASE_STYLE_H

#include <unordered_map>
#include "Objects/PairHash.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include "DboDatabase/AbstractDboDatabase.h"
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class StyleDatabase : public AbstractDboDatabase
	{
	protected:
		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::Style>,

			boost::multi_index::indexed_by<
				//Index by Style id
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ById>,
					boost::multi_index::const_mem_fun<Ddo::SurrogateKey, long long, &Ddo::Style::id>
				>,
				//Index by Style Name and Author ID
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<struct ByNameAuthor>,
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::Style>,
						boost::multi_index::const_mem_fun<Dbo::BaseStyle, std::string, &Ddo::Style::name>,
						boost::multi_index::const_mem_fun<Ddo::Style, long long, &Ddo::Style::authorId>
					>
				>
			>
		> StyleContainer;

		typedef boost::multi_index_container<
			Ddo::ptr<Ddo::StyleTemplate>,

			boost::multi_index::indexed_by<
				//Index by Page Id and Module Id
				boost::multi_index::hashed_unique<
					boost::multi_index::composite_key<
						Ddo::ptr<Ddo::StyleTemplate>,
						boost::multi_index::member<Ddo::StyleTemplate, std::string, &Ddo::StyleTemplate::templateName>,
						boost::multi_index::member<Ddo::StyleTemplate, long long, &Ddo::StyleTemplate::moduleId>,
						boost::multi_index::member<Ddo::StyleTemplate, std::string, &Ddo::StyleTemplate::styleName>,
						boost::multi_index::member<Ddo::StyleTemplate, long long, &Ddo::StyleTemplate::authorId>
					>
				>
			>
		> StyleTemplateContainer;

		typedef StyleContainer::index<ById>::type StyleById;
		typedef StyleContainer::index<ByNameAuthor>::type StyleByNameAuthor;
		typedef StyleTemplateContainer::nth_index<0>::type StyleTemplateType;

		typedef std::unordered_map< std::pair<std::string, long long>, Ddo::ptr<Ddo::Template> > TemplateMap;

	public:
		StyleDatabase(DboDatabaseManager *manager);

		Ddo::cPtr<Ddo::Style> getStylePtr(long long styleId) const;
		Ddo::cPtr<Ddo::Style> getStylePtr(const std::string &name, long long authorId) const;
		Ddo::cPtr<Ddo::Template> getTemplatePtr(const std::string &name, long long moduleId) const;
		Ddo::cPtr<Ddo::StyleTemplate> getStyleTemplatePtr(const std::string &templateName, long long moduleId, const std::string &styleName, long long styleAuthorId) const;

		bool getTemplateStr(const std::string &name, long long moduleId, std::string &result) const;
		bool getStyleTemplateStr(const std::string &templateName, long long moduleId, const std::string &styleName, long long styleAuthorId, std::string &result) const;
		
		Ddo::cPtr<Ddo::Style> firstStylePtr() const;

		long long getLoadDurationinMS() const;
		std::size_t stylesCount() const;
		std::size_t templatesCount() const;
		std::size_t styleTemplatesCount() const;
		std::size_t styleCssRulesCount() const;
		std::size_t templateCssRulesCount() const;

		virtual std::string name() const override { return "StylesDatabase"; }

	protected:
		virtual void load(Wt::Dbo::Session &dboSession) override;
		virtual void reload(Wt::Dbo::Session &dboSession) override;
		void fetchAll(Wt::Dbo::Session &dboSession);

		virtual bool isVital() const override { return true; }

		StyleContainer _styleContainer;
		TemplateMap _templateMap;
		StyleTemplateContainer _styleTemplateContainer;
		std::size_t _styleCssRulesCount = 0;
		std::size_t _templateCssRulesCount = 0;

		boost::posix_time::time_duration _loadDuration;
	};
}

#endif
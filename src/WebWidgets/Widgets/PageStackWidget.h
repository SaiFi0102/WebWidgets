#ifndef WW_WIDGET_PAGESTACK_H
#define WW_WIDGET_PAGESTACK_H

#include <Wt/WStackedWidget>
#include <unordered_map>
#include "Dbo/Page.h"

namespace WW
{
	class PageStackWidget : public Wt::WStackedWidget
	{
	public:
		PageStackWidget(Wt::WContainerWidget *parent = nullptr);
		Wt::WWidget *getPageContent(long long pageId) const;
		Wt::WWidget *getPageContent(const std::string &pageName, long long moduleId) const;
		int lazyLoadPage(long long pageId) { return lazyLoadPage(pageId, _indexMap); }
		int lazyLoadPage(const std::string &pageName, long long moduleId);
		void changePage(long long pageId);
		void changePage(const std::string &pageName, long long moduleId);

	private:
		typedef std::unordered_map<long long, int> PageIdIndexMap;

		void handlePageChanged();
		bool checkAuthorization(Ddo::cPtr<Ddo::Page> pagePtr);
		void lazyLoadPageVisitor(Ddo::cPtr<Ddo::Page> pagePtr) { lazyLoadPage(pagePtr->id(), _indexMap); }
		int lazyLoadPage(long long pageId, PageIdIndexMap &indexMap);

		PageIdIndexMap _indexMap;
		PageIdIndexMap _index404Map;
	};
}

#endif
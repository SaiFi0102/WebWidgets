#ifndef ABSTRACT_PAGE_H
#define ABSTRACT_PAGE_H

#include <Wt/WWidget>
#include "Dbo/Style.h"

struct PageSections
{
	protected:
		typedef std::map<std::pair<StyleSection::HorizontalOrientation, StyleSection::VerticalOrientation>, Wt::WWidget*> SectionMaps;
		SectionMaps _SectionMap;

	public:
		void Add(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO);
		bool Remove(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO);
};

class AbstractPage
{
	public:
		AbstractPage() : _Page404(0) { }
		virtual ~AbstractPage() { delete _Page404; }

		virtual Wt::WWidget *CreateContent() = 0;
		AbstractPage *Get404Page() { return _Page404; }
		//virtual PageSections CreateSections() { return PageSections(); }

	protected:
		void Set404Page(AbstractPage *page) { _Page404 = page; }

	private:
		AbstractPage *_Page404;
};

class Default404Page : public AbstractPage
{
	public:
		Wt::WWidget *CreateContent();
};

#endif
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
		virtual Wt::WWidget *CreateContent() = 0;
		virtual PageSections CreateSections() { return PageSections(); }
		virtual Wt::WWidget *Create404Page() { return 0; }
};

#endif
#ifndef WW_PAGEHANDLER_H
#define WW_PAGEHANDLER_H

#include <Wt/WWidget>

namespace WW
{
// 	struct PageSections
// 	{
// 	protected:
// 		typedef std::map<std::pair<StyleSection::HorizontalOrientation, StyleSection::VerticalOrientation>, Wt::WWidget*> SectionMaps;
// 		SectionMaps _SectionMap;
// 
// 	public:
// 		void Add(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO);
// 		bool Remove(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO);
// 	};

	class PageHandler
	{
	public:
		virtual ~PageHandler() { delete _page404; }

		virtual Wt::WWidget *createContent() = 0;
		PageHandler *get404Page() { return _page404; }
		//virtual PageSections CreateSections() { return PageSections(); }

	protected:
		void set404Page(PageHandler *page);

	private:
		PageHandler *_page404 = nullptr;
	};

	class Default404Page : public PageHandler
	{
	public:
		Wt::WWidget *createContent();
	};
}

#endif
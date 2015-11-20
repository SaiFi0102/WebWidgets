#include "Pages/AbstractPage.h"

void PageSections::Add(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO)
{
	if(Widget)
	{
		_SectionMap[std::make_pair(HO, VO)] = Widget;
	}
}

bool PageSections::Remove(Wt::WWidget *Widget, StyleSection::HorizontalOrientation HO, StyleSection::VerticalOrientation VO)
{
	if(Widget)
	{
		return _SectionMap.erase(std::make_pair(HO, VO)) != 0;
	}
	return false;
}

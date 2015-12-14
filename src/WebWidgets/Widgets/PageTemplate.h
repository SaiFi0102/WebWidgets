#ifndef WW_WIDGET_PAGETEMPLATE_H
#define WW_WIDGET_PAGETEMPLATE_H

#include <Wt/WCompositeWidget>
#include <Wt/WTemplate>

namespace WW
{
	class PageTemplate : public Wt::WCompositeWidget
	{
	public:
		PageTemplate(Wt::WContainerWidget *parent = nullptr);

	protected:
		Wt::WTemplate *_implTemplate;
	};
}

#endif
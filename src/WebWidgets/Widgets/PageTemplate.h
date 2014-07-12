#ifndef PAGETEMPLATE_WIDGET_H
#define PAGETEMPLATE_WIDGET_H

#include <Wt/WCompositeWidget>
#include <Wt/WTemplate>

class PageTemplate : public Wt::WCompositeWidget
{
	public:
		PageTemplate(Wt::WContainerWidget *parent = 0);

	protected:
		Wt::WTemplate *ImplTemplate;
};

#endif
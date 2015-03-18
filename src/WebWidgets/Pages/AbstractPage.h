#ifndef ABSTRACT_PAGE_H
#define ABSTRACT_PAGE_H

#include <Wt/WWidget>

class AbstractPage
{
	public:
		virtual Wt::WWidget *CreatePage() = 0;
		virtual Wt::WWidget *Create404Page() { return 0; }
};

#endif
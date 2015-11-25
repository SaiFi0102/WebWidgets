#ifndef WWPAGESSTACK_WIDGET_H
#define WWPAGESSTACK_WIDGET_H

#include <Wt/WStackedWidget>
#include <unordered_map>

class PagesStackWidget : public Wt::WStackedWidget
{
	public:
		PagesStackWidget(Wt::WContainerWidget *parent = 0);

	private:
		typedef std::unordered_map<long long, int> PageIdIndexMap;
		PageIdIndexMap indexMap;
		PageIdIndexMap index404Map;

		void HandlePageChanged();
};

#endif
#include "Widgets/PageTemplate.h"
#include "DboDatabase/ModuleDatabase.h"

namespace WW
{

PageTemplate::PageTemplate(Wt::WContainerWidget *parent)
	: Wt::WCompositeWidget(parent)
{
	setImplementation(_implTemplate = new Wt::WTemplate());
	_implTemplate->setTemplateText(Wt::WString::tstr("body", ModuleDatabase::Navigation));
}

}

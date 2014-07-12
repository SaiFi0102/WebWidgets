#include "Widgets/PageTemplate.h"
#include "DboDatabase/ModulesDatabase.h"

PageTemplate::PageTemplate(Wt::WContainerWidget *parent)
	: Wt::WCompositeWidget(parent)
{
	setImplementation(ImplTemplate = new Wt::WTemplate());
	ImplTemplate->setTemplateText(Wt::WString::tstr("body", ModulesDatabase::Navigation));
}

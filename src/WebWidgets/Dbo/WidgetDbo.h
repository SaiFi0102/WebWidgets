#ifndef WIDGET_DBO_H
#define WIDGET_DBO_H

#include "Dbo/DboTraits.h"
#include "Dbo/Module.h"

// class Widget : public Wt::Dbo::Dbo<Widget>
// {
// 	public:
// 
// 		Widget() { }
// 		Widget(const std::string &Name, Wt::Dbo::ptr<Module> ModulePtr = Wt::Dbo::ptr<Module>())
// 			: _Id(Name, ModulePtr)
// 		{ }
// 
// 
// 
// 		template<class Action>void persist(Action &a)
// 		{
// 			Wt::Dbo::id(a, _Id, "Widget");
// 		}
// 		static const char *TableName()
// 		{
// 			return "widget";
// 		}
// 
// 	private:
// 		WidgetKeys _Id;

#endif
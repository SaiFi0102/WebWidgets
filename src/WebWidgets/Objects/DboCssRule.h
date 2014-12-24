#ifndef DBOCSSRULE_H
#define DBOCSSRULE_H

#include <Wt/WCssStyleSheet>
#include "Dbo/Style.h"

class DboCssRule : public Wt::WCssRule
{
	public:
		DboCssRule(boost::shared_ptr<StyleCssRuleData> CssRulePtr, Wt::WObject *parent = 0);
		DboCssRule(boost::shared_ptr<TemplateCssRuleData> CssRulePtr, Wt::WObject *parent = 0);

		virtual std::string selector() const;
		virtual std::string declarations();

	protected:
		boost::shared_ptr<StyleCssRuleData> StyleCssRulePtr;
		boost::shared_ptr<TemplateCssRuleData> TemplateCssRulePtr;
		bool IsTemplate;
};

#endif
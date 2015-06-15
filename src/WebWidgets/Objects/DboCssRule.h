#ifndef DBOCSSRULE_H
#define DBOCSSRULE_H

#include <Wt/WCssStyleSheet>
#include "Dbo/Style.h"

class DboCssRule : public Wt::WCssRule
{
	public:
		DboCssRule(boost::shared_ptr<const StyleCssRuleData> CssRulePtr, Wt::WObject *parent = 0);
		DboCssRule(boost::shared_ptr<const TemplateCssRuleData> CssRulePtr, Wt::WObject *parent = 0);

		virtual std::string selector() const;
		virtual std::string declarations();

	protected:
		boost::shared_ptr<const StyleCssRuleData> StyleCssRulePtr;
		boost::shared_ptr<const TemplateCssRuleData> TemplateCssRulePtr;
		bool IsTemplate;
};

#endif
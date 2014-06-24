#ifndef DBOCSSRULE_H
#define DBOCSSRULE_H

#include <Wt/WCssStyleSheet>
#include "Dbo/Style.h"

class DboCssRule : public Wt::WCssRule
{
	public:
		DboCssRule(Wt::Dbo::ptr<StyleCssRule> CssRulePtr, Wt::WObject *parent = 0);
		DboCssRule(Wt::Dbo::ptr<TemplateCssRule> CssRulePtr, Wt::WObject *parent = 0);
		/*virtual ~DboCssRule();*/

		virtual std::string selector() const;
		virtual std::string declarations();

	protected:
		Wt::Dbo::ptr<StyleCssRule> StyleCssRulePtr;
		Wt::Dbo::ptr<TemplateCssRule> TemplateCssRulePtr;
		bool IsTemplate;
};

#endif
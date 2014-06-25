#include "Objects/DboCssRule.h"

DboCssRule::DboCssRule(Wt::Dbo::ptr<StyleCssRule> CssRulePtr, Wt::WObject *parent)
	: StyleCssRulePtr(CssRulePtr), Wt::WCssRule(parent), IsTemplate(false)
{
	if(!CssRulePtr || CssRulePtr.isTransient())
	{
		throw std::runtime_error("DboCssRule constructor called with an empty or transient CssRulePtr");
	}
}

DboCssRule::DboCssRule(Wt::Dbo::ptr<TemplateCssRule> CssRulePtr, Wt::WObject *parent)
	: TemplateCssRulePtr(CssRulePtr), Wt::WCssRule(parent), IsTemplate(true)
{ }

std::string DboCssRule::selector() const
{
	if(IsTemplate)
	{
		return TemplateCssRulePtr->Selector;
	}
	else
	{
		return StyleCssRulePtr->Selector;
	}
}

std::string DboCssRule::declarations()
{
	if(IsTemplate)
	{
		return TemplateCssRulePtr->Declarations;
	}
	else
	{
		return StyleCssRulePtr->Declarations;
	}
}

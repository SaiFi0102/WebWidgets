#include "Objects/DboCssRule.h"

DboCssRule::DboCssRule(boost::shared_ptr<const StyleCssRuleData> CssRulePtr, Wt::WObject *parent)
	: StyleCssRulePtr(CssRulePtr), Wt::WCssRule(parent), IsTemplate(false)
{
	if(!CssRulePtr)
	{
		throw std::runtime_error("DboCssRule constructor called with an empty CssRulePtr");
	}
}

DboCssRule::DboCssRule(boost::shared_ptr<const TemplateCssRuleData> CssRulePtr, Wt::WObject *parent)
	: TemplateCssRulePtr(CssRulePtr), Wt::WCssRule(parent), IsTemplate(true)
{
	if(!CssRulePtr)
	{
		throw std::runtime_error("DboCssRule constructor called with an empty CssRulePtr");
	}
}

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

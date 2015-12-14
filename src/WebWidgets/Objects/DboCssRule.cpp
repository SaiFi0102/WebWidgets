#include "Objects/DboCssRule.h"

namespace WW
{

DboCssRule::DboCssRule(Ddo::cPtr<Ddo::StyleCssRule> cssRulePtr, Wt::WObject *parent)
: _styleCssRulePtr(cssRulePtr), Wt::WCssRule(parent)
{
	if(!cssRulePtr)
		throw std::runtime_error("DboCssRule constructor called with an empty CssRulePtr");
}

DboCssRule::DboCssRule(Ddo::cPtr<Ddo::TemplateCssRule> CssRulePtr, Wt::WObject *parent)
: _templateCssRulePtr(CssRulePtr), Wt::WCssRule(parent)
{
	if(!CssRulePtr)
		throw std::runtime_error("DboCssRule constructor called with an empty CssRulePtr");
}

std::string DboCssRule::selector() const
{
	if(_templateCssRulePtr)
		return _templateCssRulePtr->selector;
	else
		return _styleCssRulePtr->selector;
}

std::string DboCssRule::declarations()
{
	if(_templateCssRulePtr)
		return _templateCssRulePtr->declarations;
	else
		return _styleCssRulePtr->declarations;
}

}
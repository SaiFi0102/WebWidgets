#ifndef WW_DBOCSSRULE_H
#define WW_DBOCSSRULE_H

#include <Wt/WCssStyleSheet>
#include "Dbo/ModuleTreeDbos.h"

namespace WW
{
	class DboCssRule : public Wt::WCssRule
	{
	public:
		DboCssRule(Ddo::cPtr<Ddo::StyleCssRule> cssRulePtr, Wt::WObject *parent = nullptr);
		DboCssRule(Ddo::cPtr<Ddo::TemplateCssRule> cssRulePtr, Wt::WObject *parent = nullptr);

		virtual std::string selector() const override;
		virtual std::string declarations() override;

	protected:
		Ddo::cPtr<Ddo::StyleCssRule> _styleCssRulePtr;
		Ddo::cPtr<Ddo::TemplateCssRule> _templateCssRulePtr;
	};
}

#endif
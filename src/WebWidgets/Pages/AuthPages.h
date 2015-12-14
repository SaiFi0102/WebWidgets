#ifndef WW_PAGEHANDLER_AUTH_H
#define WW_PAGEHANDLER_AUTH_H

#include "Pages/PageHandler.h"
#include <Wt/WWidget>

namespace WW
{
	class LoginPage : public PageHandler
	{
		virtual Wt::WWidget *createContent() override;
	};

	class RegistrationPage : public PageHandler
	{
		virtual Wt::WWidget *createContent() override;
	};
}

#endif
#ifndef WW_SESSION_H
#define WW_SESSION_H

#include <Wt/Auth/Login>
#include "Dbo/ModuleTreeDbos.h"
#include "Dbo/UserTreeDbos.h"

namespace WW
{
	typedef Wt::Auth::Dbo::UserDatabase<Dbo::AuthInfo> UserDatabase;

	class Session : public Wt::Dbo::Session
	{
	public:
		Session();
		~Session();

		Dbo::ptr<Dbo::User> user();
		Dbo::ptr<Dbo::User> user(const Wt::Auth::User &user);

		Wt::Auth::AbstractUserDatabase &userDatabase();
		Wt::Auth::Login &login() { return _login; }

	private:
		UserDatabase *_userDatabase;
		Wt::Auth::Login _login;
	};
}

#endif
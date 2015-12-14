#include "Application/Session.h"
#include "Application/WServer.h"

#include <Wt/Auth/AuthService>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>
#include <Wt/Auth/GoogleService>
#include <Wt/Auth/FacebookService>
#include <Wt/Auth/Dbo/AuthInfo>
#include <Wt/Auth/Dbo/UserDatabase>

namespace WW
{

Session::Session()
{
	setConnectionPool(*WServer::instance()->sqlPool());

	MAP_MODULE_DBO_TREE((*this))
	MAP_USER_DBO_TREE((*this))

	_userDatabase = new UserDatabase(*this);
}

Session::~Session()
{
	delete _userDatabase;
}

Dbo::ptr<Dbo::User> Session::user()
{
	if(_login.loggedIn())
		return user(_login.user());
	else
		return Dbo::ptr<Dbo::User>();
}

Dbo::ptr<Dbo::User> Session::user(const Wt::Auth::User &authUser)
{
	Dbo::ptr<Dbo::AuthInfo> authInfo = _userDatabase->find(authUser);
	Dbo::ptr<Dbo::User> user = authInfo->user();

	if(!user)
	{
		user = add(new Dbo::User());
		authInfo.modify()->setUser(user);
	}
	return user;
}

Wt::Auth::AbstractUserDatabase &Session::userDatabase()
{
	return *_userDatabase;
}

}

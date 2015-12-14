#ifndef WW_DBO_USERTREE_H
#define WW_DBO_USERTREE_H

#include "Dbo/DboDefs.h"
#include <Wt/Auth/Dbo/AuthInfo>

namespace WW
{
	namespace Dbo
	{
		class User;
		typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;
		typedef AuthInfo::AuthIdentityType AuthIdentity;
		typedef AuthInfo::AuthTokenType AuthToken;

		typedef Wt::Dbo::collection< ptr<User> > UserCollection;
	}
}

#include "Dbo/User.h"

#define MAP_USER_DBO_TREE(dboSession) \
	dboSession.mapClass<Dbo::User>("user"); \
	dboSession.mapClass<Dbo::AuthInfo>("auth_info"); \
	dboSession.mapClass<Dbo::AuthIdentity>("auth_identity"); \
	dboSession.mapClass<Dbo::AuthToken>("auth_token");

#endif
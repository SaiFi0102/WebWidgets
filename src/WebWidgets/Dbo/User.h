#ifndef WW_DBO_USER_H
#define WW_DBO_USER_H

#include "Dbo/UserTreeDbos.h"

namespace WW
{
	namespace Dbo
	{

		class User
		{
		public:
			template<class Action>
			void persist(Action& a)
			{ }
			constexpr static const char *tableName()
			{
				return "user";
			}
		};
// 		class AuthInfo : public Wt::Auth::Dbo::AuthInfo<User>
// 		{
// 		public:
// 			constexpr static const char *tableName()
// 			{
// 				return "auth_info";
// 			}
// 		};
// 		class AuthIdentity : public AuthInfo::AuthIdentityType
// 		{
// 		public:
// 			constexpr static const char *tableName()
// 			{
// 				return "auth_identity";
// 			}
// 		};
// 		class AuthToken : public AuthInfo::AuthTokenType
// 		{
// 		public:
// 			constexpr static const char *tableName()
// 			{
// 				return "auth_token";
// 			}
// 		};
		
	}
}

#endif
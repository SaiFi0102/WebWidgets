#ifndef SESSION_WAPPLICATION_H
#define SESSION_WAPPLICATION_H

#include <Wt/Dbo/Session>
#include <Wt/Auth/Login>

class Session : public Wt::Dbo::Session
{
	public:
		Session(Wt::Dbo::SqlConnectionPool &SQLPool);
		Session(Wt::Dbo::SqlConnection &SQLConnection);
		~Session();

		//Wt::Auth::AbstractUserDatabase &UserDatabase();
		//Wt::Auth::Login &Login() { return Login; }

	private:
		Wt::Auth::Login Login;
};

#endif
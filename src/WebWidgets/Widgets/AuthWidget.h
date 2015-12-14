#ifndef WW_WIDGET_AUTH_H
#define WW_WIDGET_AUTH_H

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/RegistrationWidget>
#include <Wt/WFormModel>

namespace WW
{
	class Session;
	class UserDetailsModel;

	class UserDetailsModel : public Wt::WFormModel
	{
	public:
		UserDetailsModel(Session &session, Wt::WObject *parent = nullptr);
		void save(const Wt::Auth::User& user);

	private:
		Session &_session;
	};

	class AuthWidget : public Wt::Auth::AuthWidget
	{
	public:
		AuthWidget(Session &session);
		virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity &id) override;

// 	protected:
// 		virtual void createLoggedInView() override;

	private:
		Session &_session;
	};

	class RegistrationView : public Wt::Auth::RegistrationWidget
	{
	public:
		RegistrationView(Session &session, Wt::Auth::AuthWidget *authWidget = nullptr);
		virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field) override;

	protected:
		/* specialize to also validate the user details */
		virtual bool validate() override;

		/* specialize to register user details */
		virtual void registerUserDetails(Wt::Auth::User &user) override;

	private:
		Session &_session;
		UserDetailsModel *_detailsModel;
	};
}

#endif
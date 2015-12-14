#include "Widgets/AuthWidget.h"
#include "Application/Session.h"
#include "Application/WServer.h"
#include "DboDatabase/ModuleDatabase.h"

#include <Wt/Auth/Login>

namespace WW
{

//const Wt::WFormModel::Field UserDetailsModel::FavouritePetField = "favourite-pet";

UserDetailsModel::UserDetailsModel(Session& session, Wt::WObject *parent)
: Wt::WFormModel(parent),
_session(session)
{
	//addField(FavouritePetField, Wt::WString::tr("favourite-pet-info"));
}

void UserDetailsModel::save(const Wt::Auth::User& authUser)
{
	Dbo::ptr<Dbo::User> user = _session.user(authUser);
	//user.modify()->favouritePet = valueText(FavouritePetField).toUTF8();
}

AuthWidget::AuthWidget(Session &session)
: Wt::Auth::AuthWidget(WServer::instance()->getAuthService(), session.userDatabase(), session.login()), _session(session)
{ }

Wt::WWidget *AuthWidget::createRegistrationView(const Wt::Auth::Identity& id)
{
	RegistrationView *w = new RegistrationView(_session, this);
	w->setTemplateText(Wt::WString::tstr("registration", ModuleDatabase::Authentication));
	delete w->takeWidget("cancel-button");

	Wt::Auth::RegistrationModel *model = createRegistrationModel();
	if(id.isValid())
	{
		model->registerIdentified(id);
	}

	w->setModel(model);

	return w;
}

// void AuthWidget::createLoggedInView()
// {
// 
// }

RegistrationView::RegistrationView(Session& session, Wt::Auth::AuthWidget *authWidget)
: Wt::Auth::RegistrationWidget(authWidget), _session(session)
{
	//setTemplateText(tr("template.registration"));
	_detailsModel = new UserDetailsModel(_session, this);

	updateView(_detailsModel);
}

Wt::WFormWidget *RegistrationView::createFormWidget(Wt::WFormModel::Field field)
{
	//if (field == UserDetailsModel::FavouritePetField)
	//  return new Wt::WLineEdit();
	//else
	return Wt::Auth::RegistrationWidget::createFormWidget(field);
}

bool RegistrationView::validate()
{
	bool result = Wt::Auth::RegistrationWidget::validate();

	updateModel(_detailsModel);
	if(!_detailsModel->validate())
		result = false;

	updateView(_detailsModel);
	return result;
}

void RegistrationView::registerUserDetails(Wt::Auth::User& user)
{
	_detailsModel->save(user);
}

}

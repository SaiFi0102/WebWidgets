#ifndef WW_UTILITY_SERVERSIGNAL_H
#define WW_UTILITY_SERVERSIGNAL_H

#include <Wt/WSignal>
//#include <Wt/WObject>
#include "Application/WApplication.h"
#include "Application/WServer.h"
#include <boost/bind/placeholders.hpp>

namespace WW
{
	class WServer;

	template<typename ...Args>
	class ServerSignal
	{
	public:
		ServerSignal(WServer *sender)
			: _server(sender)
		{ }
		~ServerSignal()
		{
			delete impl_;
		}

		template<class F>
		boost::signals2::connection connect(const F& function)
		{
			WApplication *app = WApplication::instance();
			if(!app)
				return boost::signals2::connection();

			if(!impl_)
				impl_ = new BoostSignalType;

			return impl_->connect(boost::bind(&ServerSignal<Args...>::serverSignalConnector, this, app->bind(function), app), Wt::Signals::at_front);
		}


		void emit(const Args &...args) const
		{
			if(impl_)
				(*impl_)(args...);
		}
		void operator()(const Args &...args) const { emit(args...); }

		bool isConnected() const
		{
			return impl_ ? impl_->num_slots() > 0 : false;
		}
		constexpr int argumentCount() const
		{
			return sizeof...(Args);
		}

	protected:
		WServer *_server;

	private:
		ServerSignal(const ServerSignal<Args...>&);

		void serverSignalConnector(const boost::function<void()> &function, WApplication *app)
		{
			_server->post(app->sessionId(), function);
		}

		typedef boost::signals2::signal<void(Args...)> BoostSignalType;
		BoostSignalType *impl_;
	};

}

#endif

#include "Wt/ServerSignal"

#define READ_LOCK boost::shared_lock<boost::shared_mutex> lock(mutex)
#define WRITE_LOCK boost::lock_guard<boost::shared_mutex> lock(mutex)

namespace Wt
{
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	ServerSignal::Connection::Connection(ServerSignal *Signal)
		: _signal(Signal), _blocked(false)
	{ }

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void ServerSignal::Connection::~Connection() const
	{
		if(_signal)
		{
			_signal->disconnect(this);
		}
	}
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void ServerSignal::Connection::block() const
	{
		if(_signal)
		{
			_signal->block(this, true);
		}
	}
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void ServerSignal::Connection::unblock() const
	{
		if(_signal)
		{
			_signal->block(this, false);
		}
	}
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	bool ServerSignal::Connection::blocked() const
	{
		return _blocked;
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	ServerSignal::ServerSignal(WServer *sender)
		: _sender(sender)
	{ }
	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	ServerSignal::~ServerSignal()
	{
		WRITE_LOCK;
		for(ConnectionsSet::iterator itr = _connections.begin();
			itr != _connections.end();
			++itr)
		{
			delete *itr;
		}
	}
}

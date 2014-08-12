#pragma once

#include "../externs/FastDelegate.h"
#include "../sys.h"

#include <list>
#include <map>
#include <typeinfo>

namespace sys
{
namespace events
{

//Store all the different types of messages.
class MsgTypeRegistry
{
	public:
		static std::vector< std::string > msgTypes;

};

//Recieves messages and sends them to listeners that listen to that message.
class MsgReceiver;

//Listens for specific messages.
class MsgListener;

//Base of Msg.
class MsgBase
{
public:
	MsgBase()
	{
		userdata = 0;
	}
	s32 userdata;
	virtual s32 MsgTypeID() const { return -1; }

protected:
	static s32 GetNextMsgID()
	{
		static s32 nextid = -1;
		nextid++;
		return nextid;
	}
};

//The message that is sent out from recievers to listeners.
template <class TMSG> class Msg : public MsgBase
{
public:

	inline static s32 GetMsgTypeID()
	{
		static s32 myid = -1;
		if(myid == -1)
		{
			myid = MsgBase::GetNextMsgID();
			std::string msgname;
#ifdef WIN32
			//THIS ONLY WORKS FOR WINDOWS
			msgname = &typeid(TMSG).name()[6];
#else
#error This platform needs a different typeid name format and needs to be added. If you are compiling to Windows, add WIN32 as a preprocessing define.
#endif

			MsgTypeRegistry::msgTypes.push_back( msgname );
		}
		return myid;
	}

	virtual s32 MsgTypeId() const
	{
		return GetMsgTypeID();
	}

};

typedef fastdelegate::FastDelegate1<const MsgBase&> MsgReceiver_GeneralFunc;
struct MsgReceiver_ListenInfo;

class MsgReceiverListenId
{
public:
	MsgReceiverListenId()
	{
	}
private:
	friend class MsgListener;
	MsgReceiverListenId(std::list< MsgReceiver_ListenInfo >::iterator itr)
	{
		this->itr = itr;
	}
	std::list< MsgReceiver_ListenInfo >::iterator itr;
};

struct MsgReceiver_Info
{
	MsgReceiver_Info(MsgListener* listener, const MsgReceiver_GeneralFunc& func,  MsgReceiverListenId listerneritr, s32 userdata, s32 counter_created)
	{
		this->listener = listener;
		this->func = func;
		this->listerneritr = listerneritr;
		this->removed = false;
		this->userdata = userdata;
		this->counter_created = counter_created;
	}
	MsgListener* listener;
	MsgReceiver_GeneralFunc func;
	MsgReceiverListenId listerneritr;
	bool removed;
	s32 counter_created;
	s32 userdata;
};

class MsgReceiver_RListenId
{
public:
	MsgReceiver_RListenId()
	{
	}
private:
	friend class MsgReceiver;
	MsgReceiver_RListenId(s32 msgtypeid,std::list< MsgReceiver_Info >::iterator itr)
	{
		this->msgtypeid = msgtypeid;
		this->itr = itr;
	}
	std::list< MsgReceiver_Info >::iterator itr;
	s32 msgtypeid;
};

struct MsgReceiver_ListenInfo
{
	MsgReceiver_ListenInfo()
	{
	}
	MsgReceiver_ListenInfo(MsgReceiver_RListenId listenid,MsgReceiver* receiver)
	{
		this->listenid = listenid;
		this->receiver = receiver;
	}
	MsgReceiver_RListenId listenid;
	MsgReceiver* receiver;
};

class MsgReceiver
{
	friend class MsgListener;

public:

private:

	static s32 _ReceiverTotalCount;
	s32 _counter;

public:

	MsgReceiver();
	virtual ~MsgReceiver();

	void removeAllListener();

private:

	template <class TMSG, typename F>
	MsgReceiver_RListenId AddListener(MsgListener* listener, F const& fun, MsgReceiverListenId listerneritr, s32 userdata = 0)
	{
		 //Functor<void, TYPELIST_1( TMSG )> ff( fun );
		//GeneralFunc f( *((GeneralFunc*)( void* )&ff) );

		typedef fastdelegate::FastDelegate1<const TMSG&> Tff;
		Tff ff;
		ff = fastdelegate::MakeDelegate(fun);
		MsgReceiver_GeneralFunc f ( *((MsgReceiver_GeneralFunc*)( void* )&ff) );

		 return AddGeneralListener( listener, TMSG::GetMsgTypeID(), f, listerneritr, userdata );
	}

	template <class TMSG, class P, typename MF>
	MsgReceiver_RListenId AddListener(MsgListener* listener, P const& pobj, MF memfun, MsgReceiverListenId listerneritr, s32 userdata = 0)
	{
		 //Functor<void, TYPELIST_1( const TMSG& )> ff( pobj, memfun );
		 //GeneralFunc f( *((GeneralFunc*)( void* )&ff) );
		typedef fastdelegate::FastDelegate1<const TMSG&> Tff;
		Tff ff;
		ff = fastdelegate::MakeDelegate(pobj, memfun);
		MsgReceiver_GeneralFunc f ( *((MsgReceiver_GeneralFunc*)( void* )&ff) );


		 return AddGeneralListener( listener, TMSG::GetMsgTypeID(), f, listerneritr, userdata );
	}

	template <class P, typename MF>
	MsgReceiver_RListenId AddListener(s32 msgtypeid,MsgListener* listener, P const& pobj, MF memfun, MsgReceiverListenId listerneritr, s32 userdata = 0)
	{
		 //Functor<void, TYPELIST_1( const TMSG& )> ff( pobj, memfun );
		 //GeneralFunc f( *((GeneralFunc*)( void* )&ff) );
		typedef fastdelegate::FastDelegate1<const MsgBase&> Tff;
		Tff ff;
		ff = fastdelegate::MakeDelegate(pobj, memfun);
		MsgReceiver_GeneralFunc f ( *((MsgReceiver_GeneralFunc*)( void* )&ff) );

		 return AddGeneralListener( listener, msgtypeid, f, listerneritr, userdata );
	}

	MsgReceiver_RListenId AddGeneralListener(MsgListener* listener,s32 msgtypeid, const MsgReceiver_GeneralFunc& f, MsgReceiverListenId listerneritr, s32 userdata = 0 )
	{
		 std::list< MsgReceiver_Info >::iterator insertedpos;

		 MAP_TYPE::iterator itr = listeners.find( msgtypeid );
		 if ( itr == listeners.end() )
		 {
			 listeners.insert( MAP_PAIR( msgtypeid, MAP_PAIR::second_type() ) );
			 
			 itr = listeners.find( msgtypeid );
			 insertedpos = itr->second.insert( itr->second.end(), MsgReceiver_Info(listener, f, listerneritr, userdata, _counter ) );
		 }
		 else
		 {
			 insertedpos = itr->second.insert( itr->second.end(), MsgReceiver_Info(listener, f, listerneritr, userdata, _counter ) );
		 }
		 
		return MsgReceiver_RListenId( msgtypeid, insertedpos );
	}

	void RemoveListener(const MsgReceiver_RListenId& listenid)
	{
		if ( _send_Depth == 0 )
		{
			//it's safe to remove it now
			ClearRemovedListener(listenid);
		}
		else
		{
			//delay the removal..since the code is currently iterating through the list we want to remove from
			listenid.itr->removed = true;
			_queued_removedlisteners.push_back( listenid );
		}
	}
public:
	template <typename TMSG>
	void Send( const TMSG & m )
	{
		TMSG& m2 = const_cast<TMSG&>(m);
		
		_counter++;

		MAP_TYPE::iterator itr = listeners.find( TMSG::GetMsgTypeID() );
		if ( itr != listeners.end() )
		{
			_send_Depth++;
			for( std::list< MsgReceiver_Info >::iterator i = itr->second.begin(); i!= itr->second.end(); i++)
			{
				MsgReceiver_Info& info = *i;
				if ( !info.removed && info.counter_created != _counter)
				{
					m2.userdata = info.userdata;
					info.func( *((const MsgBase*)((const void*)&m2)) );
				}
			}
			_send_Depth--;
			if ( _send_Depth == 0 )
			{
				//remove any removed listeners 
				ClearRemovedListeners();
			}
		}
	}

private:

	void ClearRemovedListeners()
	{
		for (std::list< MsgReceiver_RListenId >::iterator itr = _queued_removedlisteners.begin(); itr != _queued_removedlisteners.end(); itr++)
		{
			const MsgReceiver_RListenId& listenid = *itr;
			ClearRemovedListener(listenid);
		}

		_queued_removedlisteners.clear();
	}
	void ClearRemovedListener(const MsgReceiver_RListenId& listenid)
	{
		MAP_TYPE::iterator itr = listeners.find( listenid.msgtypeid );
		if ( itr != listeners.end() )
		{
			itr->second.erase( listenid.itr );

			if ( itr->second.size() == 0 )
			{
				//no more listeners for the msgtype...remove it from the msgtype map
				listeners.erase( itr );
			}

		}
	}

	typedef std::pair< s32, std::list< MsgReceiver_Info > > MAP_PAIR;
	typedef std::map< MAP_PAIR::first_type, MAP_PAIR::second_type > MAP_TYPE;
	MAP_TYPE listeners;

	std::list< MsgReceiver_RListenId > _queued_removedlisteners;
	s32 _send_Depth;
};

class MsgListener
{
private:

	static s32 _ListenerTotalCount;

public:

	MsgListener()
	{
		_ListenerTotalCount++;
	}
	virtual ~MsgListener()
	{
		StopAllListening();
		_ListenerTotalCount--;
	}

	typedef MsgReceiverListenId MsgListenId;

	template <class TMSG, typename F>
	MsgListenId Listen( MsgReceiver& listenonthisreceiver, F const& fun )
	{
		std::list< MsgReceiver_ListenInfo >::iterator itr = _listenids.insert( _listenids.end(), MsgReceiver_ListenInfo() );
		MsgListenId listenid( itr );
		MsgReceiver_ListenInfo info( listenonthisreceiver.AddListener<TMSG>( this, fun, listenid ), &listenonthisreceiver );
		(*itr) = info;
		return listenid;
	}

	template <class TMSG, class P, typename MF>
	MsgListenId Listen( MsgReceiver* listenonthisreceiver, P const& pobj, MF memfun )
	{
		return Listen<TMSG>( *listenonthisreceiver, pobj, memfun );
	}

	template <class TMSG, class P, typename MF>
	MsgListenId Listen( MsgReceiver& listenonthisreceiver, P const& pobj, MF memfun )
	{

		std::list< MsgReceiver_ListenInfo >::iterator itr = _listenids.insert( _listenids.end(), MsgReceiver_ListenInfo() );
		MsgListenId listenid( itr );
		MsgReceiver_ListenInfo info( listenonthisreceiver.AddListener<TMSG>( this, pobj, memfun, listenid ), &listenonthisreceiver );
		(*itr) = info;
		return listenid;
	}

	template <class P, typename MF>
	MsgListenId Listen(s32 msgtypeid, MsgReceiver& listenonthisreceiver, P const& pobj, MF memfun, s32 userdata = 0)
	{
		std::list< MsgReceiver_ListenInfo >::iterator itr = _listenids.insert( _listenids.end(), MsgReceiver_ListenInfo() );
		MsgListenId listenid( itr );
		MsgReceiver_ListenInfo info( listenonthisreceiver.AddListener( msgtypeid, this, pobj, memfun, listenid, userdata ), &listenonthisreceiver );
		(*itr) = info;
		return listenid;
	}

	void StopListening(const MsgListenId& id)
	{
		std::list< MsgReceiver_ListenInfo >::iterator itr = id.itr;
		id.itr->receiver->RemoveListener( itr->listenid );
		_listenids.erase( itr );
		
	}
	
	void StopAllListening()
	{
		for (std::list< MsgReceiver_ListenInfo >::iterator itr = _listenids.begin(); itr != _listenids.end(); itr++)
		{
			MsgReceiver_ListenInfo& info = *itr;
			info.receiver->RemoveListener( info.listenid );
		}
		_listenids.clear();
	}
	

private:
	std::list< MsgReceiver_ListenInfo > _listenids;
};

}
}
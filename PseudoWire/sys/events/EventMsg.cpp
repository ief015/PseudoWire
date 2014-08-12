#include "EventMsg.h"
#include "../Platform.h"

using namespace sys;
using namespace sys::events;

std::vector< std::string > MsgTypeRegistry::msgTypes = std::vector< std::string >();

s32 MsgReceiver::_ReceiverTotalCount = 0;
s32 MsgListener::_ListenerTotalCount = 0;

//--------------------------------------------------
MsgReceiver::MsgReceiver()
{
	_send_Depth = 0;
	_ReceiverTotalCount++;
	_counter = 0;
}
//--------------------------------------------------
MsgReceiver::~MsgReceiver()
{
	removeAllListener();
	_ReceiverTotalCount--;
}
//--------------------------------------------------
void MsgReceiver::removeAllListener()
{
	while ( listeners.size() > 0 )
	{
		MAP_TYPE::iterator itr = listeners.begin();
		s32 count = itr->second.size();
		while ( count > 0 )
		{
			MsgReceiver_Info& info = itr->second.back();
			info.listener->StopListening( info.listerneritr );
			count--;
		}
	}
	listeners.clear();
}
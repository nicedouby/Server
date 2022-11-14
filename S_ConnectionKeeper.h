#pragma once
#include "Network_pch.h"
#include "S_Network_Interface.h"
class S_ConnectionKeeper
{
	public:
	S_ConnectionKeeper(S_Network_Interface* in);
	~S_ConnectionKeeper();
	S_Network_Interface* pNetworkInterface_Ref = nullptr;
};


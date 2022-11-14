#pragma once
#include "Network_pch.h"
#include "S_Network_Interface.h"

class S_Connection
{
	public:
	S_Connection();
	~S_Connection();
	
	//对端用户 地址信息
	sockaddr_in ConnectionAddr_From;
	int Connection_Endpoint_FD = -1;
	//Reformatted 的 对端用户地址信息
	unsigned short Port;
	char* cAddressFrom;

	//上级引用
	S_Network_Interface* pOwner = nullptr;

	//业务线程
	std::thread ChildThread;

	


	inline sockaddr_in& GetAddress_In(){return ConnectionAddr_From;}

	void Handle_ConnectionAddressFrom();
	void PAddressInfo();

	static void Register_Connection_Exit(S_Connection* pPendingDelete);


	
};

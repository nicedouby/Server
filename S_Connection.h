#pragma once
#include "Network_pch.h"
#include "S_Network_Interface.h"

class S_Connection
{
	public:
	S_Connection();
	~S_Connection();
	
	//�Զ��û� ��ַ��Ϣ
	sockaddr_in ConnectionAddr_From;
	int Connection_Endpoint_FD = -1;
	//Reformatted �� �Զ��û���ַ��Ϣ
	unsigned short Port;
	char* cAddressFrom;

	//�ϼ�����
	S_Network_Interface* pOwner = nullptr;

	//ҵ���߳�
	std::thread ChildThread;

	


	inline sockaddr_in& GetAddress_In(){return ConnectionAddr_From;}

	void Handle_ConnectionAddressFrom();
	void PAddressInfo();

	static void Register_Connection_Exit(S_Connection* pPendingDelete);


	
};

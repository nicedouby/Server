#pragma once

#include "Network_pch.h"

class S_Core;
class S_Network_Interface
{
	enum E_SocketPipeline
	{
		E_Zeroed,
		E_Created,
		E_Binded,
		E_Listened
	};
public:
	S_Network_Interface();
	~S_Network_Interface();
	void AllPipelineWork_Gather()
	{
		LocalFD_Initialize();
		Sockaddr_Initialize();
		LocalFD_Bind();
		LocalFD_Listen();
	}
	bool CheckPipelineState() { if (LocalFDPipeline == E_Listened) return true; else return false; }
	void LocalFD_Initialize();
	void Sockaddr_Initialize();
	void LocalFD_Bind();
	void LocalFD_Listen();

	int LocalFD = -1;

	sockaddr_in LocalAddr;
	socklen_t len;

	E_SocketPipeline LocalFDPipeline = E_Zeroed;

	S_Core* pCoreRef = nullptr;
};


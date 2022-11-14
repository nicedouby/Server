#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <thread>
#include "S_Network_Interface.h"
#include "S_Connection.h"
#include "ConnectionService.h"
using namespace std;

class S_Core
{
public:
	S_Core();
	~S_Core();
	S_Network_Interface* pNetwork_Interface = nullptr;

	map<unsigned int, shared_ptr<S_Connection>> m_ConnectionsMap;
	

	virtual void Run();
};


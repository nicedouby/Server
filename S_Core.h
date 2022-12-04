#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <list>
#include <thread>

#include "S_Network_Interface.h"
#include "S_Connection.h"

using namespace std;

class S_Core
{
public:
	S_Core();
	~S_Core();
	S_Network_Interface* pNetwork_Interface = nullptr;

	std::mutex m_MapInsert_Mutex;
	std::mutex m_pending_destroy_Mutex;
	std::mutex m_Pending_Exit;

	map<unsigned int, shared_ptr<S_Connection>> m_ConnectionsMap;
	
	list<S_Connection*> l_Pending_Destroy;

	virtual void Run();
};


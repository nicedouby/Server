#include "S_Core.h"
#include "thread"

//extern auto Lam_ConnectionServiceLoop = [](S_Connection* pConnectionRef);
S_Core::S_Core()
{
}

S_Core::~S_Core()
{
}

void S_Core::Run()
{
	printf("S_Core::Run starts\n");
	pNetwork_Interface = new S_Network_Interface();
	pNetwork_Interface->pCoreRef = this;
	pNetwork_Interface->AllPipelineWork_Gather();
	while(!pNetwork_Interface->CheckPipelineState())	
	{
		printf("Failed to Finish socket pipeline, retrying.....\n");
		sleep(5);
		pNetwork_Interface->AllPipelineWork_Gather();
	}
	auto Lam_AccceptWork = [](S_Core* pCoreRef)
	{
		//将一些必要变量转换为局部变量
		//避免过度指针操作代码，显得代码冗长
		S_Network_Interface* pNetwork_Interface_Ref = pCoreRef->pNetwork_Interface;
		int LocalFD = pNetwork_Interface_Ref->LocalFD;
		
		unsigned int ID = 0;


		while (1)
		{
			//用于accept函数的被接受参数
			sockaddr_in recv_addr;
			socklen_t size = sizeof(sockaddr_in);
			
			//accept
			auto ret = accept(LocalFD, (sockaddr*)&recv_addr, &size);
			
			if (ret == -1)
			{
				printf("Failed to Accept\n");
				perror("Error Code is ");
			}
			else
			{
				printf("Successfully to Accept\n");
				
				shared_ptr<S_Connection> connection(new S_Connection());
				
				//此为拷贝操作，不存在超过作用域 失效等问题
				connection.get()->ConnectionAddr_From = recv_addr;
				
				connection.get()->pOwner = pNetwork_Interface_Ref;
				connection.get()->Connection_Endpoint_FD = ret;
				//Handle Socket Address
				connection.get()->Handle_ConnectionAddressFrom();
				connection.get()->PAddressInfo();
				printf(" has successfully connected\n");
				//auto ret = pCoreRef->m_ConnectionsMap.insert(make_pair(ID, connection));
				//无论上一行代码执行结果是否成功，当前ID都已经失效了，要不就是已经插入成功，要不就是插入失败。

				ID++;
				/*
				//若执行成功，该循环将直接跳过。
				//ID将一直自增直至Insert成功
				while (ret.second != true)
				{
					static int BadLoopCounts = 0;
					BadLoopCounts++;
					ret = pCoreRef->m_ConnectionsMap.insert(make_pair(ID, connection));
					if (ret.second == true)
					{
						ID++;
					}
					else
					{
						printf("Failed to Insert\n");
						auto FindRslt = pCoreRef->m_ConnectionsMap.find(ID);
						//检查该slot 是因为已经被inserted还是其他原因 而无法insert

						if (FindRslt == pCoreRef->m_ConnectionsMap.end())
						{
							//当Find的结果为无效时
							printf("this slot is invalid\n");
						}
						else
						{
							printf("this slot is inserted\n");
						}
						ID++;
					}
				}*/

				connection.get()->ChildThread = thread(Lam_ConnectionServiceLoop,connection);
				connection.get()->ChildThread.detach();
				//sleep(0.1);
				//connection.get()->ChildThread.join();
			}
		/*
		auto a = pCoreRef->m_ConnectionsMap.find(1);
		if (a == pCoreRef->m_ConnectionsMap.end())
		{
			printf("key 1 is good\n");
		}
		a = pCoreRef->m_ConnectionsMap.find(505);
		if (a == pCoreRef->m_ConnectionsMap.end())
		{
			printf("key 505 is good\n");
		}
		int i = 0;
		a = pCoreRef->m_ConnectionsMap.find(i);
		if (a == pCoreRef->m_ConnectionsMap.end())
		{
			printf("key %d is good\n", i);
		}*/
		}
	};
	
	thread accept_loop = thread(Lam_AccceptWork, this);

	while (1)
	{
		//m_ConnectionsMap.
	}
	
}


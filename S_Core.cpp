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
		//��һЩ��Ҫ����ת��Ϊ�ֲ�����
		//�������ָ��������룬�Եô����߳�
		S_Network_Interface* pNetwork_Interface_Ref = pCoreRef->pNetwork_Interface;
		int LocalFD = pNetwork_Interface_Ref->LocalFD;
		
		unsigned int ID = 0;


		while (1)
		{
			//����accept�����ı����ܲ���
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
				
				//��Ϊ���������������ڳ��������� ʧЧ������
				connection.get()->ConnectionAddr_From = recv_addr;
				
				connection.get()->pOwner = pNetwork_Interface_Ref;
				connection.get()->Connection_Endpoint_FD = ret;
				//Handle Socket Address
				connection.get()->Handle_ConnectionAddressFrom();
				connection.get()->PAddressInfo();
				printf(" has successfully connected\n");
				//auto ret = pCoreRef->m_ConnectionsMap.insert(make_pair(ID, connection));
				//������һ�д���ִ�н���Ƿ�ɹ�����ǰID���Ѿ�ʧЧ�ˣ�Ҫ�������Ѿ�����ɹ���Ҫ�����ǲ���ʧ�ܡ�

				ID++;
				/*
				//��ִ�гɹ�����ѭ����ֱ��������
				//ID��һֱ����ֱ��Insert�ɹ�
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
						//����slot ����Ϊ�Ѿ���inserted��������ԭ�� ���޷�insert

						if (FindRslt == pCoreRef->m_ConnectionsMap.end())
						{
							//��Find�Ľ��Ϊ��Чʱ
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


#include "S_Core.h"
#include "iostream"
#include "mutex"
#include <unistd.h>
#include <stdio.h>
#include "ConnectionService.h"
#include "unique_lock.h"
#include "thread"

#define MAX_NUM_OF_THREAD 50

auto Lam_ConnectionServiceLoop = [](std::shared_ptr<S_Connection> pConnectionRef)
{
	if (!pConnectionRef)
	{
		return;
	}
	pConnectionRef->connect_state = E_Connection_Connect_State::Connected;
	pConnectionRef->bussiness_state = E_Bussiness_State::E_Just_Connected;
	//��ȡ�߳�ID
	pthread_t thread_ID = pthread_self();

	//��һЩ��Ҫ����ת��Ϊ�ֲ�����
	//�������ָ��������룬�Եô����߳�
	S_Network_Interface* pNetwork_Interface = pConnectionRef.get()->pOwner;
	S_Core* pCoreRef = pNetwork_Interface->pCoreRef;

	int LocalFD = pConnectionRef.get()->Connection_Endpoint_FD;

	//���ڽ��յĻ�����
	void* buff = malloc(64);
	int recv_size = 64;

	//Hello Text output
	printf("Thread %d is working for ", thread_ID);
	pConnectionRef.get()->PAddressInfo();
	printf("\n");

	//��־�߳�״̬
	pConnectionRef.get()->ChildThreadFlag = E_Thread_Flag::Running;

	//����ҵ��ѭ��
	while (1)
	{
		auto ret = recv(LocalFD, buff, recv_size, 0);
		if (ret == recv_size)
		{
			printf("Successfully to %d bytes recv data \n", recv_size);
			if (pConnectionRef->bussiness_state == E_Bussiness_State::E_Login)
			{
				//Process business after login
			}
			else
			{
				//BufferFactory::
			}
			pConnectionRef->bIsLogIn = true;
		}
		else if (ret == 0)
		{
			pConnectionRef->PAddressInfo();
			printf("Client closed this connection\n");

			pConnectionRef->connect_state = Closed;

			//��ֹ�ڴ�й¶
			free(buff);
			close(LocalFD);

			pCoreRef->m_MapInsert_Mutex.lock();
			//ѭ����ֱ�������õ���
			//�� ����� �������� ��map�� ��� ��ǰ�߳�������� ����
			pConnectionRef->pOwner->pCoreRef->m_ConnectionsMap.erase(pConnectionRef->ID);
			pCoreRef->m_MapInsert_Mutex.unlock();

			pCoreRef->m_pending_destroy_Mutex.lock();
			pCoreRef->l_Pending_Destroy.push_back(pConnectionRef.get());
			pCoreRef->m_pending_destroy_Mutex.unlock();
			std::unique_lock u_lock(m_mutex);
			pConnectionRef.get()->cv_var.wait()

			/*
			while (pConnectionRef.get()->ChildThreadFlag != E_Thread_Flag::ExitAllowed)
			{
				usleep(5000);
			}
			printf("Exit Allowed Found\n");

			pConnectionRef.get()->m_mutex.lock();

			pConnectionRef.get()->m_mutex.unlock();
			*/
			pthread_exit(0);

		}
		else if (ret == -1)
		{
			pConnectionRef->connect_state = Error;
			printf("recv returns %d\n", ret);
			perror("Error Code is ");
		}
	}
};
//extern auto Lam_ConnectionServiceLoop = [](S_Connection* pConnectionRef);
S_Core::S_Core() {}
S_Core::~S_Core() {}
void S_Core::Run()
{
	{
		printf("S_Core::Run starts\n");
		std::cout << "Hardware has best performance on " << std::thread::hardware_concurrency() << " threads" << std::endl;
		pNetwork_Interface = new S_Network_Interface();
		pNetwork_Interface->pCoreRef = this;
		pNetwork_Interface->AllPipelineWork_Gather();
		while (!pNetwork_Interface->CheckPipelineState())
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
				//�������		
				if (ret == -1)
				{
					printf("Failed to Accept\n");
					perror("Error Code is ");
				}
				else
				{
					//printf("Successfully to Accept\n");

					shared_ptr<S_Connection> connection(new S_Connection());

					//��Ϊ���������������ڳ��������� ʧЧ������
					connection.get()->ConnectionAddr_From = recv_addr;

					connection.get()->pOwner = pNetwork_Interface_Ref;
					connection.get()->Connection_Endpoint_FD = ret;
					//Handle Socket Address
					connection.get()->Handle_ConnectionAddressFrom();
					connection.get()->PAddressInfo();

					printf(" has successfully connected\n");
					auto lock_ed = pCoreRef->m_MapInsert_Mutex.try_lock();
					while (lock_ed == false)
					{
						usleep(10000);
						lock_ed = pCoreRef->m_MapInsert_Mutex.try_lock();
					}
					auto ret_1 = pCoreRef->m_ConnectionsMap.insert(make_pair(ID, connection));
					pCoreRef->m_MapInsert_Mutex.unlock();
					//������һ�д���ִ�н���Ƿ�ɹ�����ǰID���Ѿ�ʧЧ�ˣ�Ҫ�������Ѿ�����ɹ���Ҫ�����ǲ���ʧ�ܡ�

					ID++;

					//��ִ�гɹ�����ѭ����ֱ��������
					//ID��һֱ����ֱ��Insert�ɹ�
					while (ret_1.second != true)
					{
						static int BadLoopCounts = 0;
						BadLoopCounts++;
						ret_1 = pCoreRef->m_ConnectionsMap.insert(make_pair(ID, connection));
						if (ret_1.second == true)
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
					}
					connection.get()->ID = ID - 1;
					connection.get()->ChildThread = thread(Lam_ConnectionServiceLoop, connection);
					//connection.get()->ChildThread.join();
					//connection.get()->ChildThread.
				}
			}
		};
		//Lam_AccceptWork(this);
		std::thread accept_thread = std::thread(Lam_AccceptWork, this);
		//accept_thread.detach();
	}
	while (1)
	{
		S_Connection* pPendingCheck = l_Pending_Destroy.front();
		if (pPendingCheck == nullptr)
		{
			usleep(2500);
		}
		else
		{
			if (pPendingCheck->ChildThread.joinable())
			{
				m_pending_destroy_Mutex.lock();

				//pPendingCheck->m_mutex.unlock();

				pPendingCheck->ChildThreadFlag = E_Thread_Flag::ExitAllowed;

				pPendingCheck->ChildThread.join();
				l_Pending_Destroy.pop_front();

				m_pending_destroy_Mutex.unlock();
			}
		}

		//m_ConnectionsMap.
	}
}


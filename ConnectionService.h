#pragma once
#include "Network_pch.h"
#include "S_Network_Interface.h"
#include "S_Connection.h"
class S_Connection;

#ifndef Lam_ConnectionServiceLoop
auto Lam_ConnectionServiceLoop = [](std::shared_ptr<S_Connection> pConnectionRef)
{
	if (!pConnectionRef)
	{
		return;
	}
	
	//获取线程ID
	pthread_t thread_ID = pthread_self();

	//将一些必要变量转换为局部变量
	//避免过度指针操作代码，显得代码冗长
	S_Network_Interface* pNetwork_Interface = pConnectionRef.get()->pOwner;
	int LocalFD = pConnectionRef.get()->Connection_Endpoint_FD;

	//用于接收的缓冲区
	void* buff = malloc(64);
	int recv_size = 64;

	printf("Thread %d is working for ", thread_ID);
	pConnectionRef.get()->PAddressInfo();
	printf("\n");

	while (1)
	{
		int ret = recv(LocalFD, buff, recv_size, 0);
		if (ret == recv_size)
		{
			printf("Successfully to %d bytes recv data \n", recv_size);
		}
		else if (ret == 0)
		{
			printf("Client closed this connection\n");
			//防止内存泄露
			free(buff);
			printf("use count : %d\n",pConnectionRef.use_count());
			//S_Connection::Register_Connection_Exit(pConnectionRef);
			//delete pConnectionRef;
			pthread_exit(0);
		}
		else if (ret == -1)
		{
			printf("recv returns %d\n", ret);
			perror("Error Code is ");
		}
	}
};
#endif
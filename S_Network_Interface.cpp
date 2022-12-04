#include "S_Network_Interface.h"
#include "NetworkCFG.h"
S_Network_Interface::S_Network_Interface()
{
}

S_Network_Interface::~S_Network_Interface()
{
}

//生成套接字 (即 File Descriptor) 
void S_Network_Interface::LocalFD_Initialize()
{
	if (LocalFDPipeline >= E_Created)
	{
		return;
	}
	LocalFD = socket(AF_INET,SOCK_STREAM,0);
	if (LocalFD == -1)
	{
		printf("Invalid Socket\n");
	}
	else
	{
		LocalFDPipeline = E_Created;
	}

}
//初始化本地地址信息
void S_Network_Interface::Sockaddr_Initialize()
{

	LocalAddr.sin_family = AF_INET;

	LocalAddr.sin_port = htons(DS_SERVER_PORT);

	LocalAddr.sin_addr.s_addr = INADDR_ANY;
	len = sizeof(LocalAddr);
}
//将本地信息绑定至之前生成的套接字
void S_Network_Interface::LocalFD_Bind()
{
	if (LocalFDPipeline == E_Zeroed)
	{
		return;
	}
	int optval = 1;
	int ret = setsockopt(LocalFD,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	printf("First setsockopt(ADDR) returns %d\n",ret);
	optval = 1;
	setsockopt(LocalFD, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	printf("Second setsockopt(PORT) returns %d\n", ret);
	ret = bind(LocalFD,(sockaddr*)&LocalAddr,sizeof(sockaddr));

	if (ret == -1)
	{
		printf("Failed to bind\n");
		perror("Error code is ");
		return;
	}
	LocalFDPipeline = E_Binded;
}
//监听生成的套接字
void S_Network_Interface::LocalFD_Listen()
{
	if (LocalFDPipeline < E_Binded)
	{
		return;
	}
	auto ret = listen(LocalFD, 5);
	if (ret == -1)
	{
		printf("Failed to Listen\n");
		perror("Error Code is ");
		return;
	}
	LocalFDPipeline = E_Listened;
}

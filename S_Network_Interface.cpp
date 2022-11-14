#include "S_Network_Interface.h"
#include "NetworkCFG.h"
S_Network_Interface::S_Network_Interface()
{
}

S_Network_Interface::~S_Network_Interface()
{
}

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

void S_Network_Interface::Sockaddr_Initialize()
{

	LocalAddr.sin_family = AF_INET;

	LocalAddr.sin_port = htons(DS_SERVER_PORT);

	LocalAddr.sin_addr.s_addr = INADDR_ANY;
	len = sizeof(LocalAddr);
}

void S_Network_Interface::LocalFD_Bind()
{
	if (LocalFDPipeline == E_Zeroed)
	{
		return;
	}
	int ret = bind(LocalFD,(sockaddr*)&LocalAddr,sizeof(sockaddr));
	if (ret == -1)
	{
		printf("Failed to bind\n");
		perror("Error code is ");
		return;
	}
	LocalFDPipeline = E_Binded;
}

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

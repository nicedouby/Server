#include "S_Connection.h"
#include "S_Core.h"
S_Connection::S_Connection()
{
	
}

S_Connection::~S_Connection()
{
	//标记当前实例被解析，用于追踪内存分配
	printf("~S_Connection\n");
}
//解析地址信息
void S_Connection::Handle_ConnectionAddressFrom()
{
	//端口号
	Port = htons(GetAddress_In().sin_port);
	//IP地址
	cAddressFrom = inet_ntoa(GetAddress_In().sin_addr);
}
//输出被解析的地址信息
void S_Connection::PAddressInfo()
{
	printf("%s : %d",cAddressFrom,Port);	
}

void S_Connection::Register_Connection_Exit(S_Connection* pPendingDelete)
{
	//printf("A Connection is requesting exit\n");
	S_Core* pCoreRef = pPendingDelete->pOwner->pCoreRef;




	
	//TODO: Register Work......
}

#include "S_Connection.h"

S_Connection::S_Connection()
{

}

S_Connection::~S_Connection()
{
	printf("~S_Connection\n");
}

void S_Connection::Handle_ConnectionAddressFrom()
{
	Port = htons(GetAddress_In().sin_port);
	cAddressFrom = inet_ntoa(GetAddress_In().sin_addr);
}

void S_Connection::PAddressInfo()
{
	printf("%s : %d",cAddressFrom,Port);	
}

void S_Connection::Register_Connection_Exit(S_Connection* pPendingDelete)
{
	printf("A Connection is requesting exit\n");
	S_Core* pCoreRef = pPendingDelete->pOwner->pCoreRef;
}

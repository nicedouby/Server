#include "S_Connection.h"
#include "S_Core.h"
S_Connection::S_Connection()
{
	
}

S_Connection::~S_Connection()
{
	//��ǵ�ǰʵ��������������׷���ڴ����
	printf("~S_Connection\n");
}
//������ַ��Ϣ
void S_Connection::Handle_ConnectionAddressFrom()
{
	//�˿ں�
	Port = htons(GetAddress_In().sin_port);
	//IP��ַ
	cAddressFrom = inet_ntoa(GetAddress_In().sin_addr);
}
//����������ĵ�ַ��Ϣ
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

#pragma once
#include "Network_pch.h"
#include "S_Network_Interface.h"

enum E_Connection_Connect_State
{
	Connect_Zero,
	Connected,
	Error,
	Closed
};
enum E_Bussiness_State
{
	Bussiness_Zero,
	E_Just_Connected,
	E_First_Packet_Processed,//It means this connection has process a packet
	E_Tried_Login,//Means this connection tried Login but failed for some reason
	E_Login//Means this connection has already login!!
};
enum E_Thread_Flag
{
	None,
	Running,
	Pending,
	ExitAllowed
};

class S_Connection
{
	
	public:
	S_Connection();
	~S_Connection();
	
	//�Զ��û� ��ַ��Ϣ
	sockaddr_in ConnectionAddr_From;
	int Connection_Endpoint_FD = -1;

	//Reformatted �� �Զ��û���ַ��Ϣ
	unsigned short Port = 0;
	char* cAddressFrom = 0;

	//Additional Buffer used for extending S_Connection
	//For More Features 
	void* ExtendBuffer = 0;
	unsigned int cbSizeOfExtendBuffer = 0;

	//��ID���� ���� ������ �洢���ӵ�map �еĵ�ǰ����
	unsigned int ID;

	//�ϼ�����
	S_Network_Interface* pOwner = nullptr;
	
	//��ǰ״̬
	E_Connection_Connect_State connect_state = E_Connection_Connect_State::Connect_Zero;
	E_Bussiness_State bussiness_state = E_Bussiness_State::Bussiness_Zero;
	
	//ҵ���߳�
	std::thread ChildThread;
	E_Thread_Flag ChildThreadFlag = E_Thread_Flag::None;

	std::mutex m_mutex;
	std::condition_variable cv_var;


	//��չҵ����ر���	
	bool bIsLogIn = false;	

	//���Զ˵�ַ��Ϣ �����л��� �� ���ɶ��� ��ʽ
	void Handle_ConnectionAddressFrom();
	void PAddressInfo();

	inline sockaddr_in& GetAddress_In() { return ConnectionAddr_From; }

	//��ChildThread�˳�ʱ��ͨ�����ô˺�����ע����Ϣ
	static void Register_Connection_Exit(S_Connection* pPendingDelete);


	
};

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
	
	//对端用户 地址信息
	sockaddr_in ConnectionAddr_From;
	int Connection_Endpoint_FD = -1;

	//Reformatted 的 对端用户地址信息
	unsigned short Port = 0;
	char* cAddressFrom = 0;

	//Additional Buffer used for extending S_Connection
	//For More Features 
	void* ExtendBuffer = 0;
	unsigned int cbSizeOfExtendBuffer = 0;

	//此ID用于 快速 检索在 存储连接的map 中的当前连接
	unsigned int ID;

	//上级引用
	S_Network_Interface* pOwner = nullptr;
	
	//当前状态
	E_Connection_Connect_State connect_state = E_Connection_Connect_State::Connect_Zero;
	E_Bussiness_State bussiness_state = E_Bussiness_State::Bussiness_Zero;
	
	//业务线程
	std::thread ChildThread;
	E_Thread_Flag ChildThreadFlag = E_Thread_Flag::None;

	std::mutex m_mutex;
	std::condition_variable cv_var;


	//扩展业务相关变量	
	bool bIsLogIn = false;	

	//将对端地址信息 “序列化” 成 “可读” 形式
	void Handle_ConnectionAddressFrom();
	void PAddressInfo();

	inline sockaddr_in& GetAddress_In() { return ConnectionAddr_From; }

	//当ChildThread退出时，通过调用此函数来注册信息
	static void Register_Connection_Exit(S_Connection* pPendingDelete);


	
};

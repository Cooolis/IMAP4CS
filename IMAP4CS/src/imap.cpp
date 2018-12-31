#include "imap.h"


imapClient::imapClient(){
	std::cout << "Imap Client loaded .... " << std::endl;
}

bool imapClient::imapInitSock(){
	WORD sockVersion = MAKEWORD(2,2); 
	WSADATA WSAData;

    if(WSAStartup(sockVersion, &WSAData)!=0)  
    {  
		std::cout << "Error :" << GetLastError() << std::endl;
        return FALSE;  
    }
	try{
		this->imapSock = socket(AF_INET,SOCK_STREAM,0); // TCP 套接字
		if(this->imapSock == INVALID_SOCKET){
			throw "Create Socket Error ...";
			// return FALSE;
		}
	}catch(char * msg){
		std::cout << msg <<std::endl;
		return FALSE;
	}
	return TRUE;
}
bool imapClient::imapConnect(std::string host,USHORT port){
	struct sockaddr_in imapServerAddr; // 设置端口
	imapServerAddr.sin_addr.S_un.S_addr = inet_addr(host.c_str());
	imapServerAddr.sin_port = htons(port);
	
	imapServerAddr.sin_family = AF_INET;
	try{
		int coon = connect(this->imapSock,(struct sockaddr *)&imapServerAddr,sizeof(imapServerAddr));
		if(coon == SOCKET_ERROR){
			throw "Connect to Server Fail!!";
		}
	}catch(char * msg ){
		std::cout << msg <<std::endl;
		return FALSE;
	}
	return TRUE;
}

imapClient::~imapClient(){

	closesocket(this->imapSock); // 关闭socket
	WSACleanup();
}

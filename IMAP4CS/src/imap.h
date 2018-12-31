#ifndef IMAP
#define IMAP
#define DEBUG 1

#include <iostream>
#include <Windows.h>
#include <winsock.h>


class imapClient{
public:
	imapClient();
	bool imapInitSock();
	bool imapConnect(std::string host,USHORT port);
	~imapClient();
private:
	
	int imapSock;
	int imapSendCommand(std::string command);
};



#endif // IMAP

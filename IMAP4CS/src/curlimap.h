#ifndef CURL_IMAP
#define CURL_IMAP
#include <curl\curl.h>
#include <iostream>
#include <string>
#include <vector>
class curlImap{

public:
	curlImap();
	~curlImap();
	void setOptions(std::string,std::string,std::string);
	DWORD getUID(std::string,std::vector<USHORT>*);
	bool debugImapQuery(std::string command);
	DWORD updateMessageID(DWORD *,std::vector<USHORT>*);
	std::string getPayload(DWORD);
private:
	CURL * pcurl;
	std::string userdata;
	std::string username;
	std::string password;
	static SIZE_T write_callback(PCHAR ptr,SIZE_T size_t,SIZE_T nmemb,std::string * userdata);
	static SIZE_T header_callback(PCHAR ptr,SIZE_T size_t,SIZE_T nmemb,std::string * userdata);
	std::string requestImapServer(std::string);
};

#endif // !CURL_IMAP

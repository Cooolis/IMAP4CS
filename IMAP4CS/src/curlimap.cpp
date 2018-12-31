#include "curlimap.h"

// ��ʼ��curl
curlImap::curlImap(){

	userdata = "";
	try{
		this->pcurl = curl_easy_init();
	}catch(...){
		std::cout << "curl init faild !! " << std::endl;
	}
}

// ����IMAP���������û���������
void curlImap::setOptions(std::string Imap,std::string user,std::string pass){
	Imap.append("/INBOX");
	curl_easy_setopt(this->pcurl,CURLOPT_USERNAME,user.c_str());
	curl_easy_setopt(this->pcurl,CURLOPT_PASSWORD,pass.c_str());
	curl_easy_setopt(this->pcurl,CURLOPT_URL,Imap.insert(0,"imaps://").c_str());
	curl_easy_setopt(this->pcurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(this->pcurl, CURLOPT_SSL_VERIFYHOST, 0L);
	//curl_easy_setopt(this->pcurl,CURLOPT_WRITEFUNCTION,&curlImap::write_callback);
	//curl_easy_setopt(this->pcurl,CURLOPT_WRITEDATA,&userdata);
	curl_easy_setopt(this->pcurl,CURLOPT_HEADERDATA,&userdata);
	//CURLOPT_HEADERDATA
	curl_easy_setopt(this->pcurl,CURLOPT_HEADERFUNCTION,&curlImap::header_callback);
	std::wcout << "Imap URL :" << Imap.c_str() << std::endl; 
}

// ��IMAP����������ָ��
std::string curlImap::requestImapServer(std::string command){
	userdata.clear(); // ����ϴ�ִ������������
	std::cout << "[*]Command :" << command.c_str() << std::endl;
	// ����ִ��ָ��
	curl_easy_setopt(this->pcurl, CURLOPT_CUSTOMREQUEST, command.c_str());

	CURLcode res = CURLE_OK;

	//����ִ��ָ��
	res = curl_easy_perform(this->pcurl);

	// �ж�ִ�н��
	if(res != CURLE_OK){
		std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		return "";
	}else{
		// std::cout << "curl_easy_perform() success !" << std::endl;
	}
	//����ִ�н��
	return userdata;
}

// ����Э���д�ص�����
 SIZE_T curlImap::write_callback(PCHAR ptr,SIZE_T size,SIZE_T nmemb,std::string * userdata){
		SIZE_T readSize = size * nmemb;
		if(userdata == NULL){
			return 0;
		}
		// std::cout << std::string(ptr, size*nmemb);
		userdata->append(ptr,readSize);
		// std::cout << userdata->length() << std::endl;
		return readSize;
}

 // ��ȡIMAPԭʼ��Ӧ����
 SIZE_T curlImap::header_callback(PCHAR ptr,SIZE_T size,SIZE_T nmemb,std::string * userdata){
	//std::cout << std::string(ptr, size*nmemb);
	 SIZE_T readSize = size * nmemb;
		if(userdata == NULL){
			return 0;
		}
		//std::cout << std::string(ptr, size*nmemb);
		userdata->append(ptr,readSize);
		//std::cout << userdata->length() << std::endl;
		return readSize;
 }

 /*
 ----------------------------------
 �����������ݣ���ȡ�ʼ�UID
 ----------------------------------
 */
 DWORD curlImap::getUID(std::string from,std::vector<USHORT>* vUID){
	std::string command = "SEARCH FROM ";
	command.append(from);
	// ��������ָ��
	requestImapServer(command);

	DWORD dwSearchFlag = userdata.find("SEARCH");
	// SEARCH\r\n
	// SEARCH xxx xxx \r\n
	// 123456\r\n
	// 123456789\r\n
	DWORD dwCRLF = userdata.find("\r\n",dwSearchFlag);
	// ��ȡUID����
	DWORD dwMaxLen = dwCRLF - (dwSearchFlag+6);
	std::cout << "dwSearchFlag :" << dwSearchFlag << ",dwCRLF :" << dwCRLF <<",dwMaxLen :" << dwMaxLen << std::endl;
	// �ж�UID�Ƿ����
	if(dwMaxLen == 0){
		// std::cout << "---------------!!!----------------" << std::endl;
		std::string searchString = "";
		return -1;
	}else{

		dwMaxLen--;
		std::string searchString = userdata.substr(dwSearchFlag+7,dwMaxLen); //��ȡUID
		// std::cout << searchString << std::endl;

		// �ַ�����ȡ
		DWORD dwStart = 0;
		DWORD dwEnd = 0;
		if(searchString.find(" ",dwStart) == std::string::npos){
			vUID->push_back(std::stoi(searchString));
			return 0;
		}
		while((dwEnd = searchString.find(" ",dwStart)) != std::string::npos){
			vUID->push_back(std::stoi(searchString.substr(dwStart,dwEnd-dwStart)));
			// std::cout << searchString.substr(dwStart,dwEnd-dwStart) << std::endl;
			dwStart = dwEnd+1;
		}
		// �ַ�����ȡ����
	}
	// std::cout << "--------------------------------------" << std::endl;
	return 0;
 }

 // IMAP ���Խӿ�
 bool curlImap::debugImapQuery(std::string command){
	
	 std::cout << requestImapServer(command) << std::endl;
	 return true;
 
 }

 // ����Message-ID
 DWORD curlImap::updateMessageID(DWORD * MessageID,std::vector<USHORT>* vUID){
	 for (UINT i = 0; i < vUID->size(); i++)
	 {
		 std::string command = "Fetch ";
		 command += std::to_string(vUID->at(i));
		 command +=" BODY[header.fields (Message-ID)]";
		 std::cout << requestImapServer(command) << std::endl;
		 DWORD dwStart = userdata.find("Message-ID: ");
		 DWORD dwEnd = userdata.find("\r\n",dwStart);
		 if(dwStart == std::string::npos){
			return -1;
		 }

		 dwStart += 12;
		 
		 DWORD dwCurrentMessgeID = std::stoi(userdata.substr(dwStart,dwEnd-dwStart));
		 std::cout <<"MessageID : " << dwCurrentMessgeID << std::endl;
		 if(*MessageID == dwCurrentMessgeID-1){
			 std::cout << "Current Mail :" << vUID->at(i) << std::endl;
			return vUID->at(i);
		 }else{
			continue;
		 }
	 }
	return -1;
 }

 // ��ȡUID�ʼ�����
 std::string curlImap::getPayload(DWORD dwMailUID){
	// Fetch dwMailUID RFC822.TEXT
	std::string command = "Fetch ";
	command += std::to_string(dwMailUID);
	command +=" RFC822.TEXT";
	requestImapServer(command);
	// std::cout << "/>" << userdata.substr(0,200)<<std::endl;
	DWORD dwEndFlag = userdata.find("}");
	std::string payload = userdata.substr(dwEndFlag+3);
	// std::cout << "/>" << payload.substr(0,20)<<std::endl;
	if(payload.at(0) == 'M'){
		std::cout << "Success !" << std::endl;
		return payload;
	}
	return "";
 }

 // ����
curlImap::~curlImap(){

	curl_easy_cleanup(this->pcurl);
}
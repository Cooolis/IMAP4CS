#include "inetWin.h"

// 遍历本机TCP连接状态，判断是否连接某个端口，返回Dest IP
DWORD inetWin::findPort(USHORT port,ULONG * remoteAddr){
	DWORD dwSize;
	std::vector<UCHAR> buffer;
	DWORD dwRetValue;
	dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
	do{
        buffer.resize(dwSize, 0);
        dwRetValue = GetExtendedTcpTable(buffer.data(), &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    } while( dwRetValue == ERROR_INSUFFICIENT_BUFFER);
	// resize
    if(dwRetValue == ERROR_SUCCESS)
    {
        PMIB_TCPTABLE_OWNER_PID ptTable = (PMIB_TCPTABLE_OWNER_PID)buffer.data();
        for(DWORD i = 0; i < ptTable->dwNumEntries; i++)
		{
            DWORD dwPID = ptTable->table[i].dwOwningPid;
            USHORT uPort = htons((unsigned short)ptTable->table[i].dwRemotePort);
			if(uPort == port)
			{
				*remoteAddr = ptTable->table[i].dwRemoteAddr;
				return dwPID;
			}
		}

	}else
	{
		port = -1;
		remoteAddr = NULL;
	}
    return -1;
}

// 遍历本机TCP连接状态，判断是否连接某个端口，返回Dest IP列表
DWORD inetWin::findPort(std::vector<USHORT> port ,ULONG * remoteAddr){
	DWORD dwSize;
	std::vector<UCHAR> buffer;
	DWORD dwRetValue;
	dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
	do{
        buffer.resize(dwSize, 0);
        dwRetValue = GetExtendedTcpTable(buffer.data(), &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    } while( dwRetValue == ERROR_INSUFFICIENT_BUFFER);
	// resize
    if(dwRetValue == ERROR_SUCCESS)
    {
        PMIB_TCPTABLE_OWNER_PID ptTable = (PMIB_TCPTABLE_OWNER_PID)buffer.data();
        for(DWORD i = 0; i < ptTable->dwNumEntries; i++)
		{
            DWORD dwPID = ptTable->table[i].dwOwningPid;
            USHORT uPort = htons((unsigned short)ptTable->table[i].dwRemotePort);
			if(std::count(port.begin(),port.end(),uPort) != 0)
			{
				*remoteAddr = ptTable->table[i].dwRemoteAddr;
				return dwPID;
			}
		}

	}else
	{
		port.clear();
		remoteAddr = NULL;
	}

    return -1;

}
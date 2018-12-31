#ifndef INETWIN
#define INETWIN
#include <Windows.h>
#include <Iprtrmib.h>
#include <IPHlpApi.h>
#include <iostream>
#include <string>
#include <vector>
class inetWin{


public:
	DWORD findPort(USHORT,ULONG * );
	DWORD findPort(std::vector<USHORT>,ULONG *);

};

#endif

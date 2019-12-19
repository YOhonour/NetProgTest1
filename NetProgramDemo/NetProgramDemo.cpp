#include <iostream>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"iphlpapi")
#pragma comment(lib,"Ws2_32")
/*
将整数类型ip地址转换为点分十进制并输出
*/
void printIPv4(ULONG ulHostIP) {
	ULONG ip1 = ulHostIP, ip2 = ulHostIP, ip3 = ulHostIP, ip4 = ulHostIP;
	//移位并赋值
	ip1 = ip1 >> 24;
	ip2 = (ip2 << 8) >> 24;
	ip3 = (ip3 << 16) >> 24;
	ip4 = (ip4 << 24) >> 24;
	std::cout << ip1 << "." << ip2 << "." << ip3 << "." << ip4 << std::endl;
}
/*
遍历ip表的前dwSize项，打印信息
*/
void searchIPTable(MIB_IPADDRTABLE* pIPAddrTable) {
	for (int i = 0; i < pIPAddrTable->dwNumEntries; i++)
	{
		ULONG ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
		//字节序转换
		ULONG ulHostIP = ntohl(pIPAddrTable->table[i].dwAddr);
		MIB_IPADDRROW CorrentIpTable = pIPAddrTable->table[i];
		ULONG ip[4] = { 0 };
		std::cout << "\n\n第" << i << "个IP地址为：";
		printIPv4(ulHostIP );
		ULONG mask[4] = {0};
		ULONG msk = ntohl(pIPAddrTable->table[i].dwMask);
		std::cout <<"子网掩码为：";
		printIPv4(msk);
		std::cout << "wType为 "<< CorrentIpTable.wType;
		std::cout << "\ndwIndex为 " << CorrentIpTable.dwIndex;
	}
}
/*
执行探测
*/
void run(MIB_IPADDRROW currentTable) {
	ULONG ulHostIP = ntohl(currentTable.dwAddr);
	ULONG mask = ntohl(currentTable.dwMask);
	for (ULONG i = 1; i < (~mask); i++)
	{
		static ULONG uNo = 0;
		HRESULT hr;
		IPAddr ipaddr;
		unsigned char pulMac[6];
		ULONG ulLen;
		ipaddr = htonl(i + (ulHostIP & mask));//转换为网络字节序
		memset(pulMac, 0xff, sizeof(pulMac));
		ulLen = 6;
		hr = SendARP(ipaddr, 0, pulMac, &ulLen);
		if (ulLen == 6)
		{
			uNo++;
			unsigned char* pbHexMac = pulMac;
			unsigned char* strIpAddr = (unsigned char*)(&ipaddr);//解析时将按照字节进行划分解析
			printf("%d:MAC Address %02X:%02X:%02X:%02X:%02X:%02X IP Address %d.%d.%d.%d\n"
				, uNo, pbHexMac[0], pbHexMac[1], pbHexMac[2], pbHexMac[3], pbHexMac[4], pbHexMac[5],
				strIpAddr[0], strIpAddr[1], strIpAddr[2], strIpAddr[3]);
		}
	}
}
int main()
{
	MIB_IPADDRTABLE* pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
	ULONG dwSize = 0, dwRetVel = 0;
	if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
		free(pIPAddrTable);
		//重新申请返回条数的IP表
		pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE)*dwSize);
	}
	if ((dwRetVel = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) == NO_ERROR) {
		std::cout << "IPTable中dwNumEntries为：" << pIPAddrTable->dwNumEntries << "\n";
		std::cout << "IPTable中共有" << dwSize << "条数据";
		int ThedwNumEntries = pIPAddrTable->dwNumEntries;
		searchIPTable(pIPAddrTable);//遍历IPTable
		ThedwNumEntries--;
		std::cout << "\n\n请输入查看探测的局域网序号"<<"(0~"<< ThedwNumEntries <<")：";
		int netNum;
		scanf_s("%d", &netNum);
		/*
		开始探测
		*/
		run(pIPAddrTable->table[netNum]);
	}
	else
	{
		return -1;
		std::cout<<"Call to GetIpAddrTable failed";
	}
	std::cout << "OVER!";
	free(pIPAddrTable);//释放堆资源
	return 0;
}


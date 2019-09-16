// NetProgramDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"iphlpapi")
#pragma comment(lib,"Ws2_32")

void prtIP(ULONG ulHostIP) {
	ULONG ip1 = ulHostIP, ip2 = ulHostIP, ip3 = ulHostIP, ip4 = ulHostIP;


	ip1 = ip1 >> 24;
	ip2 = (ip2 << 8) >> 24;
	ip3 = (ip3 << 16) >> 24;
	ip4 = (ip4 << 24) >> 24;

	std::cout << ip1 << "." << ip2 << "." << ip3 << "." << ip4 << std::endl;
}
/*
将整数类型ip地址转换为点分十进制并输出
*/
void getIPv4(ULONG ulHostIP, ULONG* retIP1, ULONG* retIP2, ULONG* retIP3, ULONG* retIP4) {
	ULONG ip1 = ulHostIP, ip2 = ulHostIP, ip3 = ulHostIP, ip4 = ulHostIP;


	*retIP1 = ip1 >> 24;
	*retIP2 = (ip2 << 8) >> 24;
	*retIP3 = (ip3 << 16) >> 24;
	*retIP4 = (ip4 << 24) >> 24;

	std::cout << *retIP1 << "." << *retIP2 << "." << *retIP3 << "." << *retIP4 << std::endl;
}
/*
遍历ip表的前dwSize项，打印信息
*/
void searchIPTable(ULONG dwSize, MIB_IPADDRTABLE* pIPAddrTable) {
	for (int i = 0; i < dwSize; i++)
	{
		ULONG ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
		
		ULONG ulHostIP = ntohl(pIPAddrTable->table[i].dwAddr);
		MIB_IPADDRROW CorrentIpTable = pIPAddrTable->table[i];
		ULONG ip[4] = { 0 };
		std::cout << "\n第" << i << "个IP地址为：";
		getIPv4(ulHostIP, ip, ip+1, ip+2, ip+3);
		//std::cout << ulHostIP << std::endl;
		ULONG mask[4] = {0};
		ULONG msk = ntohl(pIPAddrTable->table[i].dwMask);
		std::cout <<"子网掩码为：";
		getIPv4(msk, mask, mask + 1, mask + 2, mask + 3);
		std::cout << "wType为 "<< CorrentIpTable.wType;
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
		ULONG ulHostIP = ntohl(pIPAddrTable->table[0].dwAddr);

		std::cout << "\nIPTable中dwNumEntries为：" << pIPAddrTable->dwNumEntries << "\n";
		std::cout << "\nIPTable中共有" << dwSize << "条数据\n";
		ULONG mask = ntohl(pIPAddrTable->table[0].dwMask);
		std::cout<<(~mask)<<"\n";
		for (ULONG i = 1; i < (~mask); i++)
		{
			static ULONG uNo = 0;
			HRESULT hr;
			IPAddr ipaddr;
			ULONG pulMac[2];
			ULONG ulLen;
			ipaddr = htonl(i+ (ulHostIP & mask));//转换为网络字节序
			memset(pulMac, 0xff, sizeof(pulMac));
			
			//ULONG ip[4] = { 0 };
			//getIPv4(i + (ulHostIP & mask), ip, ip + 1, ip + 2, ip + 3);

			ulLen = 6;
			hr = SendARP(ipaddr, 0, pulMac, &ulLen);
			if (ulLen == 6)
			{
				uNo++;
				PBYTE pbHexMac = (PBYTE)pulMac;
				unsigned char* strIpAddr = (unsigned char*)(&ipaddr);//解析时将按照
				printf("%d:MAC Address %02X:%02X:%02X:%02X:%02X:%02X IP Address %d.%d.%d.%d\n"
					,uNo, pbHexMac[0], pbHexMac[1], pbHexMac[2], pbHexMac[3], pbHexMac[4], pbHexMac[5],
					strIpAddr[0], strIpAddr[1], strIpAddr[2], strIpAddr[3]);
			}
		}
		//searchIPTable(d, pIPAddrTable);
	}
	else
	{
		std::cout<<"Call to GetIpAddrTable failed";
	}
	std::cout << "OVER!";
	free(pIPAddrTable);
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
